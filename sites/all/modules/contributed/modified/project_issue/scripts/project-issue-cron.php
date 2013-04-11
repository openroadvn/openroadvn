#!/usr/bin/php
<?php
// $Id: project-issue-cron.php,v 1.3 2009/01/20 18:38:04 dww Exp $


/**
 * @file
 * Script to invoke project_issue periodic code outside of hook_cron().
 *
 * On some sites, hook_cron() becomes very busy and you can frequently get
 * timeouts where all of the cron-tasks can't be completed during the time
 * limit for web-based PHP requests. To aleviate this problem, you can move
 * some of the code invoked by hook_cron() into separate CLI scripts which you
 * invoke directly by adding additional entries to the crontab on your system.
 * If you do this, you must also set the "project_issue_hook_cron" variable
 * to FALSE in your site's settings.php file.
 *
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

// Real work begins.
if (module_exists('project_issue') && variable_get('project_issue_hook_cron', TRUE) == FALSE) {
  module_load_include('inc', 'project_issue', 'includes/cron');
  _project_issue_cron();
}

