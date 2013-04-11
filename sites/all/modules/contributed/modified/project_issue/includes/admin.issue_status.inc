<?php
// $Id: admin.issue_status.inc,v 1.1 2009/04/04 07:03:28 dww Exp $

/**
 * @file
 * Code for the issue status admin configuration form.
 */

function theme_project_issue_admin_states_form($form) {
  drupal_add_tabledrag('project-issue-status-admin-table', 'order', 'self', 'project-issue-status-weight');
  $header = array(
    array('data' => t('ID')),
    array('data' => t('Name')),
    array('data' => t('Weight')),
    array('data' => t('Author may set')),
    array('data' => t('In default queries')),
    array('data' => t('Default status')),
    array('data' => t('Operations'))
  );
  foreach (element_children($form['status']) as $key) {
    $rows[] = array(
      'class' => 'draggable',
      'data' => array(
        drupal_render($form['status'][$key]['sid']),
        drupal_render($form['status'][$key]['name']),
        drupal_render($form['status'][$key]['weight']),
        drupal_render($form['status'][$key]['author_has']),
        drupal_render($form['status'][$key]['default_query']),
        drupal_render($form['default_state'][$key]),
        drupal_render($form['delete'][$key]),
      ),
    );
  }
  $rows[] = array(
    'class' => 'draggable',
    'data' => array(
      NULL,
      drupal_render($form['status_add']['name']),
      drupal_render($form['status_add']['weight']),
      drupal_render($form['status_add']['author_has']),
      drupal_render($form['status_add']['default_query']),
      NULL, NULL,
    ),
  );
  $output = '<div>' . theme('table', $header, $rows, array('id' => 'project-issue-status-admin-table')) . '</div>';
  $output .= drupal_render($form);
  return $output;
}

function project_issue_admin_states_form(&$form_state) {
  $result = db_query('SELECT * FROM {project_issue_state} ORDER BY weight');
  $default_state = variable_get('project_issue_default_state', 1);
  $default_states = project_issue_default_states();
  $form['status']['#tree'] = TRUE;
  while ($state = db_fetch_object($result)) {
    $options[$state->sid] = '';
    $form['status'][$state->sid]['sid'] = array(
      '#type' => 'markup',
      '#value' => $state->sid,
    );
    $form['status'][$state->sid]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => $state->name,
      '#size' => 20,
      '#maxlength' => 255,
    );
    $form['status'][$state->sid]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $state->weight,
      '#delta' => 15,
      '#attributes' => array('class' => 'project-issue-status-weight'),
    );
    $form['status'][$state->sid]['author_has'] = array(
      '#type' => 'checkbox',
      '#default_value' => $state->author_has,
    );
    $form['status'][$state->sid]['default_query'] = array(
      '#type' => 'checkbox',
      '#default_value' => in_array($state->sid, $default_states),
    );
    $del_link = ($state->sid != $default_state) ? l(t('Delete'), 'admin/project/project-issue-status/delete/'. $state->sid) : '';
    $form['delete'][$state->sid] = array(
      '#type' => 'markup',
      '#value' => $del_link,
    );
  }
  $form['default_state'] = array(
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $default_state,
  );
  $form['status_add']['name'] = array(
    '#type' => 'textfield',
    '#size' => 20,
    '#maxlength' => 255,
  );
  $form['status_add']['weight'] = array(
    '#type' => 'weight',
    '#default_value' => 0,
    '#delta' => 15,
    '#attributes' => array('class' => 'project-issue-status-weight'),
  );
  $form['status_add']['author_has'] = array(
    '#type' => 'checkbox',
  );
  $form['status_add']['default_query'] = array(
    '#type' => 'checkbox',
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#tree'] = TRUE;
  return $form;
}

/**
 * Submit handler for project_issue_admin_states_form.
 */
function project_issue_admin_states_form_submit($form, &$form_state) {
  // Check for and apply changes or additions to project issue status options.
  if (isset($form_state['values']['default_state'])) {
    variable_set('project_issue_default_state', $form_state['values']['default_state']);
  }
  // Update existing status options.
  if($form_state['values']['status']) {
    foreach ($form_state['values']['status'] as $sid => $value) {
      $state = db_fetch_object(db_query('SELECT name, weight, author_has, default_query FROM {project_issue_state} WHERE sid = %d', $sid));
      // Check to see whether the record needs updating.
      if (($state->name != $value['name']) || ($state->weight != $value['weight']) || ($state->author_has != $value['author_has']) || ($state->default_query != $value['default_query'])) {
        db_query("UPDATE {project_issue_state} SET name = '%s', weight = %d, author_has = %d, default_query = %d WHERE sid = %d", $value['name'], $value['weight'], $value['author_has'], $value['default_query'], $sid);
      }
    }
  }
  // Add any new status options.
  if (isset($form_state['values']['status_add']) && !empty($form_state['values']['status_add']['name'])) {
    // Check to see whether the state already exists:
    $issue_state = db_result(db_query("SELECT COUNT(*) FROM {project_issue_state} WHERE name = '%s'", $form_state['values']['status_add']['name']));
    if (empty($issue_state)) {
      db_query("INSERT INTO {project_issue_state} (name, weight, author_has, default_query) VALUES ('%s', %d, %d, %d)", $form_state['values']['status_add']['name'], $form_state['values']['status_add']['weight'], $form_state['values']['status_add']['author_has'], $form_state['values']['status_add']['default_query']);
    }
    else {
      drupal_set_message(t('Status %status already exists.', array ('%status' => $form_state['values']['status_add']['name'])), 'error');
    }
  }
}

function project_issue_delete_state_confirm(&$form_state, $sid) {
  $states = project_issue_state();
  $name = $states[$sid];

  $total = db_result(db_query('SELECT COUNT(nid) AS total FROM {project_issues} WHERE sid = %d', $sid));
  if ($total > 0) {
    $form['new_sid'] = array(
      '#type' => 'select',
      '#title' => t('Reassign status'),
      '#default_value' => $sid,
      '#options' => $states,
      '#description' => t('There are !total existing issues with the status of @name. Please select a new status for these issues.', array('!total' => $total, '@name' => $name)),
    );
  }
  $form['sid'] = array(
    '#type' => 'value',
    '#value' => $sid,
  );
  $form['name'] = array(
    '#type' => 'hidden',
    '#value' => $name,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete the status option %name?', array('%name' => $name)),
    'admin/project/project-issue-status',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

function project_issue_delete_state_confirm_validate($form, &$form_state) {
  if ($form_state['values']['new_sid'] == $form_state['values']['sid']) {
    form_set_error('new_sid', t('Choose a new issue status for existing issues of status %name.', array('%name' => $form_state['values']['name'])));
  }
}

function project_issue_delete_state_confirm_submit($form, &$form_state) {
  if ($form_state['values']['new_sid']) {
    db_query('UPDATE {project_issues} SET sid = %d WHERE sid = %d', $form_state['values']['new_sid'], $form_state['values']['sid']);
  }
  db_query('DELETE FROM {project_issue_state} WHERE sid = %d', $form_state['values']['sid']);
  drupal_set_message(t('Project issue status %issue deleted.', array('%issue' => $form_state['values']['name'])));
  $form_state['redirect'] ='admin/project/project-issue-status';
}

