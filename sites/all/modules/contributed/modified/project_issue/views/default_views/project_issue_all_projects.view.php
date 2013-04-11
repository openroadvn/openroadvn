<?php
// $Id: project_issue_all_projects.view.php,v 1.1 2009/06/18 03:38:43 dww Exp $

/**
 * @file
 * Issue queue across all projects.
 */

$view = new view;
$view->name = 'project_issue_all_projects';
$view->description = 'Project issues for all projects';
$view->tag = 'Project issue';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE;
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('relationships', array(
  'assigned' => array(
    'label' => 'Assigned user',
    'required' => 1,
    'id' => 'assigned',
    'table' => 'project_issues',
    'field' => 'assigned',
    'relationship' => 'none',
  ),
  'rid' => array(
    'label' => 'Version',
    'required' => 0,
    'id' => 'rid',
    'table' => 'project_issues',
    'field' => 'rid',
    'relationship' => 'none',
  ),
  'pid' => array(
    'label' => 'Project node',
    'required' => 1,
    'id' => 'pid',
    'table' => 'project_issues',
    'field' => 'pid',
  ),
));
$fields = array(
  'project_issue_queue' => array(
    'label' => 'Project',
    'link_type' => 'issues',
    'exclude' => 0,
    'id' => 'project_issue_queue',
    'table' => 'node',
    'field' => 'project_issue_queue',
    'relationship' => 'pid',
  ),
  'title' => array(
    'label' => 'Summary',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'timestamp' => array(
    'label' => '',
    'alter' => array(),
    'link_to_node' => 0,
    'comments' => 0,
    'exclude' => 0,
    'id' => 'timestamp',
    'table' => 'history_user',
    'field' => 'timestamp',
    'relationship' => 'none',
  ),
  'sid' => array(
    'id' => 'sid',
    'table' => 'project_issues',
    'field' => 'sid',
  ),
  'priority' => array(
    'id' => 'priority',
    'table' => 'project_issues',
    'field' => 'priority',
  ),
  'category' => array(
    'label' => 'Category',
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'category',
    'table' => 'project_issues',
    'field' => 'category',
    'relationship' => 'none',
  ),
  'comment_count' => array(
    'label' => 'Replies',
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => '',
    'exclude' => 0,
    'id' => 'comment_count',
    'table' => 'node_comment_statistics',
    'field' => 'comment_count',
    'relationship' => 'none',
  ),
  'new_comments' => array(
    'label' => 'New replies',
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => ' new',
    'link_to_comment' => 1,
    'no_empty' => 1,
    'exclude' => 0,
    'id' => 'new_comments',
    'table' => 'node',
    'field' => 'new_comments',
    'relationship' => 'none',
  ),
  'last_comment_timestamp' => array(
    'label' => 'Last updated',
    'date_format' => 'raw time ago',
    'custom_date_format' => '',
    'exclude' => 0,
    'id' => 'last_comment_timestamp',
    'table' => 'node_comment_statistics',
    'field' => 'last_comment_timestamp',
    'relationship' => 'none',
  ),
  'name' => array(
    'label' => 'Assigned to',
    'link_to_user' => 1,
    'overwrite_anonymous' => 1,
    'anonymous_text' => '',
    'exclude' => 0,
    'id' => 'name',
    'table' => 'users',
    'field' => 'name',
    'relationship' => 'assigned',
  ),
);
if (module_exists('search')) {
  $fields['score'] = array(
    'label' => 'Score',
    'alter' => array(),
    'set_precision' => 1,
    'precision' => '3',
    'decimal' => '.',
    'separator' => ',',
    'prefix' => '',
    'suffix' => '',
    'alternate_sort' => 'last_comment_timestamp',
    'alternate_order' => 'desc',
    'exclude' => 0,
    'id' => 'score',
    'table' => 'search_index',
    'field' => 'score',
    'relationship' => 'none',
  );
}
$handler->override_option('fields', $fields);
$sorts['last_comment_timestamp'] = array(
  'order' => 'DESC',
  'granularity' => 'second',
  'id' => 'last_comment_timestamp',
  'table' => 'node_comment_statistics',
  'field' => 'last_comment_timestamp',
  'relationship' => 'none',
);
if (module_exists('search')) {
  $sorts['score'] = array(
    'order' => 'DESC',
    'id' => 'score',
    'table' => 'search_index',
    'field' => 'score',
    'relationship' => 'none',
  );
}
$handler->override_option('sorts', $sorts);
$filters = array(
  'status_extra' => array(
    'operator' => '=',
    'value' => '',
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'status_extra',
    'table' => 'node',
    'field' => 'status_extra',
    'relationship' => 'none',
  ),
  'pid' => array(
    'operator' => 'in',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'operator' => 'pid_op',
      'label' => 'Project',
      'use_operator' => 0,
      'identifier' => 'projects',
      'optional' => 1,
      'single' => 1,
      'remember' => 0,
      'reduce' => 0,
    ),
    'widget' => 'textfield',
    'project_source' => 'all',
    'project_uid_argument' => '',
    'id' => 'pid',
    'table' => 'project_issues',
    'field' => 'pid',
    'relationship' => 'none',
  ),
  'sid' => array(
    'operator' => 'in',
    'value' => array(
      'Open' => 'Open',
    ),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'sid_op',
      'identifier' => 'status',
      'label' => 'Status',
      'optional' => 1,
      'single' => 1,
      'remember' => 0,
      'reduce' => 0,
    ),
    'id' => 'sid',
    'table' => 'project_issues',
    'field' => 'sid',
    'relationship' => 'none',
  ),
  'priority' => array(
    'operator' => 'in',
    'value' => array(),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'priority_op',
      'identifier' => 'priorities',
      'label' => 'Priority',
      'optional' => 1,
      'single' => 1,
      'remember' => 0,
      'reduce' => 0,
    ),
    'id' => 'priority',
    'table' => 'project_issues',
    'field' => 'priority',
    'relationship' => 'none',
  ),
  'category' => array(
    'operator' => 'in',
    'value' => array(),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'category_op',
      'identifier' => 'categories',
      'label' => 'Category',
      'optional' => 1,
      'single' => 1,
      'remember' => 0,
      'reduce' => 0,
    ),
    'id' => 'category',
    'table' => 'project_issues',
    'field' => 'category',
    'relationship' => 'none',
  ),
);
if (module_exists('search')) {
  $search_filter['keys'] = array(
    'operator' => 'optional',
    'value' => '',
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'keys_op',
      'identifier' => 'text',
      'label' => 'Search for',
      'optional' => 1,
      'remember' => 0,
    ),
    'id' => 'keys',
    'table' => 'search_index',
    'field' => 'keys',
    'relationship' => 'none',
  );
  $filters = $search_filter + $filters;
}
$handler->override_option('filters', $filters);
$handler->override_option('access', array(
  'type' => 'none',
));
$handler->override_option('title', 'Issues for all projects');
$handler->override_option('empty', 'No issues match your criteria.');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 50);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'project_issue_table');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 1,
  'order' => 'desc',
  'columns' => array(
    'project_issue_queue' => 'project_issue_queue',
    'title' => 'title',
    'timestamp' => 'title',
    'sid' => 'sid',
    'priority' => 'priority',
    'category' => 'category',
    'comment_count' => 'comment_count',
    'new_comments' => 'comment_count',
    'last_comment_timestamp' => 'last_comment_timestamp',
    'name' => 'name',
    'score' => 'score',
  ),
  'info' => array(
    'project_issue_queue' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'timestamp' => array(
      'separator' => '',
    ),
    'sid' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'priority' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'category' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'comment_count' => array(
      'sortable' => 1,
      'separator' => '<br />',
    ),
    'new_comments' => array(
      'separator' => '',
    ),
    'last_comment_timestamp' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'score' => array(
      'sortable' => 1,
      'separator' => '',
    ),
  ),
  'default' => module_exists('search') ? 'score' : 'last_comment_timestamp',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'project/issues');
$handler->override_option('menu', array(
  'type' => 'normal',
  'title' => 'Issues',
  'description' => '',
  'weight' => '0',
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
));
$handler = $view->new_display('feed', 'Feed', 'feed_1');
$handler->override_option('sorts', array(
  'last_comment_timestamp' => array(
    'order' => 'DESC',
    'granularity' => 'second',
    'id' => 'last_comment_timestamp',
    'table' => 'node_comment_statistics',
    'field' => 'last_comment_timestamp',
    'override' => array(
      'button' => 'Use default',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('style_plugin', 'rss');
$handler->override_option('style_options', array(
  'mission_description' => FALSE,
  'description' => '',
));
$handler->override_option('row_plugin', 'node_rss');
$handler->override_option('path', 'project/issues/rss');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
  'name' => '',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'description' => '',
  'weight' => 0,
));
$handler->override_option('displays', array(
  'page_1' => 'page_1',
  'default' => 0,
));
$handler->override_option('sitename_title', FALSE);
