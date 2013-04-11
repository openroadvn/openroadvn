<?php

function get_host_name() {
	if (function_exists('gethostname')) {
		return(gethostname());
	}
	return(trim(`hostname -s`));
}

function get_lock_filepath($lock_name) {
	return sprintf('%s/%s.lock', LOCK_DIR, $lock_name);
}

function check_lock($lock_name, $acquire = TRUE) {
	$lock_file = get_lock_filepath($lock_name);
	if (file_exists($lock_file)) {
		die(sprintf('Lock file %s exists, aborting.'."\n", $lock_file));
	}
	if ($acquire) {
		acquire_lock($lock_name);
	}
}

function acquire_lock($lock_name) {
	$lock_file = get_lock_filepath($lock_name);
	$lock_handle = fopen($lock_file, 'w');
	if (!$lock_handle) {
		die(sprintf('Unable to acquire lock file %s, aborting.'."\n", $lock_file));
	} else {
		fclose($lock_handle);
	}
}

function release_lock($lock_name) {
	$lock_file = get_lock_filepath($lock_name);
	@unlink($lock_file);
}

function execute_command($command, $command_redirections = '< /dev/null 2>&1') {
	$return = array('command' => $command, 'code' => -1, 'output' => '');
	$proc_res = popen($command . ' ' . $command_redirections, 'r');
	if ($proc_res === FALSE) return $return;
	$return['output'] = stream_get_contents($proc_res);
	$return['code'] = pclose($proc_res);
	return $return;
}

function execute_command_with_stdin($stdin_content, $command, $command_redirections = '2>&1') {
	$return = array('command' => $command, 'code' => -1, 'output' => '');
	$proc_res = proc_open(
		$command . ' ' . $command_redirections,
		array(
			0 => array('pipe', 'r'),
			1 => array('pipe', 'w'),
		),
		$pipes
	);
	if ($proc_res === FALSE) return $return;

	// we send our content to the process stdin
	fwrite($pipes[0], $stdin_content);
	fclose($pipes[0]);

	// we read its stdout (+stderr since we use 2>&1 by default)
	$return['output'] = stream_get_contents($pipes[1]);
	
	// eventually, we get its return code:
	$return['code'] = proc_close($proc_res);
	return $return;
}

function taskSucceeded($table, $task_id, $result_data = '') {
	updateTask($table, $task_id, 'done', $result_data);
}

function taskFailed($table, $task_id, $result_data = '') {
	updateTask($table, $task_id, 'failed', $result_data);
}

function updateTask($table, $task_id, $state, $result_data) {
	$query = sprintf(
		'UPDATE %s SET state = \'%s\', %s = \'%s\', action_time = CURRENT_TIMESTAMP() WHERE %s = %d',
		$table,
		mysql_real_escape_string($state),
		(defined('TASK_DATA_FIELD') ? constant('TASK_DATA_FIELD') : 'result_data'),
		mysql_real_escape_string($result_data),
		(defined('TASK_ID_FIELD') ? constant('TASK_ID_FIELD') : 'task_id'),
		$task_id
	);
	$update_request = mysql_query($query);
	if (!$update_request) {
		die(
			sprintf(
				'An error occured while trying to issue the following query:'."\n".'%s'."\n".'The error is: %s',
				$query,
				mysql_error()
			)
		);
	}
}
