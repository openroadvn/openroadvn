<?php
// $Id: project_package.views.inc,v 1.1 2009/11/27 20:06:51 dww Exp $

/**
 * @file
 * Implement Views hooks to expose project_package data to views.
 */

/**
 * Implement hook_views_data().
 */
function project_package_views_data() {
  $data = array();

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this group by default.
  $data['project_package_local_release_item']['table']['group']  = t('Project package');

  // Allow implicit relationship to {node} for the items in a package. To work
  // with the package_nid, you should add an explicit relationship.
  $data['project_package_local_release_item']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'item_nid',
    ),
  );

  // package_nid
  $data['project_package_local_release_item']['package_nid'] = array(
    'title' => t('Package node'),
    'help' => t('The node ID of the package release node.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the package release.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Package release node'),
    ),
  );

  // item_nid
  $data['project_package_local_release_item']['item_nid'] = array(
    'title' => t('Item node'),
    'help' => t('The node ID of an item release node included in a package.'),
    'field' => array(
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_node_nid',
      'name field' => 'title',
      'numeric' => TRUE,
      'validate type' => 'nid',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'relationship' => array(
      'help' => t('Add a relationship to gain access to more node data for the item releases.'),
      'base' => 'node',
      'handler' => 'views_handler_relationship',
      'label' => t('Item release node'),
    ),
  );

  return $data;
}

