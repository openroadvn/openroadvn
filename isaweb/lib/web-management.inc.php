<?php
/**
	Regenerate the rewrite map for web directories
*/
function regenerate_map() {
	$web_map_filepath = constant('WEB_MAP_FILEPATH');
	$suffixed_web_map_filepath = constant('WEB_MAP_FILEPATH') . 'tmp';

	// generate the entire map
	$map_file_handle = fopen($suffixed_web_map_filepath, 'w');
	if (!$map_file_handle) {
         die(sprintf('Unable to open "%s" for writing' . "\n", $suffixed_web_map_filepath));
	}

	$all_web_directories_query = sprintf(
		'SELECT name, public_access FROM %s ORDER BY name ASC',
		constant('WEB_DIRECTORIES_TABLE')
	);
	$all_web_directories_res = mysql_query($all_web_directories_query);
	if (!$all_web_directories_res) {
		die('Unable to fetch web directories in order to regenerate the map' . "\n");
	}
	while ($web_directory = mysql_fetch_assoc($all_web_directories_res)) {
		$written = fwrite(
			$map_file_handle,
			sprintf("%s    %s\n", $web_directory['name'], $web_directory['public_access'])
		);
		if (!$written) { // both FALSE and 0 are suspect
			fclose($map_file_handle);
			die(sprintf('A write error occured when generating "%s", aborting.' . "\n", $suffixed_web_map_filepath));
		}
	}
	fclose($map_file_handle);

	// ok, we have generated the new map, we now must replace the former one
	$renaming = rename($suffixed_web_map_filepath, $web_map_filepath);
	if (!$renaming) { // both FALSE and 0 are suspect
		die(sprintf('Unable to rename "%s" to "%s", aborting.' . "\n", $suffixed_web_map_filepath, $web_map_filepath));
	}
	if (strlen(constant('WEB_SYNC_MAP_SCRIPT'))) {
		system(constant('WEB_SYNC_MAP_SCRIPT'));
	}
}

/**
	Create a web directory if needed
	@param $web_directory_task Record of the task to create/update the webdirectory
*/
function create_web_directory($web_directory_task) {
	// ensure the root directory exists
	if (!is_dir(constant('WEB_ROOT_DIR'))) {
		if (!mkdir(constant('WEB_ROOT_DIR'))) {
			taskFailed(
				constant('WEB_DIRECTORIES_TABLE'),
				$web_directory_task['web_id'],
				sprintf('Unable to create directory "%s".',	constant('WEB_ROOT_DIR'))
			);
			return FALSE;
		}
	}

	$web_directory = constant('WEB_ROOT_DIR') . '/' . $web_directory_task['name'];

	// check whether the work is already done
	if (is_dir($web_directory)) {
		taskSucceeded(
			constant('WEB_DIRECTORIES_TABLE'),
			$web_directory_task['web_id']
		);
		return TRUE;
	}

	// create the web directory itself only if needed
	if (!mkdir($web_directory)) {
		taskFailed(
			constant('WEB_DIRECTORIES_TABLE'),
			$web_directory_task['web_id'],
			sprintf('Unable to create directory "%s".', $web_directory)
		);
		return FALSE;
	}

	if (is_dir(constant('WEB_SKELETON'))) {
		$execution = execute_command(
			sprintf(
				'cp -a %s/* %s/',
				escapeshellarg(constant('WEB_SKELETON')),
				escapeshellarg($web_directory)
			)
		);

		if ($execution['code'] !== 0) {
			taskFailed(
				constant('WEB_DIRECTORIES_TABLE'),
				$web_directory_task['web_id'],
				serialize($execution)
			);
			return FALSE;
		}
	}
	taskSucceeded(constant('WEB_DIRECTORIES_TABLE'),  $web_directory_task['web_id']);
	return TRUE;
}

/**
	@param $directory name of a web directory
	@return the size of the specified web directory, or:
	* -1 if it does not exist on the filesystem
	* -2 if the `du' command failed
	* -3 if its output is not recognized
*/
function web_directory_size($directory) {
	$abs_dir_path = constant('WEB_ROOT_DIR') . '/' . $directory;
	if (!is_dir($abs_dir_path)) {
		return -1;
	}
	$execution = execute_command(sprintf('nice -n +19 du -bs %s', escapeshellarg($abs_dir_path)));
	if ($execution['code'] !== 0) {
		return -2;
	}
	if (preg_match('/^(\d+)/', $execution['output'], $matches)) {
		return $matches[1];
	}
	return -3;
}

/**
	@param $path A path supposed to exist (this function does not check it).
	@return a string describing the current usage of the filesystem hosting \a
	$path, or -1 if something went wrong.
*/
function filesystem_usage($path) {
	$execution = execute_command(sprintf('df -hP %s', escapeshellarg($path)));
	if ($execution['code'] !== 0) {
		return -1;
	}
	return $execution['output'];
}

/**
	@param $bytes a filesize in bytes
	@return a human-readable string representing the provided filesize
*/
function format_file_size($bytes) {
  $units = array('B', 'KB', 'MB', 'GB', 'TB', 'PB');
  $factor = 0;
  for ($b = $bytes ; $b >= 1024 ; $b /= 1024) ++ $factor;
  return sprintf('%.2f%s', $bytes / pow(1024, $factor), @$units[$factor]);
}
