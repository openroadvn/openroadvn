<?php
// $Id: flag_highlight.views.inc,v 1.1.2.5 2009/11/19 00:10:35 gunzip Exp $

/**
 * @file
 *
 * Provides Flag HIGHLIGHT data to Views.
 */


/**
 * Implementation of hook_views_data().
 */
function isa_highlight_views_data() {
  $data = array();
  $groupname = t('Highlight');
  $data['isa_highlight']['table']['group'] = $groupname;


 $data['isa_highlight']['table']['join']['node'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'left_table' => 'node',
    'table' => 'isa_highlight',
    'left_field' => 'nid',
    'field' => 'nid',
  );
	

  // field (text)
  $data['isa_highlight']['gid'] = array(
    'title' => t('Community'),
    'help' => t('Display the highlight for a node.'),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid'
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Highlight'),
    ),
  );

  // field (text)
  $data['isa_highlight']['nid'] = array(
    'title' => t('Highlighted items'),
    'help' => t('Display the highlights for a community.'),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'handler' => 'views_handler_relationship',
      'label' => t('Highlight'),
    ),
  );



  $data['isa_highlight']['fhid'] = array(
    'title' => t('Highlight ID'),
    'help' => t('The primary key in the Highlight table'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE,
    ), 
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ), 
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );	
	
  $data['isa_highlight']['uid'] = array(
    'title' => t('User ID'),
    'help' => t('The ID of the user who proposed the highlight'),
    'field' => array(
      'handler' => 'views_handler_field_numeric', 
      'click sortable' => TRUE,
    ), 
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ), 
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
  );

  // field (timestamp)
  $data['isa_highlight']['timestamp'] = array(
    'title' => t('Created time'),
    'help' => t('Display the time the highlight was written.'),
    'field' => array(
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort_date',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_date',
    ),
  );
	
  return $data;
}
