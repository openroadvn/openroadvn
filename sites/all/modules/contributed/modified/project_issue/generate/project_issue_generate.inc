<?php
// $Id: project_issue_generate.inc,v 1.13 2009/03/21 18:50:17 thehunmonkgroup Exp $

// If not in 'safe mode', increase the maximum execution time.
if (!ini_get('safe_mode')) {
  set_time_limit(240);
}

/**
 * Generate some random project issues.
 *
 * @param $number
 *  Number of issues to generate.
 */
function project_issue_generate_issues($number) {
  module_load_include('inc', 'devel', 'devel_generate');
  module_load_include('inc', 'node', 'node.pages');

  $empty_variables = array();
  // After retrieving the possible values for each of these variables, we
  // need to check and make sure that at least one value was returned. If
  // not, we're likely to get fatal errors later on in this function.
  $projects = _project_issue_generate_get_field('projects');
  if (empty($projects)) { $empty_variables[] = 'projects'; }
  $categories = _project_issue_generate_get_field('categories');
  if (empty($categories)) { $empty_variables[] = 'categories'; }
  $priorities = _project_issue_generate_get_field('priorities');
  if (empty($priorities)) { $empty_variables[] = 'priorities'; }
  $users = _project_issue_generate_get_field('users');
  if (empty($users)) { $empty_variables[] = 'users'; }
  $loaded_users = array();

  // Return now if any of the above variables were empty.
  if (!empty($empty_variables)) {
    drupal_set_message(t('No values for %variables could be found to use with automatically generated issues, so no issues were created.', array('%variables' => implode(', ', $empty_variables))), 'error');
    return;
  }

  srand((double) microtime() * 1000000);
  for ($i = 0; $i < $number; $i++) {
    $project = $projects[array_rand($projects)];
    $project_node = node_load($project->nid);
    $components = unserialize($project->components);
    $releases = project_release_get_releases($project_node, FALSE);
    $issue = new stdClass();
    $issue->pid = $project->nid;
    $issue->category = $categories[array_rand($categories)];
    $issue->component = $components[array_rand($components)];
    $issue->priority = array_rand($priorities);
    $issue->title = devel_create_greeking(rand(2, 15), true);
    $issue->body = devel_create_content();
    $issue->rid = array_rand($releases);

    // The user must be chosen before status (sid) so that we can make sure
    // that the status is set such that the user would have permission to
    // set the status as such.
    $account = user_load(array('uid' => $users[array_rand($users)]->uid));
    $issue->name = $account->uid;
    $issue->sid = array_rand(_project_issue_generate_get_permitted_sids($account));
    if (!isset($issue->sid)) {
      unset($issue);
      continue;
    }

    // Currently, project_issue nodes can either be unassigned or assigned
    // to the user creating the project_issue node (or in the case of comments
    // the user creating the comment).
    $possible_assignments = array(0, $account->uid);
    $issue->assigned = $possible_assignments[array_rand($possible_assignments)];
    $issue->type = 'project_issue';
    $issue->comment = COMMENT_NODE_READ_WRITE;

    node_save($issue);
  }

  drupal_set_message(format_plural($number, 'Your issue has been created.', '@count issues have been created.'));
}

/**
 * Generate some random project issues.
 *
 * @param $number
 *  Number of comments to generate.
 */
function project_issue_generate_issue_comments($number) {

  $empty_variables = array();
  // After retrieving the possible values for each of these variables, we
  // need to check and make sure that at least one value was returned. If
  // not, we're likely to get fatal errors later on in this function.
  $issues = _project_issue_generate_get_field('issues');
  if (empty($issues)) { $empty_variables[] = 'issues'; }
  $projects = _project_issue_generate_get_field('projects');
  if (empty($projects)) { $empty_variables[] = 'projects'; }
  $categories = _project_issue_generate_get_field('categories');
  if (empty($categories)) { $empty_variables[] = 'categories'; }
  $priorities = _project_issue_generate_get_field('priorities');
  if (empty($priorities)) { $empty_variables[] = 'priorities'; }
  $users = _project_issue_generate_get_field('users');
  if (empty($users)) { $empty_variables[] = 'users'; }

  // Return now if any of the above variables were empty.
  if (!empty($empty_variables)) {
    drupal_set_message(t('No values for %variables could be found to use with automatically generated comments, so no issue comments were created.', array('%variables' => implode(', ', $empty_variables))), 'error');
    return;
  }

  srand((double) microtime() * 1000000);

  for ($i = 0; $i < $number; $i++) {
    // Select a random issue to add a comment to.
    $k = array_rand($issues);

    // Update our copy of the issue to randomly decide what to change with
    // the new comment. This way, if we decide not to alter something, we
    // can leave the current value alone.
    $account = user_load(array('uid' => $users[array_rand($users)]->uid));
    $project = $projects[array_rand($projects)];
    if (rand(0, 1)) {
      $issues[$k]->pid = $project->nid;
      // If the project changes, the version must also change.
      $project_node = node_load($issues[$k]->pid, NULL, TRUE);
      $releases = project_release_get_releases($project_node, FALSE);
      $issues[$k]->rid = array_rand($releases);

      // And so must the component.
      $components = $project_node->project_issue['components'];
      $issues[$k]->component = $components[array_rand($components)];
    }
    elseif (rand(0, 1)) {
      $components = unserialize($project->components);
      $issues[$k]->component = $components[array_rand($components)];
    }
    if (rand(0, 1)) {
      $issues[$k]->category = $categories[array_rand($categories)];
    }
    if (rand(0, 1)) {
      $issues[$k]->priority = array_rand($priorities);
    }
    if (rand(0, 1)) {
      $issues[$k]->title = devel_create_greeking(rand(2, 10), true);
    }
    if (rand(0, 1)) {
      $issues[$k]->sid = array_rand(_project_issue_generate_get_permitted_sids($account));
      if (!isset($issues[$k]->sid)) {
        unset($comment);
        continue;
      }
    }
    if (rand(0, 1)) {
      $project_node = node_load($issues[$k]->pid, NULL, TRUE);
      $releases = project_release_get_releases($project_node, FALSE);
      $issues[$k]->rid = array_rand($releases);
    }
    // Construct a comment to put the issue into the state we just decided.
    $comment = array();
    $comment['uid'] = $account->uid;
    $comment['project_info'] = array('pid' => $issues[$k]->pid, 'component' => $issues[$k]->component, 'rid' => $issues[$k]->rid);
    $comment['category'] = $issues[$k]->category;
    $comment['priority'] = $issues[$k]->priority;
    $comment['title'] = $issues[$k]->title;
    $comment['sid'] = $issues[$k]->sid;
    if (rand(0, 1)) {
      $comment['comment'] = devel_create_content();
    }
    else {
      $comment['comment'] = '';
    }
    $comment['format'] = 1;
    $comment['cid'] = '';
    $comment['nid'] = $issues[$k]->nid;

    project_issue_add_followup($comment);
  }
  drupal_set_message(format_plural($number, 'Your comment has been created.', '@count comments have been created.'));
}

function _project_issue_generate_get_field($field, $pool_size = 100) {
  module_load_include('inc', 'project_issue', 'issue');

  switch ($field) {
    case 'issues':
      $issues = array();
      $results = db_query('SELECT p.nid, p.pid, p.category, p.component, p.priority, p.sid, p.rid, n.title FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid');
      while ($result = db_fetch_object($results)) {
        $issues[] = $result;
      }
      return $issues;

    case 'projects':
      $projects = array();
      $result = db_query('SELECT nid, components FROM {project_issue_projects}');
      while ($project = db_fetch_object($result)) {
        $projects[] = $project;
      }
      return $projects;

    case 'categories':
      return array_keys(project_issue_category());

    case 'priorities':
      return project_issue_priority();

    case 'users':
      // Determine what role ids have permission to create project_issue nodes.
      $users = array();
      $allowed_roles = user_roles(FALSE, 'create project issues');

      // If any authenticated user can create project_issue nodes,
      // then there is no need for an INNER JOIN in our query.
      // Otherwise, the query needs to INNER JOIN on the users
      // table so that only users with roles that are allowed to
      // create project_issue nodes are selected.
      if (isset($allowed_roles[DRUPAL_AUTHENTICATED_RID])) {
        $join = '';
        $where = '';
      }
      else {
        $join = 'INNER JOIN {users_roles} ur ON u.uid = ur.uid';
        $where = "WHERE ur.rid IN (". implode(', ', array_keys($allowed_roles)) .")";
      }
      $sql = "SELECT u.uid FROM {users} u $join $where ORDER BY RAND() LIMIT %d";
      $result = db_query($sql, $pool_size);
      while ($user = db_fetch_object($result)) {
        $users[] = $user;
      }
      return $users;
  }
}

function _project_issue_generate_get_permitted_sids($user) {
  static $permitted_states;

  if (!isset($permitted_states)) {
    $permitted_states = array();
  }

  if (isset($user->uid)) {
    if (isset($permitted_states[$user->uid])) {
      return $permitted_states[$user->uid];
    }
    else {
      $states = project_issue_state($sid = 0, TRUE, TRUE, 0, FALSE, $user);
      $permitted_states[$user->uid] = $states;
      return $states;
    }
  }
  return array();
}
