<?php
// $Id: issue_cockpit.inc,v 1.6 2009/05/06 23:43:34 dww Exp $

/**
 * Form builder for the search box in the issue cockpit block.
 */
function project_issue_issue_cockpit_searchbox(&$form_state, $node) {
  $search_path = 'project/issues/' . $node->project['uri'];
  $adv_search_path = 'project/issues/search/' . $node->project['uri'];
  return array(
    '#action' => url($search_path),
    '#method' => 'get',
    'text' => array(
      '#type' => 'textfield',
      '#size' => 20,
    ),
    'status' => array(
      '#type' => 'hidden',
      '#value' => 'All',
    ),
    'submit' => array(
      '#type' => 'submit',
      '#value' => t('Search'),
      '#name' => '',
    ),
    'advanced' => array(
      '#value' => l(t('Advanced search'), $adv_search_path),
    ),
  );
}

/**
 * Theme template preprocess to declare variables for the issue cockpit.
 */
function template_preprocess_project_issue_issue_cockpit(&$vars) {
  $node = $vars['node'];
  // Flags that indicate what kind of access to project issues to allow.
  $has_issues = !empty($node->project_issue['issues']);
  $vars['view_issues'] = $has_issues && (user_access('access project issues') || user_access('access own project issues') || user_access('administer projects'));
  $vars['make_issues'] = $has_issues && node_access('create', 'project_issue');

  if ($vars['view_issues']) {
    $open_states = '('. implode(',', array_keys(project_issue_state(0, FALSE, FALSE, 0, TRUE))) .')';

    $vars['path'] = 'project/issues/'. $node->project['uri'];
    $vars['form'] = drupal_get_form('project_issue_issue_cockpit_searchbox', $node);

    $vars['categories'] = array();
    $categories = array('All' => t('All issues')) + project_issue_category(0,0);
    $setting = variable_get('project_issue_cockpit_categories', array('All' => 'All', 'bug' => 'bug'));
    $active_categories = array_intersect_key($categories, array_filter($setting));
    foreach ($active_categories as $category => $name) {
      $vars['categories'][$category] = array('name' => $name, 'open' => 0, 'total' => 0);
    }

    if (isset($vars['categories']['All'])) {
      $vars['categories']['All']['open'] = db_result(db_query(db_rewrite_sql('SELECT count(*) FROM {node} n INNER JOIN {project_issues} i ON n.nid = i.nid WHERE n.status = 1 AND i.pid = %d AND i.sid IN '. $open_states), $node->nid));
      $vars['categories']['All']['total'] = db_result(db_query(db_rewrite_sql('SELECT count(*) FROM {node} n INNER JOIN {project_issues} i ON n.nid = i.nid WHERE n.status = 1 AND i.pid = %d'), $node->nid));
    }

    $result = db_query(db_rewrite_sql('SELECT count(*) AS open, i.category FROM {node} n INNER JOIN {project_issues} i ON n.nid = i.nid WHERE n.status = 1 AND i.pid = %d AND i.sid IN '. $open_states .' GROUP BY i.category'), $node->nid);
    while ($category = db_fetch_object($result)) {
      if (isset($vars['categories'][$category->category])) {
        $vars['categories'][$category->category]['open'] = $category->open;
      }
    }

    $result = db_query(db_rewrite_sql('SELECT count(*) AS total, i.category FROM {node} n INNER JOIN {project_issues} i ON n.nid = i.nid WHERE n.status = 1 AND i.pid = %d GROUP BY i.category'), $node->nid);
    while ($category = db_fetch_object($result)) {
      if (isset($vars['categories'][$category->category])) {
        $vars['categories'][$category->category]['total'] = $category->total;
      }
    }

    $vars['issue_subscribe'] = l(t('Subscribe via e-mail'), 'project/issues/subscribe-mail/'. $node->project['uri'], array('query' => drupal_get_destination()));
    $vars['issue_statistics'] = l(t('Issue statistics'), 'project/issues/statistics/'. $node->project['uri']);
    $vars['oldest'] = db_result(db_query(db_rewrite_sql('SELECT min(n.created) FROM {node} n INNER JOIN {project_issues} i ON n.nid = i.nid WHERE n.status = 1 AND i.pid = %d AND i.sid IN '. $open_states), $node->nid));
  }
}

