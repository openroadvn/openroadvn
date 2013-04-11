<?php
// $Id: nat.views.inc,v 1.2.2.10 2010/11/18 05:49:42 karthik Exp $

/**
 * @file
 * NAT (Node Auto Term) Views implementation.
 */

/**
 * Implementation of hook_views_handlers() to register all the basic handlers
 * that Views uses.
 */
function nat_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'nat') . '/includes'
    ),
    'handlers' => array(
      'nat_handler_argument_term_node_tid_depth' => array(
        'parent' => 'views_handler_argument'
      ),
      'nat_handler_argument_term_node_tid_depth_modifier' => array(
        'parent' => 'views_handler_argument'
      )
    )
  );
}

/**
 * Implementation of hook_views_data().
 */
function nat_views_data() {
  $data = array();

  $data['nat'] = array('table' => array('group' => t('NAT')));

  $data['nat']['table']['join']['node'] = array(
    'left_table' => 'nat_term_node',
    'left_field' => 'tid',
    'field' => 'tid'
  );

  $data['nat_term_node']['table']['join']['node'] = array(
    'table' => 'term_node',
    'left_field' => 'nid',
    'field' => 'nid'
  );

  // Add NAT node to term view.
  $data['nat']['table']['join']['term_data'] = array(
    'left_field' => 'tid',
    'field' => 'tid'
  );

  $data['nat']['nid'] = array(
    'title' => t('Nid'),
    'help' => t('The node ID of the NAT node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric'
    ),
    'relationship' => array(
      'base' => 'node',
      'field' => 'nid',
      'label' => t('NAT Node')
    )
  );

  // Term to NAT node handling
  $data['tan'] = array(
    'table' => array(
      'group' => t('NAT')
    ),
  );

  $data['tan']['table']['join']['node'] = array(
    'table' => 'nat',
    'left_field' => 'nid',
    'field' => 'nid'
  );

  $data['tan']['tid'] = array(
    'title' => t('Tid'),
    'help' => t('The tid of the NAT term.'),
    'argument' => array(
      'handler' => 'views_handler_argument_term_node_tid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'tid'
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric'
    )
  );

  // Uses data from Term Node Count module.
  if (module_exists('term_node_count')) {
    $data['nat_term_node_count']['table']['group'] = t('NAT');
    $data['nat_term_node_count']['table']['join']['node'] = array(
      'table' => 'term_node_count',
      'left_table' => 'nat_tnc',
      'left_field' => 'tid',
      'field' => 'tid'
    );
    $data['nat_tnc']['table']['join']['node'] = array(
      'table' => 'nat',
      'left_field' => 'nid',
      'field' => 'nid'
    );
    $data['nat_term_node_count']['node_count'] = array(
      'field' => 'node_count',
      'title' => t('Node Count'),
      'help' => t('The number of nodes associated with NAT term'),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric'
      )
    );
  }

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function nat_views_data_alter(&$data) {
  $data['node']['nat_term_node_tid_depth'] = array(
    'group' => t('NAT'),
    'title' => t('Nid (with depth)'),
    'help' => t('The node ID of the NAT with depth.'),
    'real field' => 'vid',
    'argument' => array(
      'handler' => 'nat_handler_argument_term_node_tid_depth',
      'accept depth modifier' => TRUE
    )
  );

  $data['node']['nat_term_node_tid_depth_modifier'] = array(
    'group' => t('NAT'),
    'title' => t('Nid depth modifier'),
    'help' => t('Allows the "depth" for NAT: Nid (with depth) to be modified via an additional argument.'),
    'argument' => array(
      'handler' => 'nat_handler_argument_term_node_tid_depth_modifier'
    )
  );
}
