<?php
// $Id: project_release_files.view.php,v 1.1 2009/11/30 09:40:25 dww Exp $

$view = new view;
$view->name = 'project_release_files';
$view->description = 'List of all files attached to a given release';
$view->tag = 'Project release';
$view->view_php = '';
$view->base_table = 'files';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('relationships', array(
  'nid' => array(
    'label' => 'Release node',
    'required' => 1,
    'id' => 'nid',
    'table' => 'project_release_file',
    'field' => 'nid',
    'relationship' => 'none',
  ),
));
$handler->override_option('fields', array(
  'file_name' => array(
    'label' => 'Download',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'link_to_file' => 1,
    'exclude' => 0,
    'id' => 'file_name',
    'table' => 'project_release_file',
    'field' => 'file_name',
    'relationship' => 'none',
  ),
  'filesize' => array(
    'label' => 'Size',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 1,
    'empty_zero' => 0,
    'file_size_display' => 'formatted',
    'exclude' => 0,
    'id' => 'filesize',
    'table' => 'files',
    'field' => 'filesize',
    'relationship' => 'none',
  ),
  'filehash' => array(
    'label' => 'md5 hash',
    'alter' => array(
      'alter_text' => 0,
      'text' => '',
      'make_link' => 0,
      'path' => '',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 1,
    'empty_zero' => 0,
    'exclude' => 0,
    'id' => 'filehash',
    'table' => 'project_release_file',
    'field' => 'filehash',
    'relationship' => 'none',
  ),
));
$handler->override_option('sorts', array(
  'fid' => array(
    'order' => 'DESC',
    'id' => 'fid',
    'table' => 'files',
    'field' => 'fid',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'nid' => array(
    'default_action' => 'not found',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'node',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'relationship' => 'nid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(),
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'project_release' => 'project_release',
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'validate_argument_project_term_vocabulary' => array(),
    'validate_argument_project_term_argument_type' => 'tid',
    'validate_argument_project_term_argument_action_top_without' => 'pass',
    'validate_argument_project_term_argument_action_top_with' => 'pass',
    'validate_argument_project_term_argument_action_child' => 'pass',
    'validate_argument_php' => '',
  ),
));
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('items_per_page', 0);
$handler->override_option('style_plugin', 'table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 0,
  'order' => 'asc',
  'columns' => array(
    'file_name' => 'file_name',
    'filesize' => 'filesize',
    'filehash' => 'filehash',
  ),
  'info' => array(
    'file_name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'filesize' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'filehash' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => '-1',
));
