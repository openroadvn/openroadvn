<?php
// $id

/**
 * Returns the page with all of the context help available in the system
 */
function contexthelp_list_page() {
  $output = '';

  // only display if we have the contexthelp node type specified in the module configuration
  if ($contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp')) {
    // table headers we will display to the user
    if (user_access('administer context help')) {
      $header[] = array('data' => t('NID'), 'field' => 'nid');
    }
    $header[] = array('data' => t('Url'), 'field' => 'url');
    $header[] = array('data' => t('Title'), 'field' => 'title');

    // building the query pager so we can run the sql query
    $rowcount = variable_get('contexthelp_pager_count', 20);
    $count_sql = sprintf("SELECT DISTINCT nid FROM {node} WHERE type = '%s'", $contexthelp_type);
    $sql = sprintf("SELECT nid FROM {node} WHERE type = '%s'". tablesort_sql($header), $contexthelp_type);

    // running the query and processing the returned rows
    $result = pager_query($sql, $rowcount, NULL, $count_sql);
    while ($row = db_fetch_object($result)) {
      $node = node_load($row->nid);
      if (user_access('administer context help')) {
        $entry[$node->nid]['nid'] = l($node->nid, 'node/'. $node->nid .'/edit');
      }
      $entry[$node->nid]['url'] = $node->field_url[0]['value'];
      $entry[$node->nid]['title'] = $node->title;
    }

    $output = theme('table', $header, $entry);
    $output .= theme('pager', NULL, $rowcount, 0);
  }
  else {
    watchdog('contexthelp', 'Context Help Type has not been defined in the system, cannot view the list of contexthelp nodes in the system', NULL, WATCHDOG_ERROR);
    drupal_set_message(t('Context Help Content Type is not set in the system. Please verify the <a href="@settings">configuration settings</a>.', array('@settings' => url('admin/settings/contexthelp'))), 'error');
  }
  return $output;
}

/**
 * Function for creating the Export Help Form
 */
function contexthelp_export_help_form(&$form_state) {
  $form = array();
  $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
  if (!is_null($contexthelp_type) && drupal_strlen($contexthelp_type) > 0) {
    if (!isset($form_state['storage']['export_nodes'])) {
      $options = array();
      $result = db_query("SELECT nid FROM {node} WHERE type = '%s'", $contexthelp_type);
      $system_locales = locale_language_list('name');
      while ($row = db_fetch_object($result)) {
        $node = node_load($row->nid);
        $options[$row->nid] = $node->title .' ('. $node->field_url[0]['value'] .')';
        if (isset($node->language) && drupal_strlen($node->language) > 0) {
          $options[$row->nid] .= ' '. $system_locales[$node->language];
        }
      }
      $form['export_nodes'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Context Help Export'),
        '#options' => $options,
        '#description' => t('Choose the help content you wish to export from the system'),
      );
      $form['submit'] = array('#type' => 'submit', '#value' => t('Export'));
    }
    else {
      // Form was submitted so now we want too retrieve the nodes and display the XML
      $nids = array_values($form_state['storage']['export_nodes']);
      $output = t('No help was exported from the system');
      if (!empty($nids)) {
        $default_locale = variable_get('contexthelp_default_locale', 'en');
        $output = '$contexthelp = array(';
        $count = 0;
        foreach ($nids as $nid) {
          // If 0 then it was not checked we don't want this one
          if ($nid === 0) { continue; }
          $node = node_load($nid);
          $output .= $count . ' => array(';
          $output .= "'title' => '". str_replace("'", "\'", $node->title) ."', ";
          $output .= "'body' => '". str_replace("'", "\'", $node->body) ."', ";
          if (isset($node->language) && !is_null($node->language) && drupal_strlen($node->language) > 0) {
            $output .= "'language' => '". str_replace("'", "\'", $node->language) ."', ";
          }
          else {
            $output .= "'language' => '". str_replace("'", "\'", $default_locale) ."', ";
          }
          $output .= "'url' => '". str_replace("'", "\'", $node->field_url[0]['value']) ."',), ";
          $count++;
        }
        $output .= ');';
      }
      $form['export_nodes'] = array(
        '#type' => 'textarea',
        '#title' => t('Context Help'),
        '#default_value' => $output,
        '#description' => t('Context Help in the format that can be imported back into the system'),
        '#rows' => 30,
      );
    }
  }
  else {
    watchdog('contexthelp', 'Context Help Type has not been defined in the system, cannot use Export functionality without the type being set', NULL, WATCHDOG_ERROR);
    drupal_set_message(t('Context Help Content Type is not set in the system. Please verify the <a href="@settings">configuration settings</a>.', array('@settings' => url('admin/settings/contexthelp'))), 'error');
  }
  return $form;
}

/**
 * Form submission handler for the contexthelp export help form
 */
function contexthelp_export_help_form_submit(&$form, &$form_state) {
  $form_state['rebuild'] = TRUE;
  $form_state['storage']['export_nodes'] = $form_state['values']['export_nodes'];
}

/**
 * Function for creating the Import Help Form
 */
function contexthelp_import_help_form(&$form_state) {
  $form = array();
  $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
  if (!is_null($contexthelp_type) && drupal_strlen($contexthelp_type) > 0) {
    $form['import'] = array(
      '#type' => 'textarea',
      '#title' => t('Import Context Help'),
      '#description' => t('Import Context Help into the system'),
      '#rows' => 30,
    );
    $form['submit'] = array('#type' => 'submit', '#value' => t('Import'));
  }
  else {
    watchdog('contexthelp', 'Context Help Type has not been defined in the system, cannot use Import functionality without the type being set', NULL, WATCHDOG_ERROR);
    drupal_set_message(t('Context Help Content Type is not set in the system. Please verify the <a href="@settings">configuration settings</a>.', array('@settings' => url('admin/settings/contexthelp'))), 'error');
  }
  return $form;
}

/**
 * Form submission handler for the contexthelp import form
 */
function contexthelp_import_help_form_submit(&$form, &$form_state) {
  global $user;
  // Check to make sure something actually was entered into the form
  if (isset($form_state['values']['import']) && !is_null($form_state['values']['import']) && drupal_strlen($form_state['values']['import']) > 0) {
    eval($form_state['values']['import']);
    // Data should now be in the correct format so we can create the pertinent nodes in the system
    if (is_array($contexthelp) && !empty($contexthelp)) {
      $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
      $default_locale = variable_get('contexthelp_default_locale', 'en');
      foreach ($contexthelp as $help) {
        $node = new StdClass();
        $node->uid = $user->uid;
        $node->type = $contexthelp_type;
        $node->title = filter_xss_admin($help['title']);
        $node->body = filter_xss_admin($help['body']);
        if (isset($help['language']) && !is_null($help['language']) && drupal_strlen($help['language']) > 0) {
          $node->language = filter_xss_admin($help['language']);
        }
        else {
          $node->language = $default_locale;
        }
        $node->field_url[0]['value'] = filter_xss_admin($help['url']);
        $node->name = $user->name;
        $node = node_submit($node);
        node_save($node);
      }
      drupal_set_message(t('Context Help has been successfully imported into the system'));
    }
    else {
      watchdog('contexthelp', t('Invalid import received by the system. !import'), array('!import' => $form_state['values']['import']), WATCHDOG_ERROR);
      drupal_set_message(t('Invalid information received by the Import function'), 'error');
    }
  }
  else {
    drupal_set_message(t('Invalid / No Context Help information was entered'), 'error');
  }
}

/**
 * Function for creating the url mappings form
 */
function contexthelp_url_mappings_form(&$form_state) {
  $form = array();
  $mappings = variable_get('contexthelp_url_mappings', array());
  if (is_array($mappings) && !empty($mappings)) {
    $rows = array();
    $header = array('url' => t('Url'), 'link' => t('Operation'));
    foreach ($mappings as $k => $v) {
      $rows[] = array('url' => $mappings[$k], 'link' => l(t('Delete'), 'admin/contexthelp/url/mappings/delete', array('query' => array('url' => $k))));
    }
    // Only display this section if the mappings are not empty
    if (!empty($rows)) {
      $form['mappings'] = array('#value' => theme('table', $header, $rows));
    }
  }
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Url'),
    '#description' => t('Url you wish help not to be displayed on'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
  );
  $form['#theme'] = 'contexthelp_url_mappings_form';
  return $form;
}

/**
 * Form submission handler for the url mappings form
 **/
function contexthelp_url_mappings_form_submit(&$form, &$form_state) {
  if (isset($form_state['values']['url']) && !is_null($form_state['values']['url']) && drupal_strlen($form_state['values']['url']) > 0) {
    $mappings = variable_get('contexthelp_url_mappings', array());
    $url = $form_state['values']['url'];
    if (isset($mappings[$url])) {
      drupal_set_message(t('Value: !url is already mapped in the system', array('!url' => $url)), 'error');
    }
    else {
      $mappings[$url] = $url;
      variable_set('contexthelp_url_mappings', $mappings);
      drupal_set_message(t('ContextHelp Url Mapping has been successfully created'));
    }
  }
  $form_state['redirect'] = 'admin/contexthelp/url/mappings';
}

/**
 * Remove a url mapping from the system
 */
function contexthelp_url_mappings_delete() {
  if (isset($_GET['url']) && !is_null($_GET['url']) && drupal_strlen($_GET['url']) > 0) {
    $mappings = variable_get('contexthelp_url_mappings', array());
    if (isset($mappings[$_GET['url']])) {
      unset($mappings[$_GET['url']]);
    }
    variable_set('contexthelp_url_mappings', $mappings);
  }
  else {
    watchdog('contexthelp', t('Invalid language %language was received by the module'), array('%language' => $language), WATCHDOG_ERROR);
    drupal_set_message(t('Invalid language %language was received by the module', array('%language' => $language), 'error'));
  }
  drupal_goto('admin/contexthelp/url/mappings');
}
