<?php

// $Id: project-release-serve-history.php,v 1.14 2010/01/16 19:59:13 dww Exp $

/**
 * @file
 * Ultra-thin PHP wrapper to serve XML release history files to
 * the update.module ("update_status.module" in 5.x contrib).
 *
 * This script requires a local .htaccess file with the following:
 *
 * DirectoryIndex project-release-serve-history.php
 * <IfModule mod_rewrite.c>
 *   RewriteEngine on
 *   RewriteRule ^(.*)$ project-release-serve-history.php?q=$1 [L,QSA]
 * </IfModule>
 *
 * @author Derek Wright (http://drupal.org/user/46549)
 */

/**
 * Required configuration: directory tree for the XML history files.
 */
define('HISTORY_ROOT', '');

/**
 * Required configuration: location of your Drupal installation for
 * bootstrapping and recording usage statistics.
 */
define('DRUPAL_ROOT', '');

/**
 * Required configuration: name of your site.
 *
 * Needed to find the right settings.php file to bootstrap Drupal with.
 */
define('SITE_NAME', '');


/**
 * Find and serve the proper history file.
 */

// Set page headers for the XML response.
header('Content-Type: text/xml; charset=utf-8');

// Make sure we have the path arguments we need.
$path = $_GET['q'];
$args = explode('/', $path);
if (empty($args[0])) {
  error('You must specify a project name to display the release history of.');
}
else {
  $project_name = $args[0];
}
if (empty($args[1])) {
  error('You must specify an API compatibility version as the final argument to the path.');
}
else {
  $api_version = $args[1];
}

// Sanitize the user-supplied input for use in filenames.
$whitelist_regexp = '@[^a-zA-Z0-9_.-]@';
$safe_project_name = preg_replace($whitelist_regexp, '#', $project_name);
$safe_api_vers = preg_replace($whitelist_regexp, '#', $api_version);

// Figure out the filename for the release history we want to serve.
$project_dir = HISTORY_ROOT .'/'. $safe_project_name;
$filename = $safe_project_name .'-'. $safe_api_vers .'.xml';
$full_path = $project_dir .'/'. $filename;

if (!is_file($full_path)) {
  if (!is_dir($project_dir)) {
    error(strtr('No release history was found for the requested project (@project).', array('@project' => _check_plain($project_name))));
  }
  error(strtr('No release history available for @project @version.', array('@project' => _check_plain($project_name), '@version' => _check_plain($api_version))));
  exit(1);
}

// Set the Last-Modified to the timestamp of the file.  Otherwise, disable all
// caching since a) we continue to have problems with squid on d.o and b)
// we're going to need this as soon as we start collecting stats.
$stat = stat($full_path);
$mtime = $stat[9];
header('Last-Modified: '. gmdate('D, d M Y H:i:s', $mtime) .' GMT');
header("Expires: Sun, 19 Nov 1978 05:00:00 GMT");
header("Cache-Control: store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", FALSE);

// Serve the contents.
$file = file_get_contents($full_path);
// Old release xml files are missing the encoding. Prepend one if necessary.
if (substr($file, 0, 5) != '<?xml') {
  echo '<?xml version="1.0" encoding="utf-8"?>' ."\n";
}
echo $file;

// Record usage statistics.
if (isset($_GET['site_key'])) {
  if (!chdir(DRUPAL_ROOT)) {
    exit(1);
  }

  // Setup variables for Drupal bootstrap
  $script_name = $argv[0];
  $_SERVER['HTTP_HOST'] = SITE_NAME;
  $_SERVER['REQUEST_URI'] = '/' . $script_name;
  $_SERVER['SCRIPT_NAME'] = '/' . $script_name;
  $_SERVER['PHP_SELF'] = '/' . $script_name;
  $_SERVER['SCRIPT_FILENAME'] = $_SERVER['PWD'] .'/'. $script_name;
  $_SERVER['PATH_TRANSLATED'] = $_SERVER['SCRIPT_FILENAME'];

  // Actually do the bootstrap.
  include_once './includes/bootstrap.inc';
  drupal_bootstrap(DRUPAL_BOOTSTRAP_DATABASE);

  // We can't call module_exists without bootstrapping to a higher level so
  // we'll settle for checking that the table exists.
  if (db_table_exists('project_usage_raw')) {
    $site_key = $_GET['site_key'];
    $project_version = isset($_GET['version']) ? $_GET['version'] : '';
    $ip_addr = ip_address();

    // Compute a GMT timestamp for begining of the day. getdate() is
    // affected by the server's timezone so we need to cancel it out.
    $now = time();
    $time_parts = getdate($now - date('Z', $now));
    $timestamp = gmmktime(0, 0, 0, $time_parts['mon'], $time_parts['mday'], $time_parts['year']);

    db_query("UPDATE {project_usage_raw} SET api_version = '%s', project_version = '%s', ip_addr = '%s' WHERE project_uri = '%s' AND timestamp = %d AND site_key = '%s'", $api_version, $project_version, $ip_addr, $project_name, $timestamp, $site_key);
    if (!db_affected_rows()) {
      db_query("INSERT INTO {project_usage_raw} (project_uri, timestamp, site_key, api_version, project_version, ip_addr) VALUES ('%s', %d, '%s', '%s', '%s', '%s')", $project_name, $timestamp, $site_key, $api_version, $project_version, $ip_addr);
    }
  }
}


/**
 * Copy of core's check_plain() function.
 */
function _check_plain($text) {
  return htmlspecialchars($text, ENT_QUOTES);
}

/**
 * Generate an error and exit.
 */
function error($text) {
  echo '<?xml version="1.0" encoding="utf-8"?>'. "\n";
  echo '<error>'. $text ."</error>\n";
  exit(1);
}
