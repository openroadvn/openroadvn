<?php
// $Id: project_node.inc,v 1.1 2009/06/18 03:30:22 dww Exp $

/**
 * hook_nodeapi implementation specific to "project_project" nodes
 * (from the project.module)
 * @see project_issue_nodeapi().
 */
function project_issue_project_nodeapi(&$node, $op, $arg) {
  switch ($op) {
    case 'load':
      project_issue_project_load($node, $arg);
      break;

    case 'insert':
      project_issue_project_insert($node);
      break;

    case 'delete':
      project_issue_project_delete($node);
      break;
  }
}

function project_issue_project_load(&$node, $arg) {
  $project = db_fetch_object(db_query('SELECT * FROM {project_issue_projects} WHERE nid = %d', $node->nid));
  // If nothing loaded, bail immediately
  if (!$project) {
    return $node;
  }
  $node->project_issue = array();
  $node->project_issue['components'] = unserialize($project->components);
  $node->project_issue['default_component'] = $project->default_component;
  $node->project_issue['mail_copy_filter'] = unserialize($project->mail_copy_filter);
  $node->project_issue['mail_copy_filter_state'] = unserialize($project->mail_copy_filter_state);
  foreach (array('issues', 'help', 'mail_digest', 'mail_copy', 'mail_reminder') as $field) {
    $node->project_issue[$field] = $project->$field;
  }
  return $node;
}

function project_issue_project_insert($node) {
  // Setup reasonable defaults: enable issue tracker and standard
  // components. If maintainers want to change these, (or any of the
  // other issue-related project settings, like the email notifications)
  // they'd use the "issues" subtab of the project edit tab (since none
  // of these fields are present on the initial project add form).
  $node->project_issue['issues'] = 1;

  // Default components are saved in the variables table as a plain
  // string instead of as a serialized array since they come out of
  // a textarea on a system settings form.  Convert here to the more
  // desirable serialized array for storage per-project.
  $defaults = array(t('Code'), t('Documentation'), t('Miscellaneous'), t('User interface'));
  $components = variable_get('project_issue_default_components', implode("\n", $defaults));  
  $default_components = explode("\n", str_replace("\r", '', $components));

  $node->project_issue['components'] = serialize($default_components);
  db_query("INSERT INTO {project_issue_projects} (nid, issues, components) VALUES (%d, %d, '%s')", $node->nid, $node->project_issue['issues'], $node->project_issue['components']);
}

function project_issue_project_delete($node) {
  // Delete all associated issues.
  $issues = db_query('SELECT nid FROM {project_issues} WHERE pid = %d', $node->nid);
  while ($issue = db_fetch_object($issues)) {
    node_delete($issue->nid);
  }
  db_query('DELETE FROM {project_issue_projects} WHERE nid = %d', $node->nid);
  db_query('DELETE FROM {project_subscriptions} WHERE nid = %d', $node->nid);
}

