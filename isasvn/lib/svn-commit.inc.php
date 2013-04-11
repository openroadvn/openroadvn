<?php

/*
 * Insert the message commit in drupal database
 * @param repos_path path to the repository (eg : /data/isasvn/oss)
*/
function pathToShortname($repos_path) {
	return substr($repos_path, strrpos($repos_path, '/') + 1);
}

/*
 * Insert the message commit in drupal database
 * @param username : username drupal
 * @param shortname : shortname of the repository (e.g.: oss)
 * @param message : commit message (eg : [1234] My commit message)
 * @param revision : the number of the revision
 * $date : the date of the commit (format : 2011-06-30 14:44:23 +0200 (jeu 30 jun 2011) )
 */
function insertCommit($username, $shortname, $message, $revision, $date) {
	$date = strtotime(substr($date,0,strpos($date, '(') - 1));
	$svn_commit_query = sprintf(
		"INSERT INTO %s (username, project_shortname, message_commit, revision, date_commit ) VALUES ('%s', '%s', '%s', %d, %d);",
		COMMIT_MANAGEMENT_TABLE, mysql_real_escape_string($username), mysql_real_escape_string($shortname), mysql_real_escape_string($message), $revision, mysql_real_escape_string($date));
	$svn_commit = mysql_query($svn_commit_query);
}

/*
 * Checks whether we should send a mail after a SVN commit
 * @param shortname shortname of the repository (e.g.: oss)
 * @return the address a mail shall be send to, or an empty string if none could be found
 */
function commitMailAddress($shortname) {
	// we look in the database whether a -commit(s) mailing list exists
	$commit_ml_query = sprintf('SELECT name FROM %s WHERE name REGEXP \'^%s-commits?\' LIMIT 1', EXISTING_ML_TABLE, mysql_real_escape_string($shortname));
	$commit_ml_res = mysql_query($commit_ml_query);
	if (!$commit_ml_res || !mysql_num_rows($commit_ml_res)) {
		return '';
	}
	$ml_name = mysql_fetch_object($commit_ml_res)->name;
	return $ml_name . '@' . ML_DOMAIN;
}
