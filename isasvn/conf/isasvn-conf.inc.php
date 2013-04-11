<?php
/*
	Common configuration for ISA/SVN scripts.
*/

// adjust include path - we expect the "isacommon" directory to be a sibling of isaweb
set_include_path(get_include_path() . PATH_SEPARATOR . dirname(__FILE__) . '/../../isacommon');

// The local server is a MySQL database located on the current machine
// It does not contain anything but a cache table used to check whether a given
// user/password couple is valid or not.
define('LOCAL_SERVER',      '127.0.0.1');
define('LOCAL_DB_USERNAME', 'svnservice');
define('LOCAL_DB_PASSWORD', 'pwd');
define('LOCAL_DB_NAME',     'isasvn');

// The ISA server is the MySQL database of the ISA Drupal application.
define('ISA_SERVER',        '127.0.0.1');
define('ISA_DB_USERNAME',   'root');
define('ISA_DB_PASSWORD',   'pwd');
define('ISA_DB_NAME',       'openray');

// This directory will host lock files
define('LOCK_DIR', '/tmp');

// Name of the table storing repositories operations
// Note this value must include the Drupal table prefix, if any
define('REPOSITORIES_MANAGEMENT_TABLE', 'repositories_management');
// Name of the table storing commit operations
// Note this value must include the Drupal table prefix, if any
define('COMMIT_MANAGEMENT_TABLE', 'commit_management');
// Name of the table storing existing mailing lists
// Note this value must include the Drupal table prefix, if any
define('EXISTING_ML_TABLE', 'mailman_lists');
// Domain appended after the mailing list name
define('ML_DOMAIN', 'openray.org');

// command to run in order to send a mail describing a commit
// available placeholders: @username, @repos_path, @repos_shortname,
// @ message, @revision, @date
define('MAIL_COMMAND', dirname(__FILE__) . '/../bin/commit-email.pl @repos_path @revision --from svn-commits@openray.org -r @commit_address @commit_address');

// Parent directory for all Subversion repositories
// Note the directory must exist and be writable.
define('SVN_ROOT_DIR', '/data/openray-SVN');
// Skeleton directory when creating a Subversion repository
// Do not define this constant if you do not want repositories to be initialized this way
define('SVN_SKELETON', dirname(__FILE__) . '/data/svn-skeleton');
define('SVN_SKELETON_COMMIT_MESSAGE', 'Repository initialization');
define('SVN_SKELETON_COMMIT_AUTHOR',  'ISA Subversion service');

// This prefix will be stripped from the received URI when authenticating a user.
// the service is given through the AuthExternalContext Apache directive
$service = ($_ENV['CONTEXT'] == 'subversion') ? 'svn' : 'webdav';
//Allows to use different permissions
define('AUTH_CONTEXT', $service);

if (preg_match("/^(\/(?:isa|joinup)-$service)/", getenv('URI'), $matches)) {
	$prefix = $matches[1];
	define('SVN_URI_PREFIX', $prefix);
}
