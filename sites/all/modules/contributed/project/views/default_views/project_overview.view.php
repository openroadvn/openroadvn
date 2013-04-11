<?php
// $Id: project_overview.view.php,v 1.1 2009/11/25 16:44:51 dww Exp $

/**
 * @file
 * A view for browsing projects on the site.
 */

$view = new view;
$view->name = 'project_overview';
$view->description = 'A view for browsing projects (currently broken).';
$view->tag = 'project';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = TRUE;
$handler = $view->new_display('default', 'Defaults', 'default');

$fields = array(
  'title' => array(
    'label' => '',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'changed' => array(
    'label' => 'Last changed',
    'date_format' => 'time ago',
    'custom_date_format' => '2',
    'exclude' => 0,
    'id' => 'changed',
    'table' => 'node',
    'field' => 'changed',
    'relationship' => 'none',
  ),
  'teaser' => array(
    'label' => '',
    'exclude' => 0,
    'id' => 'teaser',
    'table' => 'node_revisions',
    'field' => 'teaser',
    'relationship' => 'none',
  ),
  'project_type_tid' => array(
    'label' => '',
    'type' => 'separator',
    'separator' => '  ',
    'empty' => '',
    'link_to_taxonomy' => 1,
    'limit' => 0,
    'vids' => array(
      '1' => 1,
    ),
    'exclude_top_level_terms' => 1,
    'exclude' => 0,
    'id' => 'project_type_tid',
    'table' => 'term_node',
    'field' => 'project_type_tid',
    'relationship' => 'none',
  ),
);

$sorts = array(
  'sticky' => array(
    'order' => 'DESC',
    'id' => 'sticky',
    'table' => 'node',
    'field' => 'sticky',
    'relationship' => 'none',
  ),
  'title' => array(
    'order' => 'ASC',
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'changed' => array(
    'order' => 'DESC',
    'granularity' => 'day',
    'id' => 'changed',
    'table' => 'node',
    'field' => 'changed',
  ),
  'title_1' => array(
    'order' => 'ASC',
    'id' => 'title_1',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
);

$arguments = array(
  'tid' => array(
    'default_action' => 'not found',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => '%1',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'project_type_term',
    'validate_fail' => 'not found',
    'break_phrase' => 0,
    'add_table' => 0,
    'require_value' => 0,
    'reduce_duplicates' => 1,
    'set_breadcrumb' => 0,
    'id' => 'tid',
    'table' => 'term_node',
    'field' => 'tid',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(),
    'validate_argument_type' => 'tid',
    'validate_argument_project_term_vocabulary' => array(
      '1' => 1,
    ),
    'validate_argument_project_term_argument_type' => 'convert',
    'validate_argument_project_term_argument_action_top_without' => 'pass',
    'validate_argument_project_term_argument_action_top_with' => 'pass',
    'validate_argument_project_term_argument_action_child' => 'pass',
    'validate_argument_php' => '',
    'override' => array(
      'button' => 'Override',
    ),
  ),
);

$filters = array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'project_project' => 'project_project',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
  'status' => array(
    'operator' => '=',
    'value' => 1,
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status',
    'table' => 'node',
    'field' => 'status',
    'relationship' => 'none',
  ),
  'keys' => array(
    'operator' => 'optional',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'keys_op',
      'identifier' => 'keywords',
      'label' => 'Keywords:',
      'optional' => 1,
      'remember' => 0,
    ),
    'id' => 'keys',
    'table' => 'search_index',
    'field' => 'keys',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
  'project_type_tid' => array(
    'operator' => 'or',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'project_type_tid_op',
      'identifier' => 'type',
      'label' => 'Category:',
      'optional' => 1,
      'single' => 1,
      'remember' => 0,
      'reduce' => 0,
    ),
    'type' => 'select',
    'vid' => '1',
    'associated_argument' => '0',
    'remove_if_no_options' => 1,
    'argument_position' => '0',
    'id' => 'project_type_tid',
    'table' => 'term_node',
    'field' => 'project_type_tid',
    'relationship' => 'none',
    'reduce_duplicates' => 1,
  ),
  'project_sort_method' => array(
    'value' => 'sticky',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'operator' => '',
      'label' => 'Sort by:',
      'identifier' => 'sort_by',
      'optional' => 0,
      'single' => 1,
      'remember' => 0,
    ),
    'sort_methods' => array(
      'changed' => array(
        'enabled' => 1,
        'weight' => '-23',
        'display_name' => 'Date',
      ),
      'title_1' => array(
        'enabled' => 0,
        'weight' => '-25',
        'display_name' => 'title_1',
      ),
      'sticky' => array(
        'enabled' => 0,
        'weight' => '-24',
        'display_name' => 'sticky',
      ),
      'title' => array(
        'enabled' => 1,
        'weight' => '-22',
        'display_name' => 'Name',
      ),
    ),
    'id' => 'project_sort_method',
    'table' => 'views',
    'field' => 'project_sort_method',
    'relationship' => 'none',
  ),
);

$handler->override_option('fields', $fields);
$handler->override_option('sorts', $sorts);
$handler->override_option('arguments', $arguments);
$handler->override_option('filters', $filters);
$handler->override_option('access', array(
  'type' => 'perm',
  'role' => array(),
  'perm' => 'browse project listings',
));
$handler->override_option('empty', 'No results were found.');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 30);
$handler->override_option('use_pager', '1');
$handler->override_option('distinct', 1);
$handler->override_option('style_plugin', 'project_list');
$handler->override_option('row_plugin', 'project_fields');
$handler->override_option('row_options', array(
  'inline' => array(),
  'separator' => '',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'project/%');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => 'Yuki',
  'weight' => '0',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler = $view->new_display('feed', 'Feed', 'feed_1');
$handler->override_option('style_plugin', 'rss');
$handler->override_option('style_options', array(
  'mission_description' => FALSE,
  'description' => '',
));
$handler->override_option('row_plugin', 'node_rss');
$handler->override_option('row_options', array(
  'item_length' => 'default',
));
$handler->override_option('path', 'project/%/feed');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler->override_option('displays', array(
  'default' => 'default',
  'page_1' => 'page_1',
));
$handler->override_option('sitename_title', FALSE);

