<?php
// $Id: project_edit_issues.inc,v 1.3 2009/08/21 21:58:23 dww Exp $

/**
 * @file
 * Code for the "issues" subtab to the edit tab on project nodes.
 */

/**
 * Page callback for the "issues" subtab to the edit tab on project nodes.
 */
function project_issue_project_edit_issues($node) {
  project_project_set_breadcrumb($node);
  drupal_set_title(check_plain($node->title));
  return drupal_get_form('project_issue_project_edit_form', $node);
}

/**
 * Theme function to render the issue subtab form.
 */
function theme_project_issue_project_edit_form($form) {
  drupal_add_tabledrag('project-issue-edit-project-components-table', 'order', 'self', 'project-issue-component-weight');

  $header = array(
    array('data' => t('Name')),
    array('data' => t('Weight')),
    array('data' => t('Operations'))
  );
  // List the existing components.
  foreach (element_children($form['component']) as $key) {
    $rows[] = array(
      'class' => 'draggable',
      'data' => array(
        drupal_render($form['component'][$key]['name']),
        drupal_render($form['component'][$key]['weight']),
        drupal_render($form['component'][$key]['delete']),
      ),
    );
  }
  // Add a row to add a new component.
  $rows[] = array(
    'class' => 'draggable',
    'data' => array(
      drupal_render($form['component_add']['name']),
      drupal_render($form['component_add']['weight']),
      NULL,
    ),
  );

  $output = drupal_render($form['issue']);
  $output .= drupal_render($form['email']);
  $output .= '<h3>' . t('Issue components') . '</h3>';
  $output .= '<div>' . theme('table', $header, $rows, array('id' => 'project-issue-edit-project-components-table')) . '</div>';
  $output .= drupal_render($form);
  return $output;
}

/**
 * Form builder for the issues subtab on the edit tab for project nodes.
 */
function project_issue_project_edit_form(&$form_state, $node) {
  /* Issue properties */
  $form['issue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Issue information'),
    '#collapsible' => TRUE,
  );
  $form['issue']['issue'] = array(
    '#type' => 'item',
    '#title' => t('Issue tracker'),
  );
  $form['issue']['issues'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable issue tracker'),
    '#return_value' => 1,
    '#default_value' => isset($node->project_issue['issues']) ? $node->project_issue['issues'] : 1,
    '#description' => t('Let users submit bug requests, patches, feature requests, support requests, etc.'),
  );
  $form['issue']['help'] = array(
    '#type' => 'textarea',
    '#title' => t('Submission guidelines'),
    '#default_value' => isset($node->project_issue['help']) ? $node->project_issue['help'] : NULL,
    '#cols' => 20,
    '#rows' => 5,
  );

  $weight = 0;
  $default_component_options[0] = t('<none> (user must select)');
  $form['component']['#tree'] = TRUE;
  $number_components = count($node->project_issue['components']);
  $delta = max(20, $number_components + 10);
  foreach ($node->project_issue['components'] as $component) {
    $default_component_options[$component] = $component;
    $form['component'][$component]['name'] = array(
      '#type' => 'textfield',
      '#default_value' => $component,
      '#size' => 20,
    );
    $form['component'][$component]['weight'] = array(
      '#type' => 'weight',
      '#default_value' => $weight,
      '#delta' => $delta,
      '#attributes' => array('class' => 'project-issue-component-weight'),
    );
    // For the delete links, we just need the offset of the component as
    // currently stored in the DB or node. So, we can just use the $weight.
    // Using an integer offset avoids problems with urlencoding for component
    // names that have '/' in them.
    $del_link = ($component != $node->project_issue['default_component']) ? l(t('Delete'), 'node/'. $node->nid .'/edit/component/delete/'. $weight) : '';
    $form['component'][$component]['delete'] = array(
      '#type' => 'markup',
      '#value' => $del_link,
    );
    $weight++;
  }
  $form['component_add']['#tree'] = TRUE;
  $form['component_add']['name'] = array(
    '#type' => 'textfield',
    '#size' => 20,
  );
  $form['component_add']['weight'] = array(
    '#type' => 'weight',
    '#default_value' => $weight,
    '#delta' => $delta,
    '#attributes' => array('class' => 'project-issue-component-weight'),
  );
  $form['default_component'] = array(
    '#type' => 'select',
    '#title' => t('Default component for new issues'),
    '#options' => $default_component_options,
    '#default_value' => !empty($node->project_issue['default_component']) ? $node->project_issue['default_component'] : 0,
  );

  /* E-mail options */
  $form['email'] = array(
    '#type' => 'fieldset',
    '#title' => t('Issue e-mail options'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['email']['mail_digest'] = array(
    '#type' => 'textfield',
    '#title' => t('Weekly critical issues report'),
    '#default_value' => isset($node->project_issue['mail_digest']) ? $node->project_issue['mail_digest'] : NULL,
    '#size' => 20,
    '#maxlength' => 255,
    '#description' => t('To get a weekly digest of critical issues specify an e-mail address.'),
  );
  $form['email']['mail_copy'] = array(
    '#type' => 'textfield',
    '#title' => t('Issues e-mail address'),
    '#default_value' => isset($node->project_issue['mail_copy']) ? $node->project_issue['mail_copy'] : NULL,
    '#size' => 20,
    '#maxlength' => 255,
    '#description' => t('If you wish to receive a copy of all the issues to a central location specify an address here. <em>Note: the copy will contain links to file attachments.</em>'),
  );
  $options = project_issue_category();
  $form['email']['categories'] = array(
    '#type' => 'item',
    '#title' => t('Categories'),
    '#description' => t('Which issue categories to e-mail. If none is checked all categories will be posted.'),
  );
  $form['email']['mail_copy_filter']['#tree'] = TRUE;

  foreach ($options as $key => $choice) {
    $form['email']['mail_copy_filter'][$key] = array(
      '#type' => 'checkbox',
      '#title' => $choice,
      '#return_value' => $key,
      '#default_value' => isset($node->project_issue['mail_copy_filter'][$key]) ? $node->project_issue['mail_copy_filter'][$key] : NULL,
    );
  }
  $options = project_issue_state();
  $form['email']['states'] = array(
    '#type' => 'item',
    '#title' => t('States'),
    '#description' => t('Which issue states to e-mail. If none is checked all states will be posted.'),
  );
  $form['email']['mail_copy_filter_state']['#tree'] = TRUE;
  foreach ($options as $key => $choice) {
    $form['email']['mail_copy_filter_state'][$key] = array(
      '#type' => 'checkbox',
      '#title' => check_plain($choice),
      '#return_value' => $key,
      '#default_value' => isset($node->project_issue['mail_copy_filter_state'][$key]) ? $node->project_issue['mail_copy_filter_state'][$key] : NULL,
    );
  }

  if (user_access('administer projects')) {
    $form['email']['reminder'] = array(
      '#type' => 'item',
      '#title' => t('Monthly reminder'),
    );
    $form['email']['mail_reminder'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send monthly reminders to users'),
      '#return_value' => 1,
      '#default_value' => isset($node->project_issue['mail_reminder']) ? $node->project_issue['mail_reminder'] : NULL,
      '#description' => t('Enabling this will send a monthly reminder to users that have open issues registered.'),
    );
  }

  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 45,
  );

  return $form;
}

/**
 * Validation handler when project admins use the issues subtab.
 * @see project_issue_project_edit_issues
 */
function project_issue_project_edit_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['mail_digest']) && ($data = user_validate_mail($form_state['values']['mail_digest']))) {
    form_set_error('mail_digest', $data);
  }
  if (!empty($form_state['values']['mail_copy']) && ($data = user_validate_mail($form_state['values']['mail_copy']))) {
    form_set_error('mail_copy', $data);
  }
}

/**
 * Submit handler when project admins use the issues subtab.
 * @see project_issue_project_edit_issues
 */
function project_issue_project_edit_form_submit($form, &$form_state) {
  $components = array();
  if (!empty($form_state['values']['component_add']['name'])) {
    $components[trim($form_state['values']['component_add']['name'])] = $form_state['values']['component_add']['weight'];
  }
  if (!empty($form_state['values']['component'])) {
    foreach ($form_state['values']['component'] as $component) {
      $components[trim($component['name'])] = $component['weight'];
    }
  }
  asort($components);
  $components = serialize(array_keys($components));
  $default_component = !empty($form_state['values']['default_component']) ? $form_state['values']['default_component'] : '';
  $mail_copy_filter = serialize($form_state['values']['mail_copy_filter']);
  $mail_copy_filter_state = serialize($form_state['values']['mail_copy_filter_state']);

  db_query("UPDATE {project_issue_projects} SET issues = %d, components = '%s',default_component = '%s', mail_digest = '%s', mail_reminder = %d, mail_copy = '%s', mail_copy_filter = '%s', mail_copy_filter_state = '%s', help = '%s' WHERE nid = %d", $form_state['values']['issues'], $components, $default_component, $form_state['values']['mail_digest'], $form_state['values']['mail_reminder'], $form_state['values']['mail_copy'], $mail_copy_filter, $mail_copy_filter_state, $form_state['values']['help'], $form_state['values']['nid']);
  db_query("UPDATE {node} SET changed = %d WHERE nid = %d", time(), $form_state['values']['nid']);
  drupal_set_message(t('Issue settings have been saved.'));
}

function project_issue_component_delete_form($form_state, $project, $component_offset) {
  $component = $project->project_issue['components'][$component_offset];
  $form['component'] = array(
    '#type' => 'value',
    '#value' => $component,
  );
  $form['project'] = array(
    '#type' => 'value',
    '#value' => $project,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete the component %component?', array('%component' => $component)),
    'node/' . $project->nid . '/edit/issues',
    t('This action cannot be undone.'),
    t('Delete'), t('Cancel')
  );
}

function project_issue_component_delete_form_submit($form, &$form_state) {
  $project = $form_state['values']['project'];
  $component = $form_state['values']['component'];
  project_issue_delete_component($project, $component);
  drupal_set_message(t('Issue component %component deleted.', array('%component' => $component)));
  $form_state['redirect'] = 'node/' . $project->nid . '/edit/issues';
}

/**
 * Remove a component from a given project.
 *
 * @param $project
 *   The project to remove the component from.
 * @param $component
 *   The component to remove.
 */
function project_issue_delete_component($project, $component) {
  $components = array_diff($project->project_issue['components'], array($component));
  db_query("UPDATE {project_issue_projects} SET components = '%s' WHERE nid = %d", serialize($components), $project->nid);
  db_query("UPDATE {node} SET changed = %d WHERE nid = %d", time(), $project->nid);
}


