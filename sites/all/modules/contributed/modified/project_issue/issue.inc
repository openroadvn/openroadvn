<?php
// $Id: issue.inc,v 1.354 2009/06/18 03:28:55 dww Exp $


/**
 * JS callback method to return updated elements on the issue form.
 * This function is called when someone changes the "Project" selector.
 */
function project_issue_update_project() {
  $form_state = array('storage' => NULL, 'submitted' => FALSE, 'rebuild' => TRUE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form_state['post'] = $form['#post'] = $_POST;
  $form['#programmed'] = $form['#redirect'] = FALSE;

  drupal_process_form($form_id, $form, $form_state);

  // Rebuild the form and cache it again.
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);

  // These used to be path arguments. Now they arrive via form element.
  $issue_nid = $form['nid']['#value'];
  $pid = $form_state['values']['project_info']['pid'];
  $rid = isset($form_state['values']['project_info']['rid']) ? $form_state['values']['project_info']['rid'] : 0;
  $cid = $form_state['values']['cid'];
  $assigned_uid = $form_state['values']['project_info']['assigned'];

  $project_info = $form['original_issue']['project_info'];

  // Only generate release stuff if the project_release module is enabled.
  if (module_exists('project_release')) {

    $project->nid = $pid;
    if ($releases = project_release_get_releases($project, 0)) {
      $old_version = db_result(db_query("SELECT version FROM {project_release_nodes} WHERE nid = %d", $rid));
      $releases = array(t('<none>')) + $releases;
      // Since the value of releases is by nid, try to match release
      // based on the label instead.
      $default = 0;
      foreach ($releases as $key => $label) {
        if ($old_version == $label) {
          $default = $key;
        }
      }
      // Element is tree'd here to match the original form layout.
      $project_info['#tree'] = TRUE;
      $project_info['rid']['#options'] = $releases;
      $project_info['rid']['#disabled'] = FALSE;
      if (!empty($default)) {
        $project_info['rid']['#value'] = $default;
        $project_info['rid']['#default_value'] = $default;
      }
    }
    else {
      $project_info['rid']['#disabled'] = TRUE;
      $project_info['rid']['#options'] = array();
    }
  }

  // Assigned.
  if (is_numeric($issue_nid)) {
    $issue = node_load(array('nid' => $issue_nid, 'type' => 'project_issue'));
  }
  else {
    // @TODO:  This case should not ever be hit until
    // http://drupal.org/node/197281 lands.
    $issue = new stdClass;
  }
  $issue->project_issue['pid'] = $pid;
  $assigned_choices = project_issue_assigned_choices($issue);

  $project_info['assigned']['#default_value'] = $assigned_uid;
  $project_info['assigned']['#options'] = $assigned_choices;

  // Components.
  $project = db_fetch_object(db_query('SELECT * FROM {project_issue_projects} WHERE nid = %d', $pid));
  $components = array();
  if ($project->components) {
    $components = array(t('<none>'));
    foreach (unserialize($project->components) as $component) {
      $component = check_plain($component);
      $components[$component] = $component;
    }
  }

  $project_info['component']['#default_value'] = $cid;
  $project_info['component']['#options'] = $components;

  // Build the HTML output for the component select.
  $output = theme('status_messages') . drupal_render($project_info);
  drupal_json(array('status' => TRUE, 'data' => $output));
  exit;
}

/**
 * Build an array of users to whom an issue may be assigned.
 *
 * @param $issue
 *   The fully loaded issue node object.
 * @return
 *   A keyed array in the form uid => name containing users
 *   to whom an issue may be assigned.
 */
function project_issue_assigned_choices($issue) {
  // Setup the array of choices for who the issue is assigned to.
  $assigned = array();
  foreach (module_implements('project_issue_assignees') as $module) {
    $function = "{$module}_project_issue_assignees";
    $function($assigned, $issue);
  }
  natcasesort($assigned);
  $assigned = array(0 => empty($issue->project_issue['assigned']) ? t('Unassigned') : t('Unassign')) + $assigned;
  return $assigned;
}

/**
 * Implementation of hook_project_issue_assignees().
 *
 * This hook is used to modify the list of 'Assigned' users when creating or
 * commenting on an issue.
 *
 * @param $assigned
 *  An array of key => value pairs in the format of uid => name that represents
 *  the list of users that the issue may be assigned to. This parameter
 *  is passed by reference so the hook may make changes, such as adding or
 *  removing elements.
 * @param $node
 *  The node object for the issue.
 */
function project_issue_project_issue_assignees(&$assigned, $node) {
  global $user;
  if ($user->uid) {
    if (isset($node->project_issue['assigned']) && $user->uid != $node->project_issue['assigned']) {
      // Assigned to someone else, add the currently assigned user.
      $account = user_load(array('uid' => $node->project_issue['assigned']));
      $assigned[$node->project_issue['assigned']] = $account->name;
    }
    // Always let the person replying assign it to themselves.
    $assigned[$user->uid] = $user->name;
  }

  if (user_access('assign and be assigned project issues')) {
    // All users are included if either anon or auth user has the perm.
    if (db_result(db_query("SELECT rid FROM {permission} WHERE perm LIKE '%%%s%%' AND rid IN(%d, %d)", 'assign and be assigned project issues', DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID))) {
      $result = db_query("SELECT uid, name FROM {users}");
    }
    else {
      $result = db_query("SELECT u.uid, u.name FROM {users} u INNER JOIN {users_roles} ur ON u.uid = ur.uid INNER JOIN {role} r ON ur.rid = r.rid INNER JOIN {permission} p ON p.rid = r.rid WHERE p.perm LIKE '%%%s%%'", 'assign and be assigned project issues');
    }

    while ($assignee = db_fetch_object($result)) {
      $assigned[$assignee->uid] = $assignee->name;
    }
  }
}

// Support stuff

/**
 * Return information about project issue state values.
 *
 * @param $sid
 *   Integer state id to return information about, or 0 for all states.
 * @param $restrict
 *   Boolean to determine if states should be restricted based on the
 *   permissions of the current user or $account if that parameter
 *   is provided.
 * @param $is_author
 *   Boolean that indicates if the current user is the author of the
 *   issue, which can potentially grant a wider selection of states.
 * @param $current_sid
 *   The current integer state id for the issue.
 * @param $defaults
 *   Boolean to request the states used for default issue queries.
 * @param $account
 *   Account of a user to pass to user_access() for access checking.
 *   This parameter will have no effect unless $restrict is also
 *   set to TRUE.
 *
 * @return
 *   An array of states (sid as key, name as value) that match the
 *   given filters, or the name of the requested state.
 *   NOTE: The state name returned is raw text, and should be filtered via
 *   check_plain() or filter_xss() before being printed as HTML output.
 */
function project_issue_state($sid = 0, $restrict = false, $is_author = false, $current_sid = 0, $defaults = false, $account = NULL) {
  static $options;

  if (!$options) {
    $result = db_query('SELECT * FROM {project_issue_state} ORDER BY weight');
    while ($state = db_fetch_object($result)) {
      $options[] = $state;
    }
  }

  foreach($options as $state) {
    if ($restrict) {
      // Check if user has access,
      // or if status is default status and therefore available to all,
      // or if user is original issue author and author has access,
      // or if the issue is already in a state, even if the user doesn't have
      //   access to that state themselves.
      if (user_access('set issue status '. str_replace("'", "", $state->name), $account)
          || user_access('administer projects', $account)
          || ($state->sid == variable_get('project_issue_default_state', 1))
          || ($state->author_has && $is_author)
          || ($state->sid == $current_sid) ) {
        $states[$state->sid] = $state->name;
      }
    }
    else if ($defaults) {
      if ($state->default_query) {
        $states[$state->sid] = $state->name;
      }
    }
    else {
      $states[$state->sid] = $state->name;
    }
  }

  return $sid ? $states[$sid] : $states;
}

/**
 * Return an array of state ids that should be used for default queries.
 */
function project_issue_default_states() {
  static $defaults;
  if (empty($defaults)) {
    $states = project_issue_state(0, false, false, 0, true);
    $defaults = !empty($states) ? array_keys($states) : array(1, 2, 4);
  }
  return $defaults;
}

function project_issue_priority($priority = 0) {
  //GKN
  //$priorities = array(1 => t('critical'), t('normal'), t('minor'));
  //Blocker Critical Major Normal Minor Trivial
  $priorities = array(1 => t('Blocker'), t('Critical'), t('Major'),t('Normal'), t('Minor'), t('Trivial'));
  return $priority ? $priorities[$priority] : $priorities;
}

function project_issue_category($category = 0, $plural = 1,$pid = NULL) {    

  if ($plural) {
    //GKN
    //$categories = array('bug' => t('bug reports'), 'task' => t('tasks'), 'feature' => t('feature requests'), 'support' => t('support requests'));
    $categories = array('bug' => t('Bugs'), 'task' => t('Tasks'), 'improvement' => t('Improvements'), 'feature' => t('Features'), 'support' => t('Supports'), 'Asset Development Assistant'=>t('Assets Development Assistants'));
  }
  else {
    //GKN
    //$categories = array('bug' => t('bug report'), 'task' => t('task'), 'feature' => t('feature request'), 'support' => t('support request'));
    $categories = array('bug' => t('Bug'), 'task' => t('Task'), 'improvement' => t('Improvement'), 'feature' => t('Feature'), 'support' => t('Support'),'Asset Development Assistant'=>t('Asset Development Assistant'));
  }  
  //TBR
  //check if category exists in db and return it 
  $project = array('nid'=>$pid); 
  $categories_from_db = project_issue_categories_get_all($project);   
  if(NULL!==$pid && sizeof($categories_from_db)>0)
  {    
    if($category==$categories_from_db[0]['title'])
      return $category;    
    else
    {
      foreach ($categories_from_db as $value) {
        $categories[$value['title']] = $value['title'];
      }
    }
  }
  return $category ? $categories[$category] : $categories;
  
}
function project_issue_count($pid) {
  $state = array();
  $result = db_query('SELECT p.sid, count(p.sid) AS count FROM {node} n INNER JOIN {project_issues} p ON n.nid = p.nid WHERE n.status = 1 AND p.pid = %d GROUP BY p.sid', $pid);
  while ($data = db_fetch_object($result)) {
    $state[$data->sid] = $data->count;
  }
  return $state;
}

/**
 * Return the HTML to use for the links at the top of issue query pages.
 *
 * @param $links
 *   Array of links, indexed by the action ('create', 'search', etc).
 *
 * @see project_issue_query_result()
 * @see theme_links()
 */
function theme_project_issue_query_result_links($links) {
  return theme('links', $links);
}

/**
 * Provide an array of project issue summary fields.
 * @param $context
 *   The context in which the fields will be displayed.
 *     'web' for project_issue node and comment summary tables.
 *     'email' for project_issue notification e-mail messages.
 */
function project_issue_field_labels($context) {

  if ($context == 'web') {
    $labels = array('title' => t('Title'));
  }
  else {
    $labels = array();
  }

  $labels += array(
    'pid' => t('Project'),
    'rid' => t('Version'),
    'component' => t('Component'),
    'category' => t('Category'),
    'priority' => t('Priority'),
    'assigned' => t('Assigned to'),
    'sid' => t('Status'),
  );

  if ($context == 'email') {
    $labels +=  array(
      'name' => t('Reported by'),
      'updator' => t('Updated by'),
    );
  }
  return $labels;
}

/**
 * Provide the text displayed to a user for a specific metadata field.
 *
 * @param $field
 *   Name of the field.
 * @param $value
 *   Value of the field.
 *
 * @return
 *   Text to display to user. NOTE: This is unfiltered text! If this output is
 *   being sent over the web, you must use check_plain().
 */
function project_issue_change_summary($field, $value) {
  switch ($field) {
    case 'pid':
      $project = node_load(array('nid' => $value, 'type' => 'project_project'));
      return $project->title;
    case 'category':
      $pid = variable_get("current_group", NULL);
      return $value ? project_issue_category($value, 0,$pid) : t('<none>');
    case 'priority':
      return $value ? project_issue_priority($value) : t('<none>');
    case 'rid':
      if ($value) {
        $release->nid = $value;
        if (module_exists('project_release')) {
          $release = project_release_load($release);
        }
        else {
          $release->project_release['version'] = t('Unknown');
        }
        return $release->project_release['version'];
      }
      return t('<none>');
    case 'assigned':
      $user = user_load(array('uid' => $value));
      return (int) $value === 0 ? variable_get('anonymous', t('Anonymous')) : $user->name;
    case 'sid':
      return $value ? project_issue_state($value) : t('<none>');
    default:
      return $value;
  }
}

function theme_project_issue_create_forbidden($uri = '') {
  global $user;
  if ($user->uid) {
    return '';
  }

  // We cannot use drupal_get_destination() because these links sometimes appear on /node and taxo listing pages.
  $destination = "destination=". drupal_urlencode("node/add/project-issue/$uri");

  if (variable_get('user_register', 1)) {
    return t('<a href="@login">Login</a> or <a href="@register">register</a> to create an issue', array('@login' => url('user/login', array('query' => $destination)), '@register' => url('user/register', array('query' => $destination))));
  }
  else {
    return t('<a href="@login">Login</a> to create an issue', array('@login' => url('user/login', array('query' => $destination))));
  }
}

/**
 * return the list of category for a issue by user and by project
 * @param array $project A array containing nid  ie: array('nid'=>value)
 * @return array a list of category 
 */
function project_issue_categories_get_all($project) {
  //$query = db_query("SELECT  title,nid, uid,icid   FROM {project_issue_categories} where uid = %d and nid = %d",$project['uid'],$project['nid']);
  $query = db_query("SELECT  title,nid, uid,icid   FROM {project_issue_categories} where  nid = %d",$project['nid']);
  $data = array();
  while ($row = db_fetch_array($query)) $data[] = $row;
  return $data;
}
