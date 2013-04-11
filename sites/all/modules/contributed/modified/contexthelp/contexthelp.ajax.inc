<?php
// $id:

/**
 * Ajax handler to return the display for the view
 */
function contexthelp_retrieve() {
  $output = t('No Help is currently available');
  $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
  // check if the user has access to view the context help information
  if (user_access('context help view')) {
    $count = _contexthelp_get_node_ids();
    // Don't process if no node ids are returned
    if (is_numeric($count) && $count > 0) {
      $output = views_embed_view('contexthelp', 'default', 'all', _contexthelp_get_user_locale());
    }
  }
  print $output;
  exit;
}

/**
 * Menu Callback to retrieve all FAQs in the system
 */
function contexthelp_faq_retrieve() {
  $output = t('No FAQs are currently available');
  // Verify the user has access to view the context help information
  if (user_access('context help view')) {
    $count = _contexthelp_get_faq_node_ids();
    // Don't process if nothing matched
    if (is_numeric($count) && $count > 0) {
      $output = views_embed_view('faq_topics', 'default', _contexthelp_get_user_locale());
    }
  }
  print $output;
  exit;
}

/**
 * Function returns the count of contexthelp nodes that match the criteria
 */
function _contexthelp_get_node_ids() {
  $count = 0;
  $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
  // If the value is NULL or the String is empty we do not want to process anything
  if (!is_null($contexthelp_type) && drupal_strlen($contexthelp_type) > 0) {
    $count = db_result(db_query("SELECT COUNT(nid) FROM {node} WHERE type = '%s' AND language = '%s'", $contexthelp_type, _contexthelp_get_user_locale()));
  }
  else {
    watchdog('contexthelp', 'Context Help Type has not been defined in the system', NULL, WATCHDOG_ERROR);
    drupal_set_message(t('ContextHelp Content Type is not set in the system. Please verify the <a href="@settings">configuration settings</a>.', array('@settings' => url('admin/settings/contexthelp'))), 'error');
  }
  return $count;
}

/**
 * Function returns the count of faq nodes that match the criteria
 */
function _contexthelp_get_faq_node_ids() {
  $count = 0;
  $faq_type = variable_get('contexthelp_faq_node_type', 'contexthelp_faq');
  if (!is_null($faq_type) && drupal_strlen($faq_type) > 0) {
    $count = db_result(db_query("SELECT COUNT(nid) FROM {node} WHERE type = '%s' AND language = '%s'", $faq_type, _contexthelp_get_user_locale()));
  }
  else {
    watchdog('contexthelp', 'Context Help FAQ Type has not been defined in the system', NULL, WATCHDOG_ERROR);
    drupal_set_message(t('ContextHelp FAQ Content Type is not set in the system. Please verify the <a href="@settings">configuration settings</a>.', array('@settings' => url('admin/settings/contexthelp'))), 'error');
  }
  return $count;
}
