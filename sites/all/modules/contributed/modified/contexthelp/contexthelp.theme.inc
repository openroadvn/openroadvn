<?php
// $id

/**
 * Themes the contexthelp block that can be displayed on the page
 */
function theme_contexthelp_block() {
  global $user;
  $output = '';

  // check if the user has access to view the context help information
  if (user_access('context help view')) {
    $url = _contexthelp_get_url();
    $node_id = contexthelp_get_help($url);
    if (is_numeric($node_id)) {
      // checking if the user has administrative access if so allow them to easily go to the edit screen
      if (user_access('administer context help')) {
        $output = '<div>'. l(t('Edit Help'), 'node/'. $node_id .'/edit') .'</div>';
      }
      // calling the associated view so we can display the context help
      $output .= views_embed_view('contexthelp', 'default', $node_id, 'all', _contexthelp_get_user_locale());
    }
    else {
      $output = t('No context help available for this section');
      if (user_access('administer context help')) {
        $contexthelp_type = variable_get('contexthelp_node_type', 'contexthelp');
        if (!is_null($contexthelp_type) && drupal_strlen($contexthelp_type) > 0) {
          $output .= '<div>'. l(t('Create Help'), 'node/add/'. str_replace("_", "-", $contexthelp_type), array('query' => array('url' => $url))) .'</div>';
        }
      }
      else {
        drupal_add_js('$(document).ready(function() { Drupal.contexthelp.noContentHelp = 1; });', 'inline');
      }
    }
  }
  return $output;
}

/**
 * Themeing function for the administration section url mappings
 */
function theme_contexthelp_url_mappings_form($form) {
  if (isset($form['mappings'])) {
    $output = drupal_render($form['mappings']);
  }
  $rows = array();
  $rows[] = array(
    'url' => drupal_render($form['url']),
    'link' => drupal_render($form['submit']),
  );
  $output .= theme('table', NULL, $rows);
  $output .= drupal_render($form);
  return $output;

}

