<?php
// $Id: cron.inc,v 1.6 2009/03/21 18:50:18 thehunmonkgroup Exp $


/**
 * @file
 * Contains the code required during cron runs for periodic functionality.
 *
 * This code is either invoked via hook_cron() or project-issue-cron.php
 * depending on the value of the 'project_issue_hook_cron' variable.
 */

/**
 * Private helper function to run periodic functionality.
 *
 * This code is either invoked by hook_cron() or via project-issue-cron.php
 * depending on the value of the 'project_issue_hook_cron' variable. It is
 * responsible for auto-closing issues, and sending email digests and
 * reminders.
 *
 * @see project_issue_cron()
 */
function _project_issue_cron() {
  if (time() - variable_get('project_issue_digest_last', 0) > variable_get('project_issue_digest_interval', 7 * 24 * 60 * 60)) {
    variable_set('project_issue_digest_last', time());
    project_mail_digest();
  }

  if (time() - variable_get('project_issue_reminder_last', 0) > variable_get('project_issue_reminder_interval', 28 * 7 * 24 * 60 * 60)) {
    variable_set('project_issue_reminder_last', time());
    project_mail_reminder();
  }

  // Auto-close fixed issues;
  project_issue_auto_close();
}

/**
 * Automatically close issues marked as fixed for a specified number of days
 * and add a comment to each documenting the change.
 */
function project_issue_auto_close() {
  // Set query parameters.
  $auto_close_days = variable_get('project_issue_auto_close_days', PROJECT_ISSUE_AUTO_CLOSE_DAYS);
  $seconds = 24 * 60 * 60 * $auto_close_days;

  $comment = theme('project_issue_auto_close_message', $auto_close_days);
  $result = db_query('SELECT pi.nid FROM {project_issues} pi INNER JOIN {node} n ON n.nid = pi.nid WHERE pi.sid = %d AND n.changed < %d', PROJECT_ISSUE_STATE_FIXED, time() - $seconds);
  while ($issue = db_fetch_object($result)) {
    project_issue_add_auto_followup(array(
      'nid' => $issue->nid,
      'sid' => PROJECT_ISSUE_STATE_CLOSED,
      'comment' => $comment,
      'followup_no_mail' => TRUE,  // Temporary hack to get around sending of auto-close emails.
    ));
  }
}

function project_mail_reminder() {

  if (defined('PROJECT_NOMAIL')) {
    return;
  }

  $projects = array();
  $result = db_query(db_rewrite_sql('SELECT p.nid, n.title FROM {project_issue_projects} p INNER JOIN {node} n ON p.nid = n.nid WHERE p.mail_reminder = 1 AND n.status = 1', 'p'));
  while ($project = db_fetch_object($result)) {
    $projects[$project->nid] = $project->title;
  }

  if (!empty($projects)) {
    // Note: We can INNER JOIN on {users} on uid = p.assigned since there's
    // still a record in {users} for uid 0 (anonymous), so we'll still get a
    // (bogus) value, even if the issue is unassigned.
    $pids = array_keys($projects);
    $placeholders = db_placeholders($pids);
    $result = db_query(db_rewrite_sql("SELECT p.nid, n.*, p.*, u.name, u.mail, u.language, u2.name AS assigned_name FROM {project_issues} p INNER JOIN {node} n ON p.nid = n.nid INNER JOIN {users} u ON n.uid = u.uid INNER JOIN {users} u2 ON u2.uid = p.assigned WHERE n.status = 1 AND u.status = 1 AND p.pid IN ($placeholders) AND u.mail <> '' AND (p.sid = 1 OR p.sid = 2) ORDER BY u.uid, p.pid, p.component, p.sid, n.changed DESC", 'p'), $pids);

    $body = $mail = $pid = NULL;

    // TODO: This logic sucks, is inefficient, and is fragile.  It'd be nice
    // to rewrite this someday when I have more time to not be so crazy. -dww
    while (($node = db_fetch_object($result)) || !empty($body)) {

      // If we already have a message we're planning to send, and either we
      // ran out of issues, or the e-mail address of the user changed
      // (different user's issues), send out what we've got already.
      if ($body && $mail && ((!$node) || ($mail != $node->mail))) {
        $params['body'] = $body;
        drupal_mail('project_issue', 'project_issue_reminder', $mail, $language, $params);
        $body = '';
      }

      if ($node) {
        // If this is a new project, a new component, or a new user, start a
        // new banner to indicate what this issue belongs to.
        if ($pid != $node->pid || $component != $node->component || $mail != $node->mail) {
          $pid = $node->pid;
          $component = $node->component;
          $banner = "$projects[$pid] / $component";
          $body .= "[ $banner ]". str_repeat('=', 72 - 4 - strlen($banner)) ."\n";
        }
        $body .= "$node->title\n";
        if ($node->assigned) {
          $body .= "  assigned: $node->assigned_name\n";
        }
        $body .= '  state: '. project_issue_state($node->sid) ."\n";
        $body .= '  age: '. format_interval(time() - $node->created) ."\n";
        $body .= '  url: '. url("node/$node->nid", array('absolute' => TRUE)) ."\n";
        $body .= "\n";

        // Remember the e-mail and language of this issue's user so that when
        // we next decide to send what we've got, we'll have the right values.
        $mail = $node->mail;
        // We can pass $node here because user_preferred_language()
        // only needs $account->language to function, and we have that
        // loaded into the $node object.
        $language = user_preferred_language($node);
      }
    }
  }
}

function project_mail_digest() {
  if (defined('PROJECT_NOMAIL')) {
    return;
  }

  $projects = db_query(db_rewrite_sql("SELECT n.nid, n.title, p.*, u.language FROM {node} n INNER JOIN {project_issue_projects} p ON n.nid = p.nid INNER JOIN {users} u ON u.uid = n.uid WHERE n.status = 1 AND p.mail_digest <> '' ORDER BY n.title, p.mail_digest"));

  while ($project = db_fetch_object($projects)) {
    $category = '';
    $body = '';
    $issues = db_query(db_rewrite_sql('SELECT p.nid, n.title, n.created, p.sid, p.category, p.component, p.priority, p.assigned, u.name AS assigned_name FROM {project_issues} p INNER JOIN {node} n ON p.nid = n.nid INNER JOIN {users} u ON u.uid = p.assigned WHERE n.status = 1 AND p.pid = %d AND p.sid = 1 AND p.priority = 1 ORDER BY p.category, n.created DESC', 'p'), $project->nid);
    while ($node = db_fetch_object($issues)) {
      if ($category != $node->category) {
        $category = $node->category;
        $banner = "$project->title / ". project_issue_category($node->category);
        $body .= "$banner\n". str_repeat('-', $banner) ."\n";
      }
      $body .= "$node->title\n";
      if ($node->assigned) {
        $body .= '  assigned: '. $node->assigned_name ."\n";
      }
      $body .= '  age: '. format_interval(time() - $node->created) ."\n";
      $body .= '  url: '. url("node/$node->nid", array('absolute' => TRUE)) ."\n";
      $body .= "\n";
    }

    if (!empty($body)) {
      // We can pass $project here because user_preferred_language()
      // only needs $account->language to function, and we have that
      // loaded into the $project object.
      $language = user_preferred_language($project);
      $mailto = $project->mail_digest;
      $params['project'] = $project;
      $params['body'] = $body;
      drupal_mail('project_issue', 'project_issue_critical_summary', $mailto, $language, $params, $mailto);
    }
  }
}

