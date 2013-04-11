<?php
require_once 'lib/task-management.inc.php';

function doSVNRepositoriesCreations() {
	$svn_creations_query = sprintf(
		'SELECT * FROM %s WHERE action = \'svn_creation\' AND state = \'todo\' ORDER BY task_id ASC',
		REPOSITORIES_MANAGEMENT_TABLE
	);
	$svn_creations = mysql_query($svn_creations_query);
	if (!$svn_creations) {
		die(sprintf('Unable to fetch repositories to create.'));
	}
	while ($svn_creation_request = mysql_fetch_assoc($svn_creations)) {
		doSVNRepositoryCreation($svn_creation_request);
	}
}

function doSVNRepositoryCreation($svn_creation_request) {
	$repos_path = sprintf('%s/%s', SVN_ROOT_DIR, $svn_creation_request['repository']);
	$repos_url  = sprintf('file://%s', $repos_path);

	// create a basic SVN repository
	$repos_creation = execute_command(
		sprintf('svnadmin create %s', escapeshellarg($repos_path))
	);
	if ($repos_creation['code'] !== 0) {
		taskFailed(REPOSITORIES_MANAGEMENT_TABLE, $svn_creation_request['task_id'], serialize($repos_creation));
		return;
	}

	//Copy the hook post-commit
    copy('conf/post-commit.tmpl', $repos_path . '/hooks/post-commit');
    chmod(sprintf('%s/hooks/post-commit', $repos_path), fileperms(sprintf('%s/hooks/post-commit', $repos_path)) | 0110);

	// Initialize the created repository using a skeleton (typically: the usual
	// tags-trunk-branches structure)
	if (defined('SVN_SKELETON')) {
		$repos_initialization = execute_command(
			sprintf(
				'svn import %s %s --username %s --message %s',
				escapeshellarg(SVN_SKELETON),
				escapeshellarg($repos_url),
				escapeshellarg(SVN_SKELETON_COMMIT_AUTHOR),
				escapeshellarg(SVN_SKELETON_COMMIT_MESSAGE)
			)
		);
		if ($repos_initialization['code'] !== 0) {
			taskFailed(REPOSITORIES_MANAGEMENT_TABLE, $svn_creation_request['task_id'], serialize($repos_initialization));
			return;
		}
	}

	// log the work is done
	taskSucceeded(REPOSITORIES_MANAGEMENT_TABLE, $svn_creation_request['task_id']);
}
