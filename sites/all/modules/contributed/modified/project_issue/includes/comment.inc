<?php
// $Id: comment.inc,v 1.156 2010/01/17 00:26:58 dww Exp $

function project_issue_comment(&$arg, $op) {
  // $arg can be a comment object, or a form or form_values.
  if (is_object($arg)) {
    $nid = $arg->nid;
  }
  elseif (is_array($arg)) {
    $nid = is_array($arg['nid']) ? $arg['nid']['#value'] : $arg['nid'];
  }
  $node = node_load($nid);
  if ($node->type != 'project_issue') {
    return;
  }
  // Make a copy here so we have all the original metadata, since some
  // of it can change below.
  $original_node = drupal_clone($node);
  $old_data = (object) $original_node->project_issue;
  $old_data->title = $original_node->title;

  // Maintain an array of project ids that are affected by this comment
  // operation. We'll use this to invalidate the "Issue cockpit" block cache
  // for any of these projects.
  $affected_projects = array();

  switch ($op) {
    case 'insert':
      // Get a lock on the issue in order to generate the next comment ID.
      $tries = 20;
      $sleep_increment = 0;
      while ($tries) {
        $lock = db_query("UPDATE {project_issues} SET db_lock = 1 WHERE nid = %d AND db_lock = 0", $arg['nid']);
        if (db_affected_rows()) {
          $id = db_result(db_query("SELECT last_comment_id FROM {project_issues} WHERE nid = %d", $arg['nid'])) + 1;
          db_query("UPDATE {project_issues} SET last_comment_id = %d, db_lock = 0 WHERE nid = %d", $id, $arg['nid']);
          break;
        }

        // Wait a random and increasing amount of time before the next attempt.
        $sleep = rand(10000, 1000000) + $sleep_increment;
        usleep($sleep);
        $sleep_increment += 50000;
        $tries--;
      }

      if (isset($id)) {
        $rid = isset($arg['project_info']['rid']) ? $arg['project_info']['rid'] : 0;
        db_query("INSERT INTO {project_issue_comments} (nid, cid, pid, rid, component, category, priority, assigned, sid, title, timestamp, comment_number) VALUES (%d, %d, %d, %d, '%s', '%s', %d, %d, %d, '%s', %d, %d)", $arg['nid'], $arg['cid'], $arg['project_info']['pid'], $rid, $arg['project_info']['component'], $arg['category'], $arg['priority'], $arg['project_info']['assigned'], $arg['sid'], $arg['title'], $arg['timestamp'], $id);
        db_query("UPDATE {comments} SET subject = '%s' WHERE cid = %d", "#$id", $arg['cid']);
        project_issue_update_by_comment($arg, 'insert');
        $affected_projects[$old_data->pid] = 1;
        $affected_projects[$arg['project_info']['pid']] = 1;
      }
      else {
        drupal_set_message(t('There was an error submitting your comment -- please try again. If the problem persists, contact the system administrator.'), 'error');
        watchdog('project_issue', 'Error obtaining lock for project issue %nid', array('%nid' => $arg['nid']), WATCHDOG_ERROR, 'node/'. $arg['nid']);
        // This is a bit extreme, but we have to clean up the failed comment,
        // or it will appear on the issue.
        _comment_delete_thread((object) $arg);
        _comment_update_node_statistics($arg['nid']);
        cache_clear_all();
        // The hard redirect prevents any bogus data from being inserted for the failed comment.
        drupal_goto('node/'. $arg['nid']);
      }
      break;

    case 'update':
      die($arg);
      project_issue_update_by_comment($arg, 'update');
      // Updating a comment can't change anything relevant about the issue for
      // the purposes of the issue blocks, so we don't need to touch
      // $affected_projects here.
      break;

    case 'delete':
      // Save the project that's specified in this comment so we can
      // invalidate its issue block cache.
      $deleted_comment_project_id = db_result(db_query("SELECT pid FROM {project_issue_comments} WHERE cid = %d", $arg->cid));
      $affected_projects[$deleted_comment_project_id] = 1;
      // Actually delete the comment
      db_query("DELETE FROM {project_issue_comments} WHERE cid = %d", $arg->cid);
      $current_data = project_issue_update_by_comment($arg, 'delete');
      // We should also invalidate the block cache for whatever project is now
      // used for this issue, since we might be deleting a comment that moved
      // an issue from one project to another.
      $affected_projects[$current_data->pid] = 1;      
      break;

    case 'view':
      if (isset($arg->cid)) {
        $project_issue_table = project_issue_comment_view($original_node, $arg);
      }
      else {
        // Previewing a comment.
        $test = drupal_clone($arg);
        $test->pid = $arg->project_info['pid'];
        $test->component = $arg->project_info['component'];
        $test->assigned = $arg->project_info['assigned'];
        // Add a dummy rid if necessary -- prevents incorrect change data.
        $test->rid = isset($arg->project_info['rid']) ? $arg->project_info['rid'] : 0;
        $comment_changes = project_issue_metadata_changes($node, $old_data, $test, project_issue_field_labels('web'));
        $project_issue_table = theme('project_issue_comment_table', $comment_changes);
      }
      if ($project_issue_table) {
        $arg->comment = '<div class="project-issue"><div class="summary">'. $project_issue_table .'</div></div>' . $arg->comment;
      }
      break;

  }
  // If there are any affected projects, invalidate the block cache for those.
  if (!empty($affected_projects)) {
    foreach ($affected_projects as $pid => $value) {
      $cid = 'project_issue_cockpit_block:'. $pid;
      cache_clear_all($cid, 'cache');
    }
  }
}

/**
 * Add project issue metadata to the comment form.
 *
 * @param $form
 *   Reference to form structure.
 * @param $form_state
 *   Current form state.
 */
function project_issue_form_comment_form_alter(&$form, &$form_state) {
  $nid = $form['nid']['#value'];
  $node = node_load($nid);

  // Allows only project_issue
  if ($node->type != 'project_issue') {
    return;
  }
  
  // Comment body is not required since we validate that ourselves.
  unset($form['comment_filter']['comment']['#required']);

  // The 'your name' item just wastes screen estate.
  unset($form['_author']);

  // For existing comments, we want to preserve the comment subject,
  // Even if the subject field is disabled.
  if ($cid = $form['cid']['#value']) {
    $subject = db_result(db_query('SELECT subject FROM {comments} WHERE cid = %d', $cid));
  }
  // For new comments, show the expected next number for previews.
  // This is only for show, the number will be generated when the comment
  // is posted.
  else {
    $next_id = db_result(db_query('SELECT last_comment_id FROM {project_issues} WHERE nid = %d', $form['nid']['#value'])) + 1;
    $subject = "#$next_id";
    // Clobber the comment signature for new followups if necessary.
    // TODO: Revamp this for Drupal 6.
    if (!variable_get('project_issue_show_comment_signatures', 0)) {
      $form['comment_filter']['comment']['#default_value'] = '';
    }
  }
  $form['subject'] = array(
    '#type' => 'value',
    '#value' => $subject,
  );

  // Any time we're on a reply page, show the full issue below the reply.
  if (project_issue_is_comment_reply()) {
    $form['#pre_render'][] = 'project_issue_comment_pre_render';
  }

  // Make sure project is current here -- it may have changed when posted.
  if (!empty($form_state['values']['project_info']['pid'])) {
    $node->project_issue['pid'] = $form_state['values']['project_info']['pid'];
  }
  $project = node_load(array('nid' => $node->project_issue['pid'], 'type' => 'project_project'));

  project_issue_set_breadcrumb($node, $project);

  // Only allow metadata changes on new followups.
  if (isset($form['cid']['#value'])) {
    return;
  }

  // We have to set $form['#action'] to prevent AHAH nastiness.
  if (!empty($form['pid']['#value'])) {
    $form['#action'] = url('comment/reply/' . $nid . '/' . $form['pid']['#value']);
  }
  else {
    $form['#action'] = url('comment/reply/' . $nid);
  }
  
  

//  // We need to ask for almost the same metadata as project issue itself
//  // so let's reuse the form.
  $form += project_issue_form($node, $form_state, TRUE);
  
  
  // comment.module is basically still FAPI v1. It sets the preview button to
  // #type 'button', so FAPI doesn't really consider that a form submission.
  // However, we depend on the form being rebuilt on preview to do our magic.
  // Thanks to a change in 6.14 core, form.inc will only rebuild the form if
  // $form_state['submitted'] is TRUE. So, we set the preview button to
  // actually be a 'submit' button so that the form is rebuilt on preview and
  // our comment preview code can kick in.
  $form['preview']['#type'] = 'submit';
  

  // We need this otherwise pid collides with comment.
  $form['project_info']['#tree'] = TRUE;
  $form['project_info']['#weight'] = -2;

  // Remove the form item that displays the current project, and
  // replace the static single project value with a select list
  // of all projects to make swapping simpler.
  unset($form['project_info']['project_display']);
  $uris = NULL;

  if (variable_get('project_issue_autocomplete', 0) == 1) {
    $form['project_info']['project_title'] = array(
      '#type' => 'textfield',
      '#title' => t('Project'),
      '#default_value' => $project->title,
      '#required' => TRUE,
      '#weight' => -1,
      '#size' => 35,
      '#autocomplete_path' => 'project/autocomplete/issue/project',
      '#attributes' => array(
        'onFocus' => 'project_issue_autocomplete_handler()',
      ),
      '#ahah' => array(
        'progress' => array(
          'type' => 'none',
        ),
        'path' => 'project/issues/update_project',
        'wrapper' => 'project-info-wrapper',
      ),
    );
  }
  else {
    $projects = project_projects_select_options($uris);
    $form['project_info']['pid'] = array(
      '#type' => 'select',
      '#title' => t('Project'),
      '#default_value' => $project->nid,
      '#options' => $projects,
      '#required' => TRUE,
      '#weight' => -1,
      '#ahah' => array(
        'path' => 'project/issues/update_project',
        'wrapper' => 'project-info-wrapper',
        'event' => 'change',
      ),
    );
  }

  $form['issue_info']['#weight'] = -1;
  $form['#prefix'] = '<div class="project-issue"><div class="node-form"><div class="standard">';
  $form['#suffix'] = '</div></div></div>';

  $form['original_issue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Edit issue settings'),
    '#description' => t('Note: changing any of these items will update the issue\'s overall values.'),
    '#collapsible' => TRUE,
    '#weight' => -10,
  );

  $form['original_issue']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Issue title'),
    '#maxlength' => 128,
    '#default_value' => $node->title,
    '#weight' => -30,
    '#required' => TRUE,
  );
  
  

  $form['project_info']['assigned'] = $form['issue_info']['assigned'];
  unset($form['issue_info']['assigned']);

  $form['project_info']['#prefix'] = '<div id="project-info-wrapper" class="inline-options">';
  $form['project_info']['#suffix'] = '</div>';

  // Remove the 'Project information' and 'Issue information' fieldsets,
  // since we'll move everything inside the 'Edit issue settings' fieldset.
  unset($form['project_info']['#type'], $form['project_info']['#title']);
  unset($form['issue_info']['#type'], $form['issue_info']['#title']);

  
  
  //TBR add categories from db
  $pid = $form['project_info']['pid']['#value'];
  $project = array('nid'=>$pid);
  $categories = project_issue_categories_get_all($project);
  foreach ($categories as  $key=>$value) {
    $form['issue_info']['category']['#options'][$value['title']] = $value['title'];
  }
  // Restructure the UI to de-emphasize the original project form inputs.
  $form['original_issue']['project_info'] = $form['project_info'];
  $form['original_issue']['issue_info'] = $form['issue_info'];
  unset($form['project_info'], $form['issue_info']);
  unset($form['issue_details'], $form['project_help']);
  
  
  
 
  drupal_add_js(drupal_get_path('module', 'project_issue') .'/project_issue.js');
  
  //TBR add the project specific issue categories
  global $user;
    $pid = $form['project_info']['pid']['#value'];
    $project = array('uid'=>$user->uid,'nid'=>$pid);
    $categories = project_issue_categories_get_all($project);
    foreach ($categories as  $key=>$value) {
      $form['issue_info']['category']['#options'][$value['title']] = $value['title'];
    }
    
}

/**
 * Validate issue metadata on the comment form.
 *
 * @param $form
 *   The Drupal form structure.
 * @param $form_state
 *   The current state of the form.
 */
function project_issue_form_comment_validate($form, &$form_state) {
  if (empty($form['cid']['#value']) && variable_get('project_issue_autocomplete', 0) == 1) {
    if (empty($form_state['values']['project_info']['project_title'])) {
      form_set_error('project_title', t('You must enter a project to navigate to.'));
    }
    else {
      $pid = db_result(db_query("SELECT nid FROM {node} WHERE title = '%s' AND type = '%s'", $form_state['values']['project_info']['project_title'], 'project_project'));
      if (empty($pid)) {
        form_set_error('project_info][project_title', t('The name you entered (%title) is not a valid project.', array('%title' => $form_state['values']['project_info']['project_title'])));
      }
      else {
        $form_state['values']['project_info']['pid'] = $pid;
      }
    }
  }

  if (!empty($form_state['rebuild'])) {
    return;
  }
  $values = $form_state['values'];
  $project_info = $form_state['values']['project_info'];
  $nid = $values['nid'];
  $node = node_load($nid);

  // Make a copy here so we have all the original metadata, since some
  // of it can change below.
  $original_node = drupal_clone($node);
  $old_data = (object) $original_node->project_issue;
  $old_data->title = $original_node->title;

  // Adjust new file attachments to go to the issues directory.
  // We have to do this during validate, otherwise we might miss
  // adjusting the filename before comment upload saves it (module weighting)
  // TODO: is this still true?
  project_issue_change_comment_upload_path($values);

  // Only validate metadata changes on new followups.
  if (isset($values['cid'])) {
    return;
  }

  // Make sure project is current here -- it may have changed when posted.
  if (isset($project_info['pid'])) {
    $node->project_issue['pid'] = $project_info['pid'];
  }
  $project = node_load($node->project_issue['pid']);

  if (!empty($project) && $project->type == 'project_project') {
    // Force all comments to be a child of the main issue, to match the
    // flat display, and also to prevent accidentally deleting a thread.
    $form_state['values']['pid'] = 0;

    // Validate version.
    if (module_exists('project_release') && isset($project_info['rid']) && ($releases = project_release_get_releases($project, 0, 'version', 'all', array($project_info['rid'])))) {
      $rid = $project_info['rid'];
      if ($rid && !in_array($rid, array_keys($releases))) {
        $rid = 0;
      }
      // Check to make sure this release is not marked as an invalid
      // release node for user selection.
      $invalid_rids = variable_get('project_issue_invalid_releases', array());
      if (!empty($invalid_rids) &&
          ((empty($rid) && in_array($node->project_issue['rid'], $invalid_rids))
           || in_array($rid, $invalid_rids))) {
        form_set_error('project_info][rid', t('%version is not a valid version, please select a different value.', array('%version' => $releases[$node->project_issue['rid']])));
      }
      elseif (empty($rid)) {
        form_set_error('project_info][rid', t('You have to specify a valid version.'));
      }
    }
    // Add a dummy rid if necessary -- prevents incorrect change data.
    else {
      $rid = 0;
    }
    // Validate component.
    $component = $project_info['component'];
    if ($component && !in_array($component, $project->project_issue['components'])) {
      $component = 0;
    }
    empty($component) && form_set_error('project_info][component', t('You have to specify a valid component.'));
  }
  else {
    form_set_error('project_info][pid', t('You have to specify a valid project.'));
  }
  empty($values['category']) && form_set_error('category', t('You have to specify a valid category.'));

  // Now, make sure the comment changes *something* about the issue.
  // If the user uploaded a file, so long as it's not marked for removal,
  // we consider that a valid change to the issue, too.
  $has_file = FALSE;
  $files = isset($values['files']) ? $values['files'] : array();
  foreach ($files as $number => $data) {
    if (empty($data['remove'])) {
      $has_file = TRUE;
      break;
    }
  }
  if (!$has_file && empty($values['comment'])) {

    $comment = drupal_clone((object) $values);
    $comment->pid = $project_info['pid'];
    $comment->component = $component;
    $comment->rid = $rid;
    $comment->assigned = $project_info['assigned'];
    $comment_changes = project_issue_metadata_changes($node, $old_data, $comment, project_issue_field_labels('web'));
    // If the PID changed, rebuild the form
    if (isset($comment_changes['pid']['new']) && $comment_changes['pid']['new'] === TRUE) {
      $form_state['rebuild'] = TRUE;
    }
    $has_change = FALSE;
    foreach ($comment_changes as $field => $changes) {
      if (isset($changes['new'])) {
        $has_change = TRUE;
        break;
      }
    }
    if (!$has_change) {
      form_set_error('comment', t('You must either add a comment, upload a file, or change something about this issue.'));
    }
  }
}

/**
 * Theme a project issue metadata table.
 *
 * @param $comment_changes
 *  Array containing metadata differences between comments
 *  as returned by project_issue_metadata_changes().
 * @return
 *  The themed metadata table.
 */
function theme_project_issue_comment_table($comment_changes) {
  $rows = array();
  foreach ($comment_changes as $field => $change) {
    if (!empty($change['label']) && isset($change['old']) && isset($change['new'])) {
      $rows[] = theme('project_issue_comment_table_row', $field, $change);
    }
  }
  return $rows ? theme('table', array(), $rows) : '';
}

/**
 * Theme a single row of the project issue metadata changes table.
 *
 * @param $field
 *   The name of the field to theme.
 * @param $change
 *   A nested array containing changes to project issue metadata
 *   for the given issue or comment.
 * @return
 *   An array representing one row of the table.
 *
 * NOTE:  If you override this theme function, you *must* make sure
 * that you sanitize all output from this function that is displayed
 * to the user.  No further escaping/filtering of the data in this
 * table will take place after this function.  In most cases
 * this means that you need to run the $change['label'], $change['old'],
 * and $change['new'] values through either the check_plain() or
 * filter_xss() function to prevent XSS and other types
 * of problems due to any malicious input in these
 * field values.
 */
function theme_project_issue_comment_table_row($field, $change) {  
  // Allow anchor, emphasis, and strong tags in metadata tables.
  $allowed_tags = array('a', 'em', 'strong');
  // Fields that should be rendered as plain text, not filtered HTML.
  $plain_fields = array('title', 'pid', 'rid');

  if (is_array($change['old']) || is_array($change['new'])) {
    $removed = array();
    if (is_array($change['old'])){
      foreach ($change['old'] as $item) {
        $removed[] = '-'. $item;
      }
    }
    elseif (!empty($change['old'])) {
      $removed[] = '-'. $change['old'];
    }

    $added = array();
    if (is_array($change['new'])) {
      foreach ($change['new'] as $item) {
        $added[] = '+'. $item;
      }
    }
    elseif (!empty($change['new'])) {
      $added[] = '+'. $change['new'];
    }

    return array(
      filter_xss($change['label'], $allowed_tags) .':',
      filter_xss(implode(', ', $removed), $allowed_tags),
      filter_xss(implode(', ', $added), $allowed_tags),
    );
  }
  elseif (in_array($field, $plain_fields)) {
    return array(
      filter_xss($change['label'], $allowed_tags) .':',
      check_plain(project_issue_change_summary($field, $change['old'])),
      '&raquo; '. check_plain(project_issue_change_summary($field, $change['new'])),
    );
  }
  else {
    return array(
      filter_xss($change['label'], $allowed_tags) .':',
      filter_xss(project_issue_change_summary($field, $change['old']), $allowed_tags),
      '&raquo; '. filter_xss(project_issue_change_summary($field, $change['new']), $allowed_tags),
    );
  }
}

/**
 * Returns the issue metadata table for a comment.
 *
 * @param $node
 *  The corresponding node.
 * @param $comment
 *  The comment, if it's set then metadata will be returned. If it's not
 *  set then metadata will be precalculated.
 * @return
 *  A themed table of issue metadata.
 */
function project_issue_comment_view(&$node, $comment = NULL) {
  static $project_issue_tables;

  if (isset($comment)) {
    return isset($project_issue_tables[$comment->cid]) ? $project_issue_tables[$comment->cid] : '';
  }
  if (!empty($node->comment_count)) {
    $old = unserialize(db_result(db_query('SELECT original_issue_data FROM {project_issues} WHERE nid = %d', $node->nid)));
    $labels = project_issue_field_labels('web');
    $result = db_query('SELECT p.cid, p.title, p.pid, p.rid, p.component, p.category, p.priority, p.assigned, p.sid FROM {project_issue_comments} p INNER JOIN {comments} c ON p.cid = c.cid WHERE p.nid = %d AND c.status = %d ORDER BY p.timestamp ASC', $node->nid, COMMENT_PUBLISHED);
    while ($followup = db_fetch_object($result)) {
      $followup_changes = project_issue_metadata_changes($node, $old, $followup, project_issue_field_labels('web'));      
      $project_issue_tables[$followup->cid] = theme('project_issue_comment_table', $followup_changes);
      $old = $followup;
    }
  }
}

/**
 * Updates the project issue based on the comment inserted/updated/deleted.
 *
 * @param $comment_data
 *  The comment data that's been submitted.
 * @param $op
 *  The comment operation performed, 'insert', 'update', 'delete'.
 * @return
 *   An object representing the comment data used to update the issue.
 */
function project_issue_update_by_comment($comment_data, $op) {
  switch ($op) {
    case 'insert':
      // Massage the incoming data so the structure is consistent throughout the function.
      $comment_data['component'] = $comment_data['project_info']['component'];
      $comment_data['pid'] = $comment_data['project_info']['pid'];
      $comment_data['rid'] = isset($comment_data['project_info']['rid']) ? $comment_data['project_info']['rid'] : 0;
      unset ($comment_data['project_info']);
      $comment_data = (object) $comment_data;
      // Mark the node for email notification during hook_exit(), so all issue
      // and file data is in a consistent state before we generate the email.
      if (!isset($comment_data->followup_no_mail)) {  // Temporary hack to get around sending of auto-close emails.
        project_issue_set_mail_notify($comment_data->nid);
      }
      break;
    case 'update':
      $comment_data = (object) $comment_data;
      break;
  }

  // In order to deal with deleted/unpublished comments, make sure that we're performing
  // the updates to the issue with the latest available published comment.
  $comment_data = project_issue_get_newest_comment($comment_data);
  

  // Update the issue data to reflect the new final states.
  db_query("UPDATE {project_issues} SET pid = %d, category = '%s', component = '%s', priority = %d, rid = %d, assigned = %d, sid = %d WHERE nid = %d", $comment_data->pid, $comment_data->category, $comment_data->component, $comment_data->priority, $comment_data->rid, $comment_data->assigned, $comment_data->sid, $comment_data->nid);

  // Update the issue title.
  $node = node_load($comment_data->nid, NULL, TRUE);  // Don't use cached since we changed data above.
  $node->title = $comment_data->title;

  // This also updates the changed date of the issue.
  node_save($node);
  // Return the object of comment data we used to update the issue.
  return $comment_data;
}

/**
 * Adjusts the filepath of issue followups so files are saved to
 * the correct issues directory.
 *
 * @param $comment
 *   An array of the submitted comment values.
 */
function project_issue_change_comment_upload_path(&$comment) {
  static $run = NULL;

  // Only for new comments with attachments.
  if (empty($comment['cid']) && isset($comment['files']) && !isset($run)) {
    $run = TRUE;  // Make sure this only gets run once.
    project_issue_rewrite_issue_filepath($comment['files']);
  }
}

/**
 * Retrieves the newest published comment for an issue.
 *
 * @param $comment_data
 *   An object representing the current comment being edited
 * @return
 *   An object representing the most recent published comment for the issue.
 */
function project_issue_get_newest_comment($comment_data) {
  
  
  // Get the cid of the most recent comment.
  $latest_cid = db_result(db_query_range('SELECT pic.cid FROM {project_issue_comments} pic INNER JOIN {comments} c ON c.cid = pic.cid WHERE c.nid = %d AND c.status = %d ORDER BY pic.timestamp DESC', $comment_data->nid, COMMENT_PUBLISHED, 0, 1));
  if ($latest_cid) {
    $comment_data = db_fetch_object(db_query('SELECT * FROM {project_issue_comments} WHERE cid = %d', $latest_cid));
  }
  // No more comments on the issue -- use the original issue metadata.
  else {
    // nid isn't stored in the original issue data, so capture it here and pass back
    // into the object.
    $nid = $comment_data->nid;
    $comment_data = unserialize(db_result(db_query("SELECT original_issue_data FROM {project_issues} WHERE nid = %d", $comment_data->nid)));
    $comment_data->nid = $nid;
  }
  return $comment_data;
}

/**
 * Test to determine if the active page is the comment reply form.
 *
 * @return
 *   TRUE if the active page is the comment reply form, FALSE otherwise.
 */
function project_issue_is_comment_reply() {
  return arg(0) == 'comment' && arg(1) == 'reply';
}

/**
 * Test to determine if the active page is the comment edit form.
 *
 * @return
 *   TRUE if the active page is the comment edit form, FALSE otherwise.
 */
function project_issue_is_comment_edit() {
  return arg(0) == 'comment' && arg(1) == 'edit';
}

/**
 * Appends the comment thread to the comment reply form.
 */
function project_issue_comment_pre_render($form) {
  // Force the correct formatting.
  $_GET['mode'] = COMMENT_MODE_FLAT_EXPANDED;
  $_GET['sort'] = COMMENT_ORDER_OLDEST_FIRST;

  $suffix = empty($form['#suffix']) ? '' : $form['#suffix'];
  $node = node_load($form['nid']['#value']);

  // Unfortunately, the comment module blindly puts the node view
  // after the comment form on preview, in the case where the comment
  // parent is 0.  If we want our issue previews to be consistent, this
  // ugly hack is necessary.
  if (isset($form['#parameters'][1]['values']['op']) && $form['#parameters'][1]['values']['op'] == t('Preview')) {
    $preview = comment_render($node, 0);
  }
  else {
    $preview = node_show($node, 0);
  }

  $form['#suffix'] = $suffix . $preview;
  return $form;
}
