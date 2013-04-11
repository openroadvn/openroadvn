<?php

// $Id: project-release-private-download.php,v 1.3 2009/08/07 05:28:23 dww Exp $

/**
 * @file
 * Script to serve private file downloads for project releases.
 *
 * MOTIVATION:
 *
 * This script is intended for sites that use public file access by default
 * (which is much better for regular performance) but wish to restrict access
 * to file downloads for their release nodes, and which generates the files
 * associated with release nodes (see package-release-nodes.php) instead of
 * allowing users to upload files and attach them directly.
 *
 * INSTRUCTIONS:
 *
 * 1. (Optional) See if your web server supports the 'X-Sendfile' header.  In
 * Apache, you need mod_xsendfile (http://tn123.ath.cx/mod_xsendfile).  If
 * your server supports this header, set the 'USE_XSENDFILE' setting below to
 * TRUE and the file downloads will be significantly faster and use less RAM.
 *
 * 2. Configure package-release-nodes.php to generate files into a directory
 * outside of your site's webroot (via the "$dest_root" variable).  For
 * example:
 *
 * $dest_root = '/home/package-system';
 * $dest_rel = 'releases';
 *
 * That will generate files like: /home/package-system/releases/foo-1.0.tar.gz
 *
 * 3. Set the "Downlod link base URL" setting (which can be found at
 * http://example.com/admin/project/project-release-settings) to point to an
 * unused location (for example "download/").  That will prevent links
 * pointing into the public "files" directory, and will cause the URLs to
 * download the files attached to release nodes to look something like:
 * http://example.com/download/releases/foo-1.0.tar.gz
 *
 * 4. Create a directory in your web root with the same name specifed above
 * (e.g. "download") and place a copy of this script in that directory.  In
 * the same directory, you should put a .htaccess file with the following:
 *
 * # Begin .htaccess
 * DirectoryIndex project-release-private-download.php
 * <IfModule mod_rewrite.c>
 *   RewriteEngine on
 *   RewriteRule ^(.*)$ project-release-private-download.php?q=$1 [L,QSA]
 * </IfModule>
 * <IfModule mod_xsendfile.c>
 *   XSendFile on
 *   XSendFileAllowAbove on
 * </IfModule>
 * # End .htaccess
 * 
 * 5. Configure the FILE_ROOT, DRUPAL_ROOT and SITE_NAME constants below.
 * - FILE_ROOT should point to whatever you set $dest_root in step 1.
 * - DRUPAL_ROOT should point to the web root for your site.
 * - SITE_NAME should match the name of your site (e.g. the 'xxx' part of
 *   where your 'sites/xxx/settings.php' file lives).
 * - Don't forget to set USE_XSENDFILE to TRUE if your server supports it.
 *
 * 6. Start creating release nodes and running package-release-nodes.php
 *
 * 7. Enjoy your private downloads!
 *
 *
 * @author Derek Wright (http://drupal.org/user/46549)
 */


/**
 * Required configuration: directory tree where the real files live.
 */
define('FILE_ROOT', '');

/**
 * Required configuration: location of your Drupal installation.
 */
define('DRUPAL_ROOT', '');

/**
 * Required configuration: name of your site.
 *
 * Needed to find the right settings.php file to bootstrap Drupal with.
 */
define('SITE_NAME', '');

/**
 * Optional configuration: does your server support the X-Sendfile header?
 */
define('USE_XSENDFILE', FALSE);


/*
 * --------------------------------------------------
 * Real work begins, nothing to configure below this. 
 * --------------------------------------------------
 */

// Need to be inside the Drupal webroot to properly bootstrap.
if (!chdir(DRUPAL_ROOT)) {
  exit(1);
}

// Setup variables for bootstrap.
$script_name = $argv[0];
$_SERVER['HTTP_HOST'] = SITE_NAME;
$_SERVER['REMOTE_ADDR'] = '127.0.0.1';
$_SERVER['REQUEST_URI'] = '/' . $script_name;
$_SERVER['SCRIPT_NAME'] = '/' . $script_name;
$_SERVER['PHP_SELF'] = '/' . $script_name;
$_SERVER['SCRIPT_FILENAME'] = $_SERVER['PWD'] .'/'. $script_name;
$_SERVER['PATH_TRANSLATED'] = $_SERVER['SCRIPT_FILENAME'];

// Actually do the bootstrap. Since we're relying on db_rewrite_sql() to
// enforce the access checks on the release node, and since that invokes a
// hook, we need a full bootstrap here, not just DRUPAL_BOOTSTRAP_DATABASE.
include_once './includes/bootstrap.inc';
drupal_bootstrap(DRUPAL_BOOTSTRAP_FULL);

// Make sure we have the path argument for the file to download.
$path = $_GET['q'];
if (empty($path)) {
  drupal_not_found();
  exit(1);
}

// Figure out the filename for the release history we want to serve, and make
// sure that file actually exists in our directory.
$full_path = FILE_ROOT . '/' . $path;
if (!is_file($full_path)) {
  drupal_not_found();
  exit(1);
}

// Find the release this file is associated with. Due to the db_rewrite_sql(),
// this will enforce node access checks for us, so a user without permission
// to view the given file will be denied. Since we're testing if there's a
// node with a file that uses the given path as its filepath, this protects us
// against prying eyes that might try to access "/../../../etc/passwd" or
// something.  Even if they managed to find a file that actually exists that
// way, it's not going to match a valid release node.  So they're going to get
// a 403, not the file they're trying to steal.
$nid = db_result(db_query(db_rewrite_sql("SELECT n.nid FROM {node} n INNER JOIN {project_release_file} prf ON n.nid = prf.nid INNER JOIN {files} f ON prf.fid = f.fid WHERE n.status = 1 AND f.filepath = '%s'"), $path));
if (empty($nid)) {
  drupal_access_denied();
  exit(1);
}

// If we found the release, serve up the file.
$stat = stat($full_path);
$file_size = $stat[7];
$file_mtime = $stat[9];
header('Expires: 0');
header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
header('Last-Modified: '. gmdate('D, d M Y H:i:s', $file_mtime) .' GMT');
header('Content-Type: application/octet-stream'); 
header('Content-Description: File Transfer');
header('Content-Disposition: attachment; filename="' . basename($path) . '"'); 
header('Content-Length: ' . $file_size);

if (USE_XSENDFILE) {
  // Faster and better.
  header('X-Sendfile: ' . $full_path);
}
else {
  // More portable.
  flush();
  readfile($full_path);
}

// If we got this far, invoke our hook to let modules know this happened.
global $user;
module_invoke_all('project_release_download', $nid, $path, $user->uid);

