<?php
// $Id: issue_node_form.inc,v 1.5 2009/10/08 23:36:16 dww Exp $

/**
 * @file
 * Code required for the issue node form.
 */

/**
 * Redirect node/add/project_issue/* to node/add/project-issue/*.
 */
function project_issue_add_redirect_page($project = NULL, $category = NULL) {
  $path = 'node/add/project-issue';
  if (!empty($project)) {
    $path .= "/$project";
  }
  if (!empty($category)) {
    $path .= "/$category";
  }
  drupal_goto($path);
}

function project_issue_pick_project_page() {
  drupal_set_title(t('Submit @name', array('@name' => node_get_types('name', 'project_issue'))));
  return drupal_get_form('project_issue_pick_project_form');
}

/**
 * Form builder for a simple form to select a project when creating a new
 * issue (as the first "page", but this is not really a multi-page form).
 */
function project_issue_pick_project_form(&$form_state) {
  $form = array();

  // Fetch a list of all projects (nid => title, grouped by project type
  // categories if any), and also get an array of project shortnames.
  $short_names = array();
  $projects = project_projects_select_options($short_names);
  if (empty($projects)) {
    drupal_set_message(t('You do not have access to any projects.'), 'error');
    // No reason to return a project selector form with no elements, so just
    // return an empty array for the form.
    return array();
  }

  // See if there's only one project the user has access to, and if so,
  // redirect directly to the issue form for that project.
  if (count($short_names) == 1) {
    drupal_goto('node/add/project-issue/' . key($short_names));
  }

  // Otherwise, return a form to select which project to submit an issue for.
  $form['pid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#options' => array(t('<none>')) + $projects,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  return $form;
}

function project_issue_pick_project_form_validate($form, &$form_state) {
  if (empty($form_state['values']['pid'])) {
    form_set_error('pid', t('You must select a project.'));
  }
  $node = node_load($form_state['values']['pid']);
  if (empty($node) || $node->type != 'project_project') {
    form_set_error('pid', t('Invalid project selected.'));
  }
}

function project_issue_pick_project_form_submit($form, &$form_state) {
  $project = node_load($form_state['values']['pid']);
  $form_state['redirect'] = 'node/add/project-issue/'. $project->project['uri'];
}

/**
 * Private helper to implement hook_form().
 *
 * Create the project issue node form.
 *
 * @param $node
 *   The project issue node object.
 * @param $include_metadata_fields
 *   If set, metadata fields (eg. status, assigned, title) will
 *   be included in the form regardless of whether $node->nid is set.
 *   Otherwise, metadata fields will only be included in the form
 *   if $node->nid is empty.
 */
function _project_issue_form($node, $form_state, $include_metadata_fields = FALSE) {
  global $user;

  $defaults = array(
    'rid',
    'component',
    'category',
    'priority',
    'assigned',
    'sid',
  );

  // Set some defaults for new forms.
  if (!isset($node->nid)) {
    foreach ($defaults as $default) {
      $node->project_issue[$default] = 0;
    }
  }

  // In the case of an issue preview, get our defaults from the submitted form.
  // TODO: do we want to #tree our form so we don't need this hack?
  if (isset($form_state['node'])) {
    foreach ($defaults as $default) {
      if (isset($form_state['node'][$default])) {
        $node->project_issue[$default] = $form_state['node'][$default];
      }
    }
  }

  // If this function is being called as a result of CCK building the form
  // in content_admin_field_overview_form(), just return an empty array, since
  // CCK only grabs the top level fields and this form has none of those anyway.
  if (!empty($node->cck_dummy_node_form)) {
    return array();
  }

  if (arg(0) == 'node' && arg(1) == 'add') {
    $breadcrumb = array();
    $breadcrumb[] = l(t('Home'), NULL);
    $breadcrumb[] = l(t('Create content'), 'node/add');
    drupal_set_breadcrumb($breadcrumb);
  }

  $default_state = variable_get('project_issue_default_state', 1);

  // Fetch a list of all projects to test for access.
  $uris = NULL;
  $projects = array(t('<none>')) + project_projects_select_options($uris);
  if (count($projects) == 1) {
    drupal_set_message(t('You do not have access to any projects.'), 'error');
    drupal_goto('node/add/project-issue');
    return;
  }

  // Figure out what project we should use for the issue metadata.
  if (!empty($form_state['values']['project_info']['pid'])) {
    // The project has been selected in the form itself (e.g. it's been
    // changed and we're previewing, etc.)
    $pid = $form_state['values']['project_info']['pid'];
  }
  elseif (!empty($node->project_issue['pid'])) {
    // The issue node already knows what project it belongs to.
    $pid = $node->project_issue['pid'];
  }
  else {
    // Fallback and try to learn the project from the URL -- evil.
    $pid = arg(3);
    if (!empty($pid)) {
      if (is_numeric($pid)) {
        $node->project_issue['pid'] = db_result(db_query(db_rewrite_sql('SELECT p.nid FROM {project_projects} p WHERE p.nid = %d', 'p'), $pid));
      }
      else {
        $node->project_issue['pid'] = db_result(db_query(db_rewrite_sql("SELECT p.nid FROM {project_projects} p WHERE p.uri = '%s'", 'p'), $pid));
      }
    }
    $pid = $node->project_issue['pid'];
  }

  if (empty($pid)) {
    drupal_set_message(t('Invalid project selected.'), 'error');
    drupal_goto('node/add/project-issue');
  }

  // If this issue has already been created and is just being
  // edited, we want to prevent any metadata changes.  However, allow
  // the $include_metadata_fields parameter to override this check.
  if ($include_metadata_fields) {
    $allow_metadata_changes = TRUE;
  }
  else {
    $allow_metadata_changes = empty($node->nid);
  }

  // Load the project and initialize some support arrays.
  $project = node_load($pid);
  if ($project->type != 'project_project') {
    drupal_set_message(t('Invalid project selected.'), 'error');
    // Not sure the best place to go here...
    drupal_goto('node/add/project-issue');
  }
  if ($allow_metadata_changes) {
    if (module_exists('project_release') && isset($node->project_issue['rid']) &&
        $releases = project_release_get_releases($project, 0, 'version', 'all', array($node->project_issue['rid']))) {
      $releases = array(t('<none>')) + $releases;
    }
    // Remove releases marked as invalid release nodes for user selection.
    foreach (variable_get('project_issue_invalid_releases', array()) as $rid) {
      unset($releases[$rid]);
    }
    // Setup components and default component.
    if (!empty($node->project_issue['component'])) {
      $default_component = $node->project_issue['component'];
    }
    else {
      $default_component = $project->project_issue['default_component'];
    }
    $components = empty($default_component) ? array(t('<none>')) : array();
    if ($project->project_issue['components']) {
      foreach ($project->project_issue['components'] as $component) {
        $component = check_plain($component);
        $components[$component] = $component;
      }
    }
    
    $categories = array_merge(array(t('<none>')), project_issue_category(0, 0));
    $priorities = project_issue_priority();
    $states = project_issue_state(0, TRUE, !empty($node->nid) && ($node->uid == $user->uid), $node->project_issue['sid']);
    $assigned = project_issue_assigned_choices($node);
  }

  // Display the site-wide and/or per-project help text.
  $site_help = trim(variable_get('project_issue_site_help', ''));
  if (!empty($site_help)) {
    $form['project_help']['site'] = array(
      '#prefix' => '<div class="messages status site">',
      '#value' => check_markup($site_help),
      '#suffix' => '</div>',
    );
  }
  $project_help = trim($project->project_issue['help']);
  if (!empty($project_help)) {
    $form['project_help']['project'] = array(
      '#prefix' => '<div class="messages status project">',
      '#value' => check_markup($project_help),
      '#suffix' => '</div>',
    );
  }

  if ($allow_metadata_changes) {
    $form['project_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Project information'),
      '#prefix' => '<div class="inline-options">',
      '#suffix' => '</div>',
    );
    $form['project_info']['project_display'] = array(
      '#type' => 'item',
      '#title' => t('Project'),
      '#value' => check_plain($project->title),
    );
    $form['project_info']['pid'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['pid'],
    );
    if ($releases) {
      $form['project_info']['rid'] = array(
        '#type' => 'select',
        '#title' => t('Version'),
        '#default_value' => $node->project_issue['rid'],
        '#options' => $releases,
        '#required' => TRUE,
      );
    }
    $form['project_info']['component'] = array(
      '#type' => 'select',
      '#title' => t('Component'),
      '#default_value' => $default_component,
      '#options' => $components,
      '#required' => TRUE,
    );
    $form['issue_info'] = array(
      '#type' => 'fieldset',
      '#title' => t('Issue information'),
      '#prefix' => '<div class="inline-options">',
      '#suffix' => '</div>',
    );
    $form['issue_info']['category'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#default_value' => $node->project_issue['category'] ? $node->project_issue['category'] : arg(4),
      '#options' => $categories,
      '#required' => TRUE,
    );
    $form['issue_info']['priority'] = array(
      '#type' => 'select',
      '#title' => t('Priority'),
      '#default_value' => $node->project_issue['priority'] ? $node->project_issue['priority'] : 2,
      '#options' => $priorities,
    );
    $form['issue_info']['assigned'] = array(
      '#type' => 'select',
      '#title' => t('Assigned'),
      '#default_value' => $node->project_issue['assigned'],
      '#options' => $assigned,
    );
    if (count($states) > 1) {
      $form['issue_info']['sid'] = array(
        '#type' => 'select',
        '#title' => t('Status'),
        '#default_value' => $node->project_issue['sid'] ? $node->project_issue['sid'] : $default_state,
        '#options' => $states,
      );
    }
    else {
      $form['issue_info']['sid'] = array(
        '#type' => 'value',
        '#value' => $default_state,
      );
      $form['issue_info']['status'] = array(
        '#type' => 'item',
        '#title' => t('Status'),
        '#value' => check_plain(project_issue_state($default_state)),
      );
    }
  }
  else {
    // If we're not allowing issue metadata changes, add all of these values
    // into the form so they show up in the $node->project_issue array during
    // validation and submit, so we're consistent with where they live.
    $form['project_issue'] = array('#tree' => TRUE);
    $form['project_issue']['pid'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['pid'],
    );
    if (isset($node->project_issue['rid'])) {
      $form['project_issue']['rid'] = array(
        '#type' => 'value',
        '#value' => $node->project_issue['rid'],
      );
    }
    $form['project_issue']['component'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['component'],
    );
    $form['project_issue']['category'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['category'],
    );
    $form['project_issue']['priority'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['priority'],
    );
    $form['project_issue']['assigned'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['assigned'],
    );
    $form['project_issue']['sid'] = array(
      '#type' => 'value',
      '#value' => $node->project_issue['sid'],
    );
    
      //TBR add categories from db
      $pid = $form['project_info']['pid']['#value'];
      $project = array('nid'=>$pid);
      $categories = project_issue_categories_get_all($project);
      foreach ($categories as  $key=>$value) {
        $form['issue_info']['category']['#options'][$value['title']] = $value['title'];
      }
    
    
  }

  $form['issue_details'] = array(
    '#type' => 'fieldset',
    '#title' => t('Issue details'),
    '#prefix' => '</div><div class="standard">',
  );

  if ($allow_metadata_changes) {
    $form['issue_details']['title'] = array(
      '#type' => 'textfield',
      '#title' => t('Title'),
      '#default_value' => $node->title,
      '#size' => 60,
      '#maxlength' => 128,
      '#required' => TRUE,
    );
  }
  else {
    $form['issue_details']['title'] = array(
      '#type' => 'value',
      '#value' => $node->title,
    );
  }

  $form['issue_details']['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $node->body,
    '#rows' => 10,
    '#required' => TRUE,
  );
  $form['issue_details']['format'] = filter_form($node->format);

  $directory = file_create_path(variable_get('project_directory_issues', 'issues'));
  if (!file_check_directory($directory, 0)) {
    $msg = t('File attachments are disabled. The issue directory has not been properly configured.');
    if (user_access('administer site configuration')) {
      $msg .= ' '. t('Please visit the !admin-project-issue-settings page.', array('!admin-project-issue-settings' => l(t('Project issue settings'), 'admin/project/project-issue-settings')));
    }
    else {
      $msg .= ' '. t('Please contact the site administrator.');
    }
    drupal_set_message($msg, 'error');
  }
  
 
  
  return $form;
}

/**
 * Private helper to implement hook_validate().
 *
 * Ensures that the issue node form has valid values for all required fields.
 * We use hook_validate() here instead of a #validate handler or even defining
 * project_issue_node_form_validate() since if we did, node_form_validate()
 * itself would not be invoked, which would lead to all kinds of problems,
 * including hook_nodeapi('validate') never being invoked.
 *
 * @param $node
 *   An object of form values from the project_issue node form, not a fully
 *   loaded issue node object.  Therefore, the fields are not in the usual
 *   $node->project_issue array.
 */
function _project_issue_validate($node) {
  // If $node->nid is set, that means that the node was being
  // edited and not created.  If that's the case, the user was
  // not presented with any of the metadata fields, so there's no
  // need to validate them here.
  if (empty($node->nid)) {
    if (empty($node->pid)) {
      form_set_error('pid', t('You have to specify a valid project.'));
    }
    elseif ($project = node_load($node->pid)) {
      if (module_exists('project_release') &&
          $releases = project_release_get_releases($project, 0)) {
        if (empty($node->rid)) {
          form_set_error('rid', t('You have to specify a valid version.'));
        }
      }
      if (isset($node->component) && !in_array($node->component, $project->project_issue['components'])) {
        $node->component = 0;
      }
      if (empty($node->component)) {
        form_set_error('component', t('You have to specify a valid component.'));
      }
      if (empty($node->category)) {
        form_set_error('category', t('You have to specify a valid category.'));
      }
    }
  }
}

/**
 * Private helper to implement hook_insert().
 *
 * @param $node
 *   Object containing form values from the project_issue node form.  This is
 *   NOT a fully loaded $node object, so the issue-related values are directly
 *   in $node, not in the $node->project_issue array.
 */
function _project_issue_insert($node) {
  // Permanently store the original issue states in a serialized array. This
  // is a bit yucky, but we need them for proper handling of states workflow.
  // The current states need to be stored in {project_issues} as well for
  // query efficiency in issue queue searches, and it seems too messy to add a
  // bunch of new columns to the {project_issues} table for the original
  // states.
  $original_issue_data = new stdClass();
  $fields = array(
    'pid' => 0,
    'rid' => 0,
    'component' => '',
    'category' => '',
    'priority' => 0,
    'assigned' => 0,
    'sid' => 0,
    'title' => '',
  );
  foreach ($fields as $field => $default) {
    // Some of the incoming data may not have the correct default.
    if (empty($node->$field)) {
      $node->$field = $default;
    }
    $original_issue_data->$field = $node->$field;
  }

  db_query("INSERT INTO {project_issues} (nid, pid, category, component, priority, rid, assigned, sid, original_issue_data, last_comment_id, db_lock) VALUES (%d, %d, '%s', '%s', %d, %d, %d, %d, '%s', %d, %d)", $node->nid, $node->pid, $node->category, $node->component, $node->priority, $node->rid, $node->assigned, $node->sid, serialize($original_issue_data), 0, 0);

  // Invalidate the "Issue cockpit" block cache for this project, since the
  // new issue will have altered the summary totals.
  cache_clear_all('project_issue_cockpit_block:'. $node->pid, 'cache');
}

