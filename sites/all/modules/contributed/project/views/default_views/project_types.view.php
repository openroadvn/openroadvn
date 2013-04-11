<?php
// $Id: project_types.view.php,v 1.1 2009/11/25 16:44:51 dww Exp $

/**
 * @file
 * An overview listing of site-wide project types with descriptions.
 */

$view = new view;
$view->name = 'project_types';
$view->description = 'A list of project types with descriptions (currently broken).';
$view->tag = 'project';
$view->view_php = '';
$view->base_table = 'term_data';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = TRUE;
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name' => array(
    'id' => 'name',
    'table' => 'term_data',
    'field' => 'name',
    'label' => '',
    'relationship' => 'none',
    'link_to_taxonomy' => 1,
  ),
  'description' => array(
    'id' => 'description',
    'table' => 'term_data',
    'field' => 'description',
    'label' => '',
    'relationship' => 'none',
  ),
));
$handler->override_option('filters', array(
  'project_type' => array(
    'operator' => 'in',
    'value' => array(
      '1' => 1,
    ),
    'group' => 0,
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'include' => array(
      'top_level' => 'top_level',
    ),
    'id' => 'project_type',
    'table' => 'term_data',
    'field' => 'project_type',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'role' => array(),
  'perm' => 'browse project listings',
));
$handler->override_option('title', 'Project types');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 30);
$handler->override_option('use_pager', '1');
$handler->override_option('distinct', 1);
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'type' => 'ul',
));
$handler->override_option('row_options', array(
  'inline' => array(),
  'separator' => '',
));
$handler = $view->new_display('page', 'Page: Project types', 'page');
$handler->override_option('path', 'project');
$handler->override_option('menu', array(
  'type' => 'normal',
  'title' => 'Projects',
  'weight' => '0',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
