<?php

/**
 * @file
 * Admin page callbacks for the advanced CSS/JS aggregation module.
 */

/**
 * Page generation function for admin/settings/advagg
 */
function advagg_admin_page() {
  $output = '';
  return $output . drupal_get_form('advagg_admin_settings_form');
}

/**
 * Page generation function for admin/settings/advagg/info
 */
function advagg_admin_info_page() {
  $output = '';
  return $output . drupal_get_form('advagg_admin_info_form');
}

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_admin_info_form() {
  $form = array();

  $form['info'] = array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Hook Theme Info'),
  );
  init_theme();
  $hooks = theme_get_registry();
  $data = implode("\n", $hooks['page']['preprocess functions']);
  $form['info']['advagg_debug_info'] = array(
    '#type'           => 'textarea',
    '#default_value'  => $data,
    '#rows'           => count($hooks['page']['preprocess functions'])+1,
  );

  $types = db_query("SELECT DISTINCT(filetype) FROM {advagg_files}");
  while ($type = db_result($types)) {
    $form[$type] = array(
      '#type'          => 'fieldset',
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
      '#title'         => t('@type files', array('@type' => drupal_strtoupper($type))),
    );
    // filename  filename_md5  checksum  filetype  counter   data
    $results = db_query("SELECT * FROM {advagg_files} WHERE filetype = '%s'", $type);
    while ($row = db_fetch_array($results)) {
      $data = advagg_get_file_data($row['filename_md5']);
      if (!empty($data)) {
        list($data, $rows) = advagg_form_print_r($data);

        $form[$type][$row['filename_md5']] = array(
          '#type'         => 'textarea',
          '#title'        => check_plain($row['filename']),
          '#default_value'  => $data,
          '#rows'           => $rows-1,
          '#description'  => t('File has changed %counter times', array('%counter' => $row['counter'])),
        );
      }
      else {
        $form[$type][$row['filename_md5']] = array(
          '#type'   => 'markup',
          '#value'  => '<div>' . format_plural($row['counter'], 'changed 1 time - %file<br />', 'changed %counter times - %file<br /></div>',  array(
            '%counter'  => $row['counter'],
            '%file'     => $row['filename'],
          )),
        );
      }
    }
  }

  // Get hooks in use.
  $hooks = array(
    'advagg_css_alter'              => array(),
    'advagg_css_pre_alter'          => array(),
    'advagg_css_extra_alter'        => array(),
    'advagg_js_alter'               => array(),
    'advagg_js_pre_alter'           => array(),
    'advagg_js_extra_alter'         => array(),
    'advagg_filenames_alter'        => array(),
    'advagg_files_table'            => array(),
    'advagg_js_header_footer_alter' => array(),
    'advagg_master_reset'           => array(),
  );
  foreach ($hooks as $hook => $values) {
    $hooks[$hook] = module_implements($hook);
  }
  // Record function overrides. Not working currently.
//   $hooks['advagg_css_render_function'] = array(variable_get('advagg_css_render_function', ADVAGG_CSS_RENDER_FUNCTION));
//   $hooks['advagg_js_render_function'] = array(variable_get('advagg_js_render_function', ADVAGG_JS_RENDER_FUNCTION));
//   $hooks['advagg_file_save_function'] = array(variable_get('advagg_file_save_function', ADVAGG_FILE_SAVE_FUNCTION));

  // Output hooks in form.
  foreach ($hooks as $hook => $values) {
    if (empty($values)) {
      $form['hooks_implemented'][$hook] = array(
      '#type'   => 'markup',
      '#value'  => '<div><strong>' . check_plain($hook) . ':</strong><br />&nbsp;&nbsp;&nbsp;&nbsp;' . t('None') . '</div>',
      );
    }
    else {
      $form['hooks_implemented'][$hook] = array(
        '#type'   => 'markup',
        '#value'  => '<div><strong>' . check_plain($hook) . ':</strong><br />&nbsp;&nbsp;&nbsp;&nbsp;' . filter_xss(implode('<br />&nbsp;&nbsp;&nbsp;&nbsp;', $values)) . '</div>',
      );
    }
  }
  $form['hooks_implemented'] += array(
    '#type'          => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'         => t('Modules implementing advagg hooks'),
  );

  // If any files are missing list them.
  // TODO: Reset missing files list.
  $results = db_query("SELECT * FROM {advagg_files} WHERE checksum = '-1' ORDER BY filetype ASC");
  while ($row = db_fetch_array($results)) {
    $form['missing'][$row['filename_md5']] = array(
      '#type'   => 'markup',
      '#value'  => '<div>' . check_plain($row['filename']) . '<br /></div>',
    );
  }
  if (!empty($form['missing'])) {
    $form['missing'] += array(
      '#type'          => 'fieldset',
      '#collapsible'   => TRUE,
      '#collapsed'     => FALSE,
      '#title'         => t('Missing files'),
    );
  }

  return $form;
}

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_admin_settings_form() {
  $form = array();
  $readme = drupal_get_path('module', 'advagg') . '/README.txt';
  $bundle_count = db_result(db_query("SELECT COUNT(*) FROM (SELECT bundle_md5 FROM {advagg_bundles} GROUP BY bundle_md5) as temp"));

  $form['advagg_enabled'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable Advanced Aggregation'),
    '#default_value'  => variable_get('advagg_enabled', ADVAGG_ENABLED),
  );
  $form['advagg_closure'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Use AdvAgg in closure'),
    '#default_value'  => variable_get('advagg_closure', ADVAGG_CLOSURE),
    '#description'    => t('If enabled javascript files in the closure region will be aggregated.'),
  );
  // Make sure the advagg_check_missing_handler function is available.
  module_load_install('advagg');
  $ret = advagg_check_missing_handler();
  $form['advagg_async_generation'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Generate CSS/JS files on request (async mode)'),
    '#default_value'  => variable_get('advagg_async_generation', ADVAGG_ASYNC_GENERATION),
    '#disabled'       => $ret['advagg_async_generation']['severity'] == REQUIREMENT_ERROR ? TRUE : FALSE,
    '#description'    => t('Current State: !value', array('!value' => filter_xss($ret['advagg_async_generation']['value'] . ' ' . (!empty($ret['advagg_async_generation']['description']) ? $ret['advagg_async_generation']['description'] : '')))),
  );
  $form['advagg_gzip_compression'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Gzip CSS/JS files'),
    '#default_value'  => variable_get('advagg_gzip_compression', ADVAGG_GZIP_COMPRESSION),
    '#description'    => t('This might break CSS/JS handling at the Apache level. If it does, use the rules for your webroot level htaccess file before re-enabling. Directions on what to change are located in the <a href="@readme">readme</a> file. In short, be sure to test this out.', array('@readme' => url($readme))),
  );
  $form['advagg_dir_htaccess'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Generate .htaccess files in the advagg_* dirs'),
    '#default_value'  => variable_get('advagg_dir_htaccess', ADVAGG_DIR_HTACCESS),
    '#description'    => t('Disable if your using the rules from the <a href="@readme">readme</a> file in your webroot level htaccess file.', array('@readme' => url($readme))),
  );
  $form['advagg_rebuild_on_flush'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Regenerate flushed bundles in the cache flush request'),
    '#default_value'  => variable_get('advagg_rebuild_on_flush', ADVAGG_REBUILD_ON_FLUSH),
    '#description'    => t('Enable if your server will not timeout on a request.'),
  );
  $public_downloads = (variable_get('file_downloads', FILE_DOWNLOADS_PUBLIC) == FILE_DOWNLOADS_PUBLIC);
  if (!$public_downloads) {
    $extra = module_exists('boost') ? t('If boost is installed, you can use the cache directory for advagg files.') : '';
    $form['advagg_custom_files_dir'] = array(
      '#type'           => 'textfield',
      '#field_prefix'   => '<div>' . t('You are using a private file system. You can serve aggregated files via a public folder.') . '</div>',
      '#title'          => t('Use a different directory for storing advagg files'),
      '#default_value'  => variable_get('advagg_custom_files_dir', ADVAGG_CUSTOM_FILES_DIR),
      '#description'    => check_plain(t('If left blank it will use Drupals default files directory.') . ' ' . $extra),
    );
  }
  $form['advagg_checksum_mode'] = array(
    '#type'           => 'radios',
    '#title'          => t('File Checksum Mode'),
    '#default_value'  => variable_get('advagg_checksum_mode', ADVAGG_CHECKSUM_MODE),
    '#options'        => array(
      'mtime' => 'mtime',
      'md5' => 'md5',
    ),
    '#description'    => t('If Drupal is on multiple servers and the file system is not shared;  using md5 is recommended. The file modification time (mtime) could be different with this type of setup. WARNING: Changing this value will invalidate all bundles and new ones will have to be built.'),
  );
  $form['advagg_server_addr'] = array(
    '#type'           => 'textfield',
    '#title'          => t('IP Address to send all asynchronous requests to'),
    '#default_value'  => variable_get('advagg_server_addr', FALSE),
    '#description'    => t('If left blank it will use the same server as the request. If set to -1 it will use the host name instead of an IP address.'),
  );
//   $form['advagg_debug'] = array(
//     '#type'           => 'checkbox',
//     '#title'          => t('Debug to watchdog.'),
//     '#default_value'  => variable_get('advagg_debug', ADVAGG_DEBUG),
//   );

  $form['flush'] = array(
    '#type'         => 'fieldset',
    '#title'        => t('Smart cache flush'),
    '#description'  => t('Scan all files referenced in aggregated files. If any of them have changed, increment the counters containing that file and rebuild the bundle.'),
  );
  $form['flush']['advagg_flush'] = array(
    '#type' => 'submit',
    '#value' => t('Flush AdvAgg Cache'),
    '#submit' => array('advagg_admin_flush_cache_button'),
  );
  $form['rebuild'] = array(
    '#type'         => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'        => t('Cache Rebuild'),
    '#description'  => t('Recreate all aggregated files. Useful if JS or CSS compression was just enabled. %count Files', array('%count' => $bundle_count)),
  );
  $form['rebuild']['advagg_rebuild'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild AdvAgg Cache'),
    '#submit' => array('advagg_admin_batch_rebuild'),
  );
  $form['forced_rebuild'] = array(
    '#type'         => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'        => t('Forced Cache Rebuild'),
    '#description'  => t('Recreate all aggregated files by incrementing internal counter for every bundle. %count Files', array('%count' => $bundle_count)),
  );
  $form['forced_rebuild']['advagg_forced_build'] = array(
    '#type' => 'submit',
    '#value' => t('Force all counters to be increment by one'),
    '#submit' => array('advagg_admin_batch_rebuild'),
  );
  $form['master_reset'] = array(
    '#type'         => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'        => t('Master Reset'),
    '#description'  => t('Clean Slate - Truncate all advagg tables and delete all advagg files.'),
  );
  $form['master_reset']['advagg_reset'] = array(
    '#type' => 'submit',
    '#value' => t('Master Reset'),
    '#submit' => array('advagg_admin_master_reset'),
  );
  $form['htaccess'] = array(
    '#type'         => 'fieldset',
    '#collapsible'   => TRUE,
    '#collapsed'     => TRUE,
    '#title'        => t('Rebuild htaccess files'),
    '#description'  => t('This will recreate the htaccess files located in the advagg_* directories.'),
  );
  $form['htaccess']['advagg_recreate_htaccess'] = array(
    '#type' => 'submit',
    '#value' => t('Recreate htaccess files'),
    '#submit' => array('advagg_admin_recreate_htaccess'),
  );

  $form['#submit'][] = 'advagg_admin_settings_form_submit';
  return system_settings_form($form);
}

/**
 * Validate form values. Used to unset variables before they get saved.
 */
function advagg_admin_settings_form_validate($form, &$form_state) {
  global $conf;

  // Custom directory handling.
  if (!empty($form_state['values']['advagg_custom_files_dir'])) {
    $files_dir = $form_state['values']['advagg_custom_files_dir'];
    if ($files_dir != $conf['advagg_custom_files_dir']) {

      // Try to create dir structure.
      $cumulative = '';
      $path_array = explode('/', $files_dir);
      foreach ($path_array as $dir) {
        if (empty($cumulative)) {
          $cumulative = $dir;
        }
        else {
          $cumulative .= '/' . $dir;
        }
        // If dir creation fails, bail out of loop.
        if (!file_check_directory($cumulative, FILE_CREATE_DIRECTORY)) {
          break;
        }
      }

      // See if dir exists and is writable.
      if (file_check_directory($files_dir) == TRUE) {

        // Remove old files.
        list($css_path, $js_path) = advagg_get_root_files_dir();
        file_scan_directory($css_path, '.*', array('.', '..', 'CVS'), 'file_delete', TRUE);
        @unlink($css_path);
        file_scan_directory($js_path, '.*', array('.', '..', 'CVS'), 'file_delete', TRUE);
        @unlink($js_path);

        // Set new path.
        $conf['advagg_custom_files_dir'] = $files_dir;
        advagg_get_root_files_dir(TRUE);
        menu_rebuild();
      }
      else {
        form_set_error('advagg_custom_files_dir', t('%dir is not a directory or is not writable by the web server.', array('%dir' => $files_dir)));
      }
    }
  }

  // If the IP field is not blank, check that its a valid address.
  if (!empty($form_state['values']['advagg_server_addr']) && $form_state['values']['advagg_server_addr'] != -1 && ip2long($form_state['values']['advagg_server_addr']) === FALSE) {
    form_set_error('advagg_server_addr', t('Must be a valid IP address.'));
  }

  // Remove non variable form info.
  unset($form_state['values']['advagg_flush']);
  unset($form_state['values']['advagg_rebuild']);
  unset($form_state['values']['advagg_forced_build']);
  unset($form_state['values']['advagg_recreate_htaccess']);
  unset($form_state['values']['advagg_reset']);
}


/**
 * Validate form values. Used to unset variables before they get saved.
 */
function advagg_admin_settings_form_submit($form, &$form_state) {
  global $conf;

  // Gzip & htaccess checks.
  list($css_path, $js_path) = advagg_get_root_files_dir();
  $css_path .= '/.htaccess';
  $js_path .= '/.htaccess';

  if ($conf['advagg_gzip_compression'] != $form_state['values']['advagg_gzip_compression'] && $form_state['values']['advagg_dir_htaccess']) {
    $conf['advagg_gzip_compression'] = $form_state['values']['advagg_gzip_compression'];
    $conf['advagg_dir_htaccess'] = $form_state['values']['advagg_dir_htaccess'];
    advagg_admin_recreate_htaccess();
  }
  if ($form_state['values']['advagg_dir_htaccess'] == FALSE) {
    $conf['advagg_dir_htaccess'] = FALSE;
    if (file_exists($css_path) || file_exists($js_path)) {
      file_delete($css_path);
      file_delete($js_path);
      drupal_set_message(t('Advanced CSS/JS Aggregation directory level htaccess files have been removed.'));
    }
  }
  elseif ($conf['advagg_dir_htaccess'] != $form_state['values']['advagg_dir_htaccess']) {
    $conf['advagg_dir_htaccess'] = $form_state['values']['advagg_dir_htaccess'];
    advagg_admin_recreate_htaccess();
  }
  if ($conf['advagg_dir_htaccess'] && (!file_exists($css_path) || !file_exists($js_path))) {
    advagg_admin_recreate_htaccess();
  }

  // If checksum mode changed, smart flush cache with new checksum mode selected.
  if ($conf['advagg_checksum_mode'] != $form_state['values']['advagg_checksum_mode']) {
    $conf['advagg_checksum_mode'] = $form_state['values']['advagg_checksum_mode'];
    advagg_admin_flush_cache_button();
  }
}

/**
 * Master reset button.
 */
function advagg_admin_master_reset() {
  cache_clear_all('*', 'cache_advagg', TRUE);
  cache_clear_all('*', 'cache_advagg_files_data', TRUE);
  cache_clear_all('*', 'cache_advagg_bundle_reuse', TRUE);
  db_query("TRUNCATE TABLE {advagg_files}");
  db_query("TRUNCATE TABLE {advagg_bundles}");

  module_invoke_all('advagg_master_reset');

  list($css_path, $js_path) = advagg_get_root_files_dir();
  file_scan_directory($css_path, '.*', array('.', '..', 'CVS'), 'file_delete', TRUE);
  file_scan_directory($js_path, '.*', array('.', '..', 'CVS'), 'file_delete', TRUE);

  drupal_set_message(t('Advanced CSS/JS Aggregation has been reset.'));
}

/**
 * Cache clear button.
 */
function advagg_admin_flush_cache_button() {
  global $_advagg;
  _drupal_flush_css_js();
  $cache_tables = advagg_flush_caches();
  foreach ($cache_tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }

  if (empty($_advagg['files'])) {
    drupal_set_message(t('Advanced CSS/JS Aggregation cache scanned and no out of date bundles detected.'));
  }
  else {
    if (empty($_advagg['rebuilt'])) {
      drupal_set_message(t('Advanced CSS/JS Aggregation cache scanned and out of date bundles have been marked. <br />Old Files: <br />!files <br />Marked Bundles Count: %count', array(
        '!files' => filter_xss(implode('<br />', $_advagg['files'])),
        '%count' => count($_advagg['bundles']),
      )
      ));
    }
    else {
      drupal_set_message(t('Advanced CSS/JS Aggregation cache scanned and out of date bundles have been incremented and rebuilt. <br />Old Files: <br />%files <br />%count done.', array(
        '%files' => implode('<br />', $_advagg['files']),
        '%count' => count($_advagg['rebuilt']),
      )
      ));
    }
  }
}

/**
 * Cache clear callback for admin_menu/flush-cache/advagg.
 */
function advagg_admin_flush_cache() {
  advagg_admin_flush_cache_button();
  drupal_goto();
}

/**
 * Set up batch for first and last name loading.
 *
 * This is where Drupal's Batch API comes into play.
 * It's as simple as defining $batch, and then calling batch_set($batch)
 */
function advagg_admin_batch_rebuild($form, &$form_state) {
  $batch = array();

  // Set up the type of batch operation we will do.
  if ($form_state['clicked_button']['#post']['op'] == t('Force all counters to be increment by one')) {
    $increment = TRUE;
    $batch['title'] = t('Increment and rebuild aggregated files');

  }
  else {
    $increment = FALSE;
    $batch['title'] = t('Rebuilding aggregated files');
  }

  // Build batch operational data.
  $batch += array(
    'operations' => array(
      array('advagg_admin_rebuild_bundles', array($increment)),
    ),
    'finished' => 'advagg_admin_rebuild_bundles_done',
    'init_message' => t('Initializing...'),
    'progress_message' => t('@current of @total batch operations done.'),
    'error_message' => t('Rebuilding aggregated files encountered an error.'),
    'file' => drupal_get_path('module', 'advagg') . '/advagg.admin.inc',
  );

  // Run it
  batch_set($batch);
  batch_process('admin/settings/advagg');
}

/**
 * Rebuild bundles.
 */
function advagg_admin_rebuild_bundles($increment, &$context) {
  // init batch.
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = db_result(db_query("SELECT COUNT(*) FROM (SELECT bundle_md5 FROM {advagg_bundles} GROUP BY bundle_md5) as temp"));

    // Increment All Counters.
    if ($increment) {
      db_query("UPDATE {advagg_bundles} SET counter = counter + 1");
    }
  }

  // Set limit.
  $limit = 1;

  // Get list of all bundles.
  $result = db_query_range("SELECT bundle_md5 FROM {advagg_bundles} GROUP BY bundle_md5", $context['sandbox']['progress'], $limit);
  while ($bundle_md5 = db_result($result)) {
    $filenames = advagg_rebuild_bundle($bundle_md5, '', TRUE);
    $filename = array();
    foreach ($filenames as $name => $data) {
      $filename[] = $name;
    }

    // Update our progress information.
    $context['sandbox']['progress']++;
    $context['message'] = t('%md5 Created', array('%md5' => $bundle_md5));
    $context['results'][] = $filename;
  }

  // Inform the batch engine that we are not finished,
  // and provide an estimation of the completion level we reached.
  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function advagg_admin_rebuild_bundles_done($success, $results, $operations) {
  if ($success) {
    // Here we do something meaningful with the results.
    $message = count($results) .' files generated.';
    $message .= theme('item_list', $results);
  }
  else {
    // An error occurred.
    // $operations contains the operations that remained unprocessed.
    $error_operation = reset($operations);
    $message = t('An error occurred while processing %error_operation with arguments: @arguments', array('%error_operation' => $error_operation[0], '@arguments' => print_r($error_operation[1], TRUE)));
  }
  drupal_set_message($message);
}

/**
 * Rebuild htaccess files.
 */
function advagg_admin_recreate_htaccess() {
  list($css_path, $js_path) = advagg_get_root_files_dir();
  $css_path .= '/.htaccess';
  $js_path .= '/.htaccess';

  file_delete($css_path);
  advagg_htaccess_check_generate($css_path, TRUE);

  file_delete($js_path);
  advagg_htaccess_check_generate($js_path, TRUE);

  drupal_set_message(t('Advanced CSS/JS Aggregation htaccess files rebuilt.'));
}

function advagg_form_print_r($data) {
  $data = print_r($data, TRUE);
  $data = explode("\n", $data);
  $new_data = '';
  foreach ($data as $text) {
    $new_text = str_replace('    ', '', $text);
    if ($new_text == "(") {
      $new_data .= ' ' . $new_text;
    }
    else {
      $new_data .= "\n" . $text;
    }
  }
  $new_data = explode("\n", $new_data);
  $new_data = array_filter($new_data);
  $rows = count($new_data);
  $new_data = implode("\n", $new_data);
  return array($new_data, $rows);
}
