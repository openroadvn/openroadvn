<?php
// $Id: project_issue.views_default.inc,v 1.48 2009/09/23 00:21:36 dww Exp $


/**
 * @file
 * Provides default views on behalf of the project_issue module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function project_issue_views_default_views() {
  // Search the "default_views" directory for files ending in .view.php.
  $files = file_scan_directory(drupal_get_path('module', 'project_issue'). '/views/default_views', 'view.php');
  foreach ($files as $absolute => $file) {
    require_once $absolute;
    if (isset($view)) {
      // $file->name has the ".php" stripped off, but still has the ".view".
      $view_name = substr($file->name, 0, strrpos($file->name, '.'));
      $views[$view_name] = $view;
    }
  }
  return $views;
}

/**
 * Add exposed filters for every taxonomy vocabulary for project_issue nodes.
 *
 * This is a helper function used by a few of the default view definitions.
 */
function _project_issue_views_add_taxonomy_filters(&$filters) {
  if (!module_exists('taxonomy')) {
    return;
  }
  $issue_vocabularies = taxonomy_get_vocabularies('project_issue');
  $i = 0;
  foreach ($issue_vocabularies as $vid => $vocab) {
    $view_key = 'tid';
    if ($i) {
      $view_key .= "_$i";
    }
    $i++;
    $identifier = project_issue_views_filter_identifier($vocab->name);
    $filters[$view_key] = array(
      'operator' => 'or',
      'value' => '',
      'group' => '0',
      'exposed' => TRUE,
      'expose' => array(
        'use_operator' => 1,
        'operator' => $identifier . '_op',
        'identifier' => $identifier,
        'label' => check_plain($vocab->name),
        'optional' => 1,
        'single' => 1,
        'remember' => 0,
      ),
      'type' => 'textfield',
      'vid' => $vid,
      'id' => 'tid',
      'table' => 'term_node',
      'field' => 'tid',
      'hierarchy' => 0,
      'relationship' => 'none',
      'reduce_duplicates' => 1,
    );
  }
}

