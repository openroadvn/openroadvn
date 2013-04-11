<?php

/**
 * @file
 * Advanced aggregation module; 404 handler.
 *
 */

/**
 * Menu Callback; regenerates a missing css file.
 */
function advagg_missing_css() {
  ignore_user_abort();

  // Try to regenerate missing file
  $msg = advagg_missing_regenerate();

  // If here send out fast 404.
  advagg_missing_fast404($msg);
}

/**
 * Menu Callback; regenerates a missing js file.
 */
function advagg_missing_js() {
  ignore_user_abort();

  // Try to regenerate missing file
  $msg = advagg_missing_regenerate();

  // If here send out fast 404.
  advagg_missing_fast404($msg);
}

/**
 * regenerates a missing css file.
 *
 * @param $filename
 *   filename
 * @param $type
 *   css or js
 * @return
 *   false if bundle couldn't be generated.
 */
function advagg_missing_regenerate() {
  global $base_path, $conf;

  // Get filename from request.
  $arg = arg();
  $filename = array_pop($arg);
  $filename = array_shift(explode('?', $filename));

  $data = advagg_get_bundle_from_filename($filename);
  if (is_array($data)) {
    list($type, $md5, $counter) = $data;
  }
  else {
    return $data;
  }

  $_GET['redirect_counter'] = isset($_GET['redirect_counter']) ? intval($_GET['redirect_counter']) : 0;
  if ($_GET['redirect_counter'] > 5) {
    watchdog('advagg', 'This request could not generate correctly. Loop detected. Request data: %info', array('%info' => $_GET['q']));
    return t('In a Loop.');
  }

  // Set file(s) in cache to FALSE.
  $arg[] = $filename;
  cache_set(implode('/', $arg), FALSE, 'cache_advagg', TRUE);
  advagg_missing_remove_cache($md5);

  // Counter in database.
  $counter_in_db = db_result(db_query("SELECT counter FROM {advagg_bundles} WHERE bundle_md5 = '%s'", $md5));
  if ($counter_in_db === FALSE) {
    return t('Not a valid bundle.');
  }

  // Only process if we got an older counter.
  // If we have an out of range counter see if a simlar file exists and serve
  // that up.
  if ($counter > $counter_in_db || $counter < 0) {
    list($css_path, $js_path) = advagg_get_root_files_dir();
    if ($type == 'js') {
      $file_type_path = $js_path;
    }
    if ($type == 'css') {
      $file_type_path = $css_path;
    }

    $new_filename = advagg_build_filename($type, $md5, $counter_in_db);
    $filepath = $file_type_path .'/'. $new_filename;
    header('Location: ' . advagg_build_uri($filepath), TRUE, 302);
    exit;
  }

  // Break connection and do generation in the background.
  if (!empty($_GET['generator'])) {
    advagg_missing_async_opp($md5 . ' ' . $counter);
  }

  // Rebuild file.
  $conf['advagg_async_generation'] = FALSE;
  $good = advagg_rebuild_bundle($md5, $counter, TRUE);
  if (!$good) {
    watchdog('advagg', 'This request could not generate correctly. Aggregate not generated. Request data: %info', array('%info' => $_GET['q']));
    return t('Rebuild Failed.');
  }

  // Redirect to file.
  $_GET['redirect_counter']++;
  $uri = $base_path . $_GET['q'] . '?redirect_counter=' . $_GET['redirect_counter'];
  if (!headers_sent()) {
    header('Location: ' . $uri, TRUE, 307);
    usleep(250000); // Sleep for 250ms
  }
  exit;
}

/**
 * Set cache value to FALSE.
 *
 * @param $bundle_md5
 *   Bundle's machine name.
 */
function advagg_missing_remove_cache($bundle_md5) {
  $files = array();
  $results = db_query("SELECT filename, filetype FROM {advagg_files} AS af INNER JOIN {advagg_bundles} AS ab USING ( filename_md5 ) WHERE bundle_md5 = '%s' ORDER BY porder ASC", $bundle_md5);
  while ($row = db_fetch_array($results)) {
    $files[] = $row['filename'];
    $type = $row['filetype'];
  }

  list($css_path, $js_path) = advagg_get_root_files_dir();
  if ($type == 'js') {
    $file_type_path = $js_path;
  }
  if ($type == 'css') {
    $file_type_path = $css_path;
  }

  $filenames = advagg_get_filename($files, $type, '', $bundle_md5);
  foreach ($filenames as $key => $info) {
    $filename = $info['filename'];
    $filepath = $file_type_path .'/'. $filename;

    cache_set($filepath, FALSE, 'cache_advagg', TRUE);
  }
}

/**
 * Output text & set php in async mode.
 *
 * @param $output
 *  string - Text to output to open connection.
 * @param $wait
 *  bool - Wait 1 second?
 * @param $content_type
 *  string - Content type header.
 * @param $length
 *  int - Content length.
 */
function advagg_missing_async_opp($output, $wait = TRUE, $content_type = "text/html; charset=utf-8", $length = 0) {
  if (headers_sent()) {
    return FALSE;
  }

  // Calculate Content Length
  if ($length == 0) {
    $output .= "\n";
    $length = (advagg_missing_strlen($output)-1);
  }
  // Prime php for background operations
  $loop = 0;
  while (ob_get_level() && $loop < 25) {
    ob_end_clean();
    $loop++;
  }
  header("Connection: close");
  ignore_user_abort();

  // Output headers & data
  ob_start();
  header("Content-type: " . $content_type);
  header("Expires: Sun, 19 Nov 1978 05:00:00 GMT");
  header("Cache-Control: no-cache");
  header("Cache-Control: must-revalidate");
  header("Content-Length: " . $length);
  header("Connection: close");
  print($output);
  ob_end_flush();
  flush();
  // wait for 1 second
  if ($wait) {
    sleep(1);
  }

  // text returned and connection closed.
  // Do background processing. Time taken after should not effect page load times.
  return TRUE;
}

/**
 * Get the length of a string in bytes
 *
 * @param $string
 *   get string length
 */
function advagg_missing_strlen($string) {
  if (function_exists('mb_strlen')) {
    return mb_strlen($string, '8bit');
  }
  else {
    return strlen($string);
  }
}
