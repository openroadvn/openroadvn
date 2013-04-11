#!/usr/bin/php
<?php
// $Id: project-usage-process.php,v 1.7 2009/08/07 15:42:32 dww Exp $


/**
 * @file
 * Processes the project_usage statistics.
 *
 * @author Andrew Morton (http://drupal.org/user/34869)
 * @author Derek Wright (http://drupal.org/user/46549)
 *
 */

// ------------------------------------------------------------
// Required customization
// ------------------------------------------------------------

// The root of your Drupal installation, so we can properly bootstrap
// Drupal. This should be the full path to the directory that holds
// your index.php file, the "includes" subdirectory, etc.
define('DRUPAL_ROOT', '');

// The name of your site. Required so that when we bootstrap Drupal in
// this script, we find the right settings.php file in your sites folder.
define('SITE_NAME', '');


// ------------------------------------------------------------
// Initialization
// (Real work begins here, nothing else to customize)
// ------------------------------------------------------------

// Check if all required variables are defined
$vars = array(
  'DRUPAL_ROOT' => DRUPAL_ROOT,
  'SITE_NAME' => SITE_NAME,
);
$fatal_err = FALSE;
foreach ($vars as $name => $val) {
  if (empty($val)) {
    print "ERROR: \"$name\" constant not defined, aborting\n";
    $fatal_err = TRUE;
  }
}
if ($fatal_err) {
  exit(1);
}

$script_name = $argv[0];

// Setup variables for Drupal bootstrap
$_SERVER['HTTP_HOST'] = SITE_NAME;
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_URI'] = '/' . $script_name;
$_SERVER['SCRIPT_NAME'] = '/' . $script_name;
$_SERVER['PHP_SELF'] = '/' . $script_name;
$_SERVER['SCRIPT_FILENAME'] = $_SERVER['PWD'] .'/'. $script_name;
$_SERVER['PATH_TRANSLATED'] = $_SERVER['SCRIPT_FILENAME'];

if (!chdir(DRUPAL_ROOT)) {
  print "ERROR: Can't chdir(DRUPAL_ROOT), aborting.\n";
  exit(1);
}
// Make sure our umask is sane for generating directories and files.
umask(022);

require_once 'includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

if (!module_exists('project_usage')) {
  wd_err(t("ERROR: Project usage module does not exist, aborting.\n"));
  exit(1);
}

// Load the API functions we need for manipulating dates and timestamps.
module_load_include('inc', 'project_usage', 'includes/date_api');

// ------------------------------------------------------------
// Call the daily and weekly processing tasks as needed.
// ------------------------------------------------------------

$now = time();

// Figure out if it's been 24 hours since our last daily processing.
if (variable_get('project_usage_last_daily', 0) <= ($now - PROJECT_USAGE_DAY)) {
  project_usage_process_daily();
  variable_set('project_usage_last_daily', $now);
}

// We can't process the weekly data until the week has completed. To see if
// there's data available: determine the last time we completed the weekly
// processing and compare that to the start of this week. If the last
// weekly processing occurred before the current week began then there should
// be one (or more) week's worth of data ready to process.
$default = $now - variable_get('project_usage_life_daily', 4 * PROJECT_USAGE_WEEK);
$last_weekly = variable_get('project_usage_last_weekly', $default);
$current_week_start = project_usage_weekly_timestamp(NULL, 0);
if ($last_weekly <= $current_week_start) {
  project_usage_process_weekly($last_weekly);
  variable_set('project_usage_last_weekly', $now);
  // Reset the list of active weeks.
  project_usage_get_active_weeks(TRUE);
}

// Wipe the cache of all expired usage pages.
cache_clear_all(NULL, 'cache_project_usage');


/**
 * Process all the raw data up to the previous day.
 *
 * The primary key on the {project_usage_raw} table will prevent duplicate
 * records provided we process them once the day is complete. If we pull them
 * out too soon and the site checks in again they will be counted twice.
 */
function project_usage_process_daily() {
  // Timestamp for begining of the previous day.
  $timestamp = project_usage_daily_timestamp(NULL, 1);
  $time_0 = time();

  watchdog('project_usage', 'Starting to process daily usage data for !date.', array('!date' => format_date($timestamp, 'custom', 'Y-m-d')));

  // Assign API version term IDs.
  $terms = array();
  foreach (project_release_get_api_taxonomy() as $term) {
    $terms[$term->tid] = $term->name;
  }
  $num_updates = 0;
  $query = db_query("SELECT DISTINCT api_version FROM {project_usage_raw} WHERE tid = 0");
  while ($row = db_fetch_object($query)) {
    $tid = array_search($row->api_version, $terms);
    db_query("UPDATE {project_usage_raw} SET tid = %d WHERE api_version = '%s'", $tid, $row->api_version);
    $num_updates += db_affected_rows();
  }
  $time_1 = time();
  $substitutions = array(
    '!rows' => format_plural($num_updates, '1 row', '@count rows'),
    '!delta' => format_interval($time_1 - $time_0),
  );
  watchdog('project_usage', 'Assigned API version term IDs for !rows (!delta).', $substitutions);

  // Asign project and release node IDs.
  $num_updates = 0;
  $query = db_query("SELECT DISTINCT project_uri, project_version FROM {project_usage_raw} WHERE pid = 0 OR nid = 0");
  while ($row = db_fetch_object($query)) {
    $pid = db_result(db_query("SELECT pp.nid AS pid FROM {project_projects} pp WHERE pp.uri = '%s'", $row->project_uri));
    if ($pid) {
      $nid = db_result(db_query("SELECT prn.nid FROM {project_release_nodes} prn WHERE prn.pid = %d AND prn.version = '%s'", $pid, $row->project_version));
      db_query("UPDATE {project_usage_raw} SET pid = %d, nid = %d WHERE project_uri = '%s' AND project_version = '%s'", $pid, $nid, $row->project_uri, $row->project_version);
      $num_updates += db_affected_rows();
    }
  }
  $time_2 = time();
  $substitutions = array(
    '!rows' => format_plural($num_updates, '1 row', '@count rows'),
    '!delta' => format_interval($time_2 - $time_1),
  );
  watchdog('project_usage', 'Assigned project and release node IDs to !rows (!delta).', $substitutions);

  // Move usage records with project node IDs into the daily table and remove
  // the rest.
  db_query("INSERT INTO {project_usage_day} (timestamp, site_key, pid, nid, tid, ip_addr) SELECT timestamp, site_key, pid, nid, tid, ip_addr FROM {project_usage_raw} WHERE timestamp < %d AND pid <> 0", $timestamp);
  $num_new_day_rows = db_affected_rows();
  db_query("DELETE FROM {project_usage_raw} WHERE timestamp < %d", $timestamp);
  $num_deleted_raw_rows = db_affected_rows();
  $time_3 = time();
  $substitutions = array(
    '!day_rows' => format_plural($num_new_day_rows, '1 row', '@count rows'),
    '!raw_rows' => format_plural($num_deleted_raw_rows, '1 row', '@count rows'),
    '!delta' => format_interval($time_3 - $time_2),
  );
  watchdog('project_usage', 'Moved usage from raw to daily: !day_rows added to {project_usage_day}, !raw_rows deleted from {project_usage_raw} (!delta).', $substitutions);

  // Remove old daily records.
  $seconds = variable_get('project_usage_life_daily', 4 * PROJECT_USAGE_WEEK);
  db_query("DELETE FROM {project_usage_day} WHERE timestamp < %d", time() - $seconds);
  $time_4 = time();
  $substitutions = array(
    '!rows' => format_plural(db_affected_rows(), '1 old daily row', '@count old daily rows'),
    '!delta' => format_interval($time_4 - $time_3),
  );
  watchdog('project_usage', 'Removed !rows (!delta).', $substitutions);

  watchdog('project_usage', 'Completed daily usage data processing (total time: !delta).', array('!delta' => format_interval($time_4 - $time_0)));
}

/**
 * Compute the weekly summaries for the week starting at the given timestamp.
 *
 * @param $timestamp
 *   UNIX timestamp indicating the last time weekly stats were processed.
 */
function project_usage_process_weekly($timestamp) {
  watchdog('project_usage', 'Starting to process weekly usage data.');
  $time_0 = time();

  // Get all the weeks since we last ran.
  $weeks = project_usage_get_weeks_since($timestamp);
  // Skip the last entry since it's the current, incomplete week.
  $count = count($weeks) - 1;
  for ($i = 0; $i < $count; $i++) {
    $start = $weeks[$i];
    $end = $weeks[$i + 1];
    $date = format_date($start, 'custom', 'Y-m-d');
    $time_1 = time();

    // Try to compute the usage tallies per project and per release. If there
    // is a problem--perhaps some rows existed from a previous, incomplete
    // run that are preventing inserts, throw a watchdog error.

    $sql = "INSERT INTO {project_usage_week_project} (nid, timestamp, tid, count) SELECT pid, %d, tid, COUNT(DISTINCT site_key) FROM {project_usage_day} WHERE timestamp >= %d AND timestamp < %d AND pid <> 0 GROUP BY pid, tid";
    $query_args = array($start, $start, $end);
    $result = db_query($sql, $query_args);
    $time_2 = time();
    _db_query_callback($query_args, TRUE);
    $substitutions = array(
      '!date' => $date,
      '%query' => preg_replace_callback(DB_QUERY_REGEXP, '_db_query_callback', $sql),
      '!projects' => format_plural(db_affected_rows(), '1 project', '@count projects'),
      '!delta' => format_interval($time_2 - $time_1),
    );
    if (!$result) {
      watchdog('project_usage', 'Query failed inserting weekly project tallies for !date, query: %query (!delta).', $substitutions, WATCHDOG_ERROR);
    }
    else {
      watchdog('project_usage', 'Computed weekly project tallies for !date for !projects (!delta).', $substitutions);
    }

    $sql = "INSERT INTO {project_usage_week_release} (nid, timestamp, count) SELECT nid, %d, COUNT(DISTINCT site_key) FROM {project_usage_day} WHERE timestamp >= %d AND timestamp < %d AND nid <> 0 GROUP BY nid";
    $query_args = array($start, $start, $end);
    $result = db_query($sql, $query_args);
    $time_3 = time();
    _db_query_callback($query_args, TRUE);
    $substitutions = array(
      '!date' => $date,
      '!releases' => format_plural(db_affected_rows(), '1 release', '@count releases'),
      '%query' => preg_replace_callback(DB_QUERY_REGEXP, '_db_query_callback', $sql),
      '!delta' => format_interval($time_3 - $time_2),
    );
    if (!$result) {
      watchdog('project_usage', 'Query failed inserting weekly release tallies for !date, query: %query (!delta).', $substitutions, WATCHDOG_ERROR);
    }
    else {
      watchdog('project_usage', 'Computed weekly release tallies for !date for !releases (!delta).', $substitutions);
    }
  }

  // Remove any tallies that have aged out.
  $time_4 = time();
  $project_life = variable_get('project_usage_life_weekly_project', PROJECT_USAGE_YEAR);
  db_query("DELETE FROM {project_usage_week_project} WHERE timestamp < %d", $now - $project_life);
  $time_5 = time();
  $substitutions = array(
    '!rows' => format_plural(db_affected_rows(), '1 old weekly project row', '@count old weekly project rows'),
    '!delta' => format_interval($time_5 - $time_4),
  );
  watchdog('project_usage', 'Removed !rows (!delta).', $substitutions);

  $release_life = variable_get('project_usage_life_weekly_release', 26 * PROJECT_USAGE_WEEK);
  db_query("DELETE FROM {project_usage_week_release} WHERE timestamp < %d", $now - $release_life);
  $time_6 = time();
  $substitutions = array(
    '!rows' => format_plural(db_affected_rows(), '1 old weekly release row', '@count old weekly release rows'),
    '!delta' => format_interval($time_6 - $time_5),
  );
  watchdog('project_usage', 'Removed !rows (!delta).', $substitutions);

  watchdog('project_usage', 'Completed weekly usage data processing (total time: !delta).', array('!delta' => format_interval($time_6 - $time_0)));
}

