<?php
// $Id: isa_private_files.views.inc v 1.0 2011/07/25 11:00:00 Exp $

/**
 * @file
 *
 * Provides Download statistics data for Views.
 */

/**
 * Tell views about the new handler
 */
function isa_private_files_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'isa_private_files') .'/includes',
    ),
    'handlers' => array(
      'isa_private_files_handler_field_pid' => array(
        'parent' => 'views_handler_field_numeric',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function isa_private_files_views_data() {
  $data = array();
  $groupname = t('Download statistics');
  $data['isa_private_files']['table']['group'] = $groupname;

  $data['isa_private_files']['table']['join']['node'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'left_table' => 'node',
    'table' => 'isa_download_statistics',
    'left_field' => 'nid',
    'field' => 'pid',
  );

  $data['isa_private_files']['pid'] = array(
    'title' => t('Project downloads count'),
    'help' => t('The number of downloads for a project with \'downloads\' appended'),
    'field' => array(
      'handler' => 'isa_private_files_handler_field_pid', 
      'click sortable' => TRUE,
    ), 
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ), 
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  return $data;
}
