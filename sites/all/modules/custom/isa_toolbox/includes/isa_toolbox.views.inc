<?php
// $Id: isa_toolbox.views.inc v 1.0 2011/07/27 10:00:00 Exp $

/**
 * @file
 *
 * Provides the group type field
 */

/**
 * Tell views about the new handler
 */
function isa_toolbox_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'isa_toolbox') .'/includes',
    ),
    'handlers' => array(
      'isa_toolbox_handler_field_group_type' => array(
        'parent' => 'views_handler_field_custom',
      ),
      'isa_toolbox_handler_field_group_short_name' => array(
        'parent' => 'views_handler_field_custom',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function isa_toolbox_views_data() {
  $data = array();
  $groupname = t('Node');
  $data['isa_toolbox']['table']['group'] = $groupname;

  $data['isa_toolbox']['table']['join']['node'] = array();

  $data['isa_toolbox']['group_type'] = array(
    'title' => t('Group type'),
    'help' => t('The node group type'),
    'field' => array(
      'handler' => 'isa_toolbox_handler_field_group_type', 
      'click sortable' => TRUE,
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_custom',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  $data['isa_toolbox']['group_short_name'] = array(
    'title' => t('Group short name'),
    'help' => t('The short name of the community or the project'),
    'field' => array(
      'handler' => 'isa_toolbox_handler_field_group_short_name',
      'click sortable' => TRUE,
    ),
  );

  return $data;
}
