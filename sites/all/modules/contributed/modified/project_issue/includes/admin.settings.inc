<?php
// $Id: admin.settings.inc,v 1.2 2009/04/04 06:30:21 dww Exp $

/**
 * @file
 * Code for the admin settings form at project/project-issue-settings.
 */

/**
 * Form builder for the main settings form.
 */
function project_issue_settings_form(&$form_state) {
  $form['project_directory_issues']  = array(
    '#type' => 'textfield',
    '#title' => t('Issue directory'),
    '#default_value' => variable_get('project_directory_issues', 'issues'),
    '#size' => 30,
    '#maxlength' => 255,
    '#description' => t("Subdirectory in the directory '%dir' where attachments to issues will be stored.", array('%dir' => variable_get('file_directory_path', 'files') . '/')),
    '#after_build' => array('project_issue_check_directory'),
  );

  $form['project_issue_show_comment_signatures'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display user signatures on issue followups'),
    '#default_value' => variable_get('project_issue_show_comment_signatures', 0),
    '#description' => t('If selected, user signatures will be appended to the display of issue followups.'),
  );

  $form['project_issue_reply_to'] = array(
    '#type' => 'textfield',
    '#title' => t('Reply-to address on e-mail notifications'),
    '#default_value' => variable_get('project_issue_reply_to', variable_get('site_mail', ini_get('sendmail_from'))),
    '#description' => t('All issue e-mails sent via subscriptions will appear from this e-mail address. You can use %project as a placeholder which will be replaced with the %short_project_name setting for the issue\'s current project.', array('%project' => '%project', '%short_project_name' => t('Short project name'))),
  );

  // Determine the auto followup username from the auto followup setting.
  $followup_username = '';
  $anon = variable_get('anonymous', t('Anonymous'));
  if ($followup_user_object = _project_issue_followup_get_user()) {
    $followup_username = $followup_user_object->name;
  }

  $form['project_issue_followup_user'] = array(
    '#title' => t('Auto-followup user'),
    '#type' => 'textfield',
    '#default_value' => $followup_username,
    '#maxlength' => 60,
    '#description' => t('Enter the name of the user which will create automatic followups to issues -- leave empty to disable auto-followup or set to %anon to use the anonymous user.', array('%anon' => $anon)),
    '#element_validate' => array('project_issue_validate_followup_user'),
    '#autocomplete_path' => 'user/autocomplete',
  );

  $form['project_issue_auto_close_days'] = array(
    '#title' => t('Auto-close days'),
    '#type' => 'textfield',
    '#default_value' => (int) variable_get('project_issue_auto_close_days', PROJECT_ISSUE_AUTO_CLOSE_DAYS),
    '#size' => 4,
    '#maxlength' => 10,
    '#description' => t('Issues being in "fixed" state for the specified number of days will be closed by the followup user specified above. For example, if this is 14, and an issue is set to fixed on January 1, then it will be closed on January 15.'),
  );
  
  $defaults = array(t('Code'), t('Documentation'), t('Miscellaneous'), t('User interface'));
  $components = variable_get('project_issue_default_components', implode("\n", $defaults));

  $form['project_issue_default_components'] = array(
    '#type' => 'textarea',
    '#title' => t('Default components'),
    '#default_value' => $components,
    '#cols' => 20,
    '#rows' => 6,
    '#required' => TRUE,
    '#description' => t("Enter the default list of components that new projects will have.  The project owner will be able to change this list on the project's edit/issues page."),
    '#element_validate' => array('project_issue_validate_default_components'),
  );

  if (module_exists('mailhandler')) {
    // TODO: move this stuff to mailhandler.module ?
    $items = array(t('<none>'));
    $result = db_query('SELECT mail FROM {mailhandler} ORDER BY mail');
    while ($mail = db_result($result, $i++)) {
      $items[$mail] = $mail;
    }

    // Switch reply-to to a select box instead.
    $form['project_issue_reply_to']['#type'] = 'select';
    $form['project_issue_reply_to']['#options'] = $items;
  }

  $form['project_issue_autocomplete'] = array(
    '#type' => 'radios',
    '#title' => t('Project selection widget'),
    '#default_value' => variable_get('project_issue_autocomplete', 0),
    '#options' => array(t('Select Menu'), t('Autocomplete')),
  );

  $form['project_issue_site_help'] = array(
    '#title' => t('Site-wide help text for new issues'),
    '#type' => 'textarea',
    '#default_value' => variable_get('project_issue_site_help', ''),
    '#cols' => 20,
    '#rows' => 5,
    '#description' => t('Optionally enter site-wide help text that will be displayed whenever a user tries to create a new issue. Please note that there is no automatic formatting on this text, but you can use regular HTML tags as necessary (for example %p, %ul, and so on).', array('%p' => '<p>', '%ul' => '<ul>')),
  );

  return system_settings_form($form);
}

function project_issue_validate_default_components($form, &$form_state) {
  // If the list of default components has changed (other than whitespace),
  // warn the admin that the change will only affect new projects.
  $value = trim($form['#value']);

  // If it's empty and required, the user will get a message that the
  // field is required, so don't bother warning them that the component
  // list has changed.
  if ($form['#required'] && !empty($value)) {
    $old = explode("\n", str_replace("\r", '', $form['#default_value']));
    $new = explode("\n", str_replace("\r", '', $value));

    // Trim whitespace on each entry in the array.
    array_walk($old, 'project_issue_trim');
    array_walk($new, 'project_issue_trim');
    // Filter out blank lines and reindex the arrays from 0.
    $old = array_values(array_filter($old));
    $new = array_values(array_filter($new));

    if ($old != $new) {
      drupal_set_message(t("The list of default components for new projects has changed. This will not affect existing projects."), 'warning');
    }
    // Regardless, now that we've trimmed everything, save those values.
    $form_state['values']['project_issue_default_components'] = implode("\n", $new);
  }
}

/**
 * Validates that the followup user exists, and has sufficient permissions
 * to follow up on issues.
 */
function project_issue_validate_followup_user($form, &$form_state) {
  $name = $form['#value'];
  if ($name !== '') {
    $anon = variable_get('anonymous', t('Anonymous'));
    // Make this check case-insensitive to allow the admin some data entry leeway.
    $is_anon = drupal_strtolower($name) == drupal_strtolower($anon);
    // Load the user. (don't see a constant for uid 0... )
    $account = $is_anon ? user_load(array('uid' => 0)) : user_load(array('name' => $name));
    if ($account) {
      if (user_access('access project issues', $account)) {
        // Transform the username into the more stable user ID.
        $form_state['values']['project_issue_followup_user'] = $account->uid;
      }
      else {
        form_error($form, t('%name does not have sufficient permissions to follow up on issues.', array('%name' => $is_anon ? $anon : $name)));
      }
    }
    else {
      form_error($form, t('%name is not a valid user.', array('%name' => $name)));
    }
  }
}

/**
 * Check whether the intended issues directory exists and ensure it is writable.
 */
function project_issue_check_directory($form_element) {
  $directory = file_create_path($form_element['#value']);
  file_check_directory($directory, FILE_CREATE_DIRECTORY, $form_element['#parents'][0]);
  return $form_element;
}

