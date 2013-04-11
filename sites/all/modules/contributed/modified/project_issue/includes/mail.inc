<?php
// $Id: mail.inc,v 1.123 2010/01/17 00:16:15 dww Exp $

function project_issue_mailhandler($node, $result, $i, $header, $mailbox) {
  if ($node->type == 'project') {
    if (node_access('create', 'project_issue')) {
      $node->nid = preg_replace('/@.+/', '', $node->nid);

      if ($node->nid) {
        /*
        ** Base the new entry on the node it belongs to, this ensures all
        ** values are initially correct.
        */
        $entry = node_load(array('nid' => $node->nid, 'type' => 'project_issue'));
      }

      // Possible attributes
      $fields = array(
        'pid' => t('Project'),
        'category' => t('Category'),
        'component' => t('Component'),
        'priority' => t('Priority'),
        'rid' => t('Version'),
        'assigned' => t('Assigned to'),
        'sid' => t('Status')
      );

      /*
      ** Only change the title if it doesn't have the old title in it.
      ** This should prevent the title from changing due to added
      ** prefixes. It may on occasion make false positives, but if
      ** a title change is that minor who cares?
      */
      $entry->title = (strpos($node->title, $entry->title)) ? $entry->title : $node->title;

      $entry->teaser = $node->teaser;
      $entry->body = $node->body;
      $entry->uid = $node->uid;

      foreach ($fields as $var => $text) {
        $text = strtolower(str_replace(' ', '_', $text));
        if (isset($node->project_issue[$text])) {
          $node->project_issue[$text] = trim($node->project_issue[$text]);
          switch ($var) {
            case 'pid':
              $project = node_load($node->project_issue[$text]);
              if ($project->nid) {
                $entry->project_issue['pid'] = $project->nid;
              }
              break;
            case 'category':
              if (($category = array_search($node->project_issue[$text], project_issue_category(0, 0)))) {
                $entry->project_issue['category'] = $category;
              }
              break;
            case 'priority':
              if (($priority = array_search($node->project_issue[$text], project_issue_priority()))) {
                $entry->project_issue['priority'] = $priority;
              }
              break;
            case 'rid':
              if ($entry->project_issue['pid'] && ($nid = db_result(db_query("SELECT nid FROM {project_release_nodes} WHERE pid = %d AND version = '%s'", $entry->project_issue['pid'], $node->project_issue[$text]), 0))) {
                $entry->project_issue['rid'] = $nid;
              }
              break;
            case 'assigned':
              if ($user = user_load(array('name' => $node->project_issue[$text]))) {
                $entry->project_issue['assigned'] = $user->uid;
              }
              break;
            case 'sid':
              if (($state = array_search($node->project_issue[$text], project_issue_state()))) {
                $entry->project_issue['sid'] = $state;
              }
              break;
            case 'component':
              if ($entry->project_issue['pid'] && ($project = node_load(array('nid' => $entry->project_issue['pid'], 'type' => 'project_project')))) {
                if ($project && in_array($node->project_issue[$text], $project->project_issue['components'])) {
                  $entry->project_issue['component'] = $node->project_issue[$text];
                }
              }
              break;
          }
        }
      }

      if (empty($entry->nid)) {
        $entry->sid = variable_get('project_issue_default_state', 1);
        $entry->type = 'project_issue';
        $entry = node_validate($entry, $error);
        $error or ($entry->nid = node_save($entry));
      }
      else {
        $error = project_comment_validate($entry);
        $error or project_comment_save($entry);
      }
    }
    else {
      $error['user'] = t('You are not authorized to access this page.');
    }

    if ($error && $mailbox['replies']) {
      // Send the user his errors
      $mailto = mailhandler_get_fromaddress($header, $mailbox);
      $mailfrom = variable_get('site_mail', ini_get('sendmail_from'));
      $headers = array(
        'X-Mailer' => 'Drupal Project module (http://drupal.org/project/project)',
      );

      $body = t('You had some errors in your submission:');
      foreach ($error as $field => $text) {
        $body .= "\n * $field: $text";
      }

      drupal_mail('project_issue_mailhandler_error', $mailto, t('E-mail submission to !sn failed - !subj', array('!sn' => variable_get('site_name', 'Drupal'), '!subj' => $header->subject)), $body, $mailfrom, $headers);
    }

    // Return a NULL result so mailhandler doesn't save the node using the default methods.
    return NULL;
  }
  else {
    return $node;
  }
}

function project_mail_urls($url = 0) {
  static $urls = array();
  if ($url) {
    // If $url is an internal link (eg. '/project/project'), such
    // as might be returned from the url() function with the
    // $absolute parameter set to FALSE, we must remove
    // the leading slash before passing this path through the url()
    // function again, or otherwise we'll get two slashes in a row
    // and thus a bad URL.
    if (substr($url, 0, 1) == '/') {
      $url = substr($url, 1);
    }
    $urls[] = strpos($url, '://') ? $url : url($url, array('absolute' => TRUE));
    return count($urls);
  }
  return $urls;
}

function project_mail_output(&$body, $html = 1, $format = FILTER_FORMAT_DEFAULT) {
  static $i = 0;

  if ($html) {
    $body = check_markup($body, $format, FALSE);
    $pattern = '@<a +([^ >]+ )*?href *= *"([^>"]+?)"[^>]*>([^<]+?)</a>@ei';
    $body = preg_replace($pattern, "'\\3 ['. project_mail_urls('\\2') .']'", $body);
    $urls = project_mail_urls();
    if (count($urls)) {
      $body .= "\n";
      for ($max = count($urls); $i < $max; $i++) {
        $body .= '['. ($i + 1) .'] '. $urls[$i] ."\n";
      }
    }

    $body = preg_replace('!</?blockquote>!i', '"', $body);
    $body = preg_replace('!</?(em|i)>!i', '/', $body);
    $body = preg_replace('!</?(b|strong)>!i', '*', $body);
    $body = preg_replace("@<br />(?!\n)@i", "\n", $body);
    $body = preg_replace("@</p>(?!\n\n)@i", "\n\n", $body);
    $body = preg_replace("@<li>@i", "* ", $body);
    $body = preg_replace("@</li>\n?@i", "\n", $body);
    $body = strip_tags($body);
    $body = decode_entities($body);
    $body = wordwrap($body, 72);
  }
  else {
    $body = decode_entities($body);
  }
}

function project_mail_notify($nid) {
  global $user;

  if (defined('PROJECT_NOMAIL')) {
    return;
  }

  $node = node_load($nid, NULL, TRUE);
  $project = node_load(array('nid' => $node->project_issue['pid'], 'type' => 'project_project'));

  // Store a copy of the issue, so we can load the original issue values
  // below.
  $issue = drupal_clone($node);

  // Load in the original issue data here, since we want a running
  // reverse history.
  $original_issue_data = unserialize($node->project_issue['original_issue_data']);
  $fields = project_issue_field_labels('email');
  foreach ($fields as $field => $label) {
    if ($field != 'name' && $field != 'updator') {
      $issue->original_issue_metadata->$field = $original_issue_data->$field;
    }
  }

  // Record users that are connected to this issue.
  $uids = array();
  if (!empty($node->uid)) {
    $uids[$node->uid] = $node->uid;
  }
  if (!empty($node->project_issue['assigned'])) {
    $uids[$node->project_issue['assigned']] = $node->project_issue['assigned'];
  }

  // Create complete history of the bug report.
  $history = array($issue);
  $result = db_query('SELECT u.name, c.cid, c.nid, c.subject, c.comment, c.uid, c.format, pic.* FROM {project_issue_comments} pic INNER JOIN {comments} c ON c.cid = pic.cid INNER JOIN {users} u ON u.uid = c.uid WHERE c.nid = %d AND c.status = %d ORDER BY pic.timestamp', $node->nid, COMMENT_PUBLISHED);

  while ($comment = db_fetch_object($result)) {
    $comment->comment = db_decode_blob($comment->comment);
    $comment->files = comment_upload_load_files($comment->cid);
    $history[] = $comment;
    // Record users that are connected to this issue.
    if ($comment->uid) {
      $uids[$comment->uid] = $comment->uid;
    }
  }

  if (count($uids)) {
    $placeholders = implode(',', array_fill(0, count($uids), '%d'));
    array_unshift($uids, $node->project_issue['pid']);
    $result = db_query("SELECT p.*, u.uid, u.name, u.mail FROM {project_subscriptions} p INNER JOIN {users} u ON p.uid = u.uid WHERE u.status = 1 AND p.nid = %d AND (p.level = 2 OR (p.level = 1 AND u.uid IN ($placeholders)))", $uids);
  }
  else {
    $result = db_query('SELECT p.*, u.uid, u.name, u.mail FROM {project_subscriptions} p INNER JOIN {users} u ON p.uid = u.uid WHERE u.status = 1 AND p.nid = %d AND p.level = 2', $node->project_issue['pid']);
  }

  // To save workload, check here if either the anonymous role or the
  // authenticated role has the 'view uploaded files' permission, since
  // we only need to process each user's file access permission if this
  // is NOT the case.
  $check_file_perms = !db_result(db_query("SELECT COUNT(*) FROM {permission} WHERE perm LIKE '%view uploaded files%' AND rid IN (%d, %d)", DRUPAL_ANONYMOUS_RID, DRUPAL_AUTHENTICATED_RID));

  // We need to determine if node_access() checks are necessary.  The
  // check will be needed if any of the following is true:
  //   1. The node is not published.
  //   2. There is at least on node access control module enabled.
  //   3. Both the anonymous and authenticated user do not have
  //      the 'access project issues' permission.
  $allowed_roles = user_roles(FALSE, 'access project issues');
  if (isset($allowed_roles[DRUPAL_ANONYMOUS_RID]) || isset($allowed_roles[DRUPAL_AUTHENTICATED_RID])) {
    $anon_auth_access = TRUE;
  }
  $grants = module_implements('node_grants');
  $check_node_access = $node->status != 1 || empty($anon_auth_access) || !empty($grants);

  $params['node']    = $node;
  $params['project'] = $project;
  $params['history'] = $history;

  $sender->name = t('!name (!site)', array('!name' => $user->name, '!site' => variable_get('site_name', 'Drupal')));
  $sender->mail = strtr(variable_get('project_issue_reply_to', variable_get('site_mail', ini_get('sendmail_from'))), array('%project' => $project->project['uri']));
  // The sender name is enclosed by double quotes below
  // to satisfy RFC2822 <http://www.faqs.org/rfcs/rfc2822.html>,
  // which requires double quotes when special characters (including
  // some punctuation) are used.  See example in Appendix A.1.2.
  $from = '"' . mime_header_encode($sender->name) . "\" <$sender->mail>";

  while ($recipient = db_fetch_object($result)) {
    // To save work, only go through a user_load if we need it.
    if ($check_file_perms || $check_node_access) {
      $account = user_load(array('uid' => $recipient->uid));
      $language = user_preferred_language($account);
    }
    else {
      $language = language_default();
    }

    $can_access = $check_node_access ? node_access('view', $node, $account) : TRUE;

    if ($can_access) {
      $display_files = $check_file_perms ? user_access('view uploaded files', $account) : TRUE;

      $params['display_files'] = $display_files;
      drupal_mail('project_issue', 'project_issue_update_notification', $recipient->mail, $language, $params, $from);
    }
  }

  if (is_array($project->project_issue['mail_copy_filter']) && count(array_filter($project->project_issue['mail_copy_filter'])) && !$project->project_issue['mail_copy_filter'][$node->project_issue['category']]) {
    return;
  }

  if (is_array($project->project_issue['mail_copy_filter_state']) && count(array_filter($project->project_issue['mail_copy_filter_state'])) && !$project->project_issue['mail_copy_filter_state'][$node->project_issue['sid']]) {
    return;
  }

  if (!empty($project->project_issue['mail_copy'])) {
    $params['display_files'] = TRUE;
    $message['body'][] = $links;
    $message['body'][] = project_mail_generate_followup_mail_body($node, $history, TRUE);
    drupal_mail('project_issue', 'project_issue_update_notification', $project->project_issue['mail_copy'], language_default(), $params, $from);
  }
}

/*
 * Implementation of hook_mail()
 */
function project_issue_mail($key, &$message, $params) {
  global $base_url;

  switch ($key) {
    case "project_issue_update_notification":
      // There could be stale data in the cached node, so reset the cache.
      $node    = $params['node'];
      $project = $params['project'];
      $history = $params['history'];
      $fields  = project_issue_field_labels('email');

      $domain = preg_replace('|.+://([a-zA-Z0-9\._-]+).*|', '\1', $base_url);

      $message['headers'] += array(
        'Date' => date('r'),
        'X-Mailer' => 'Drupal Project module (http://drupal.org/project/project)',
        'List-Id' => "$project->title <". $project->project['uri'] ."-issues-$domain>",
        'List-Archive' => '<'. url('project/issues/'. $project->project['uri'], array('absolute' => TRUE)) .'>',
        'List-Subscribe' => '<'. url('project/issues/subscribe-mail/'. $project->project['uri'], array('absolute' => TRUE)) .'>',
        'List-Unsubscribe' => '<'. url('project/issues/subscribe-mail/'. $project->project['uri'], array('absolute' => TRUE)) .'>'
      );

      // Comments exist, set headers accordingly.
      if (count($history) > 1) {
        foreach ($history as $comment) {
          // We need the most recent cid and the next most recent cid for the
          // message headers.  Instead of issuing another query, just keep track
          // of them here.
          $previous_cid = isset($cid) ? $cid : '';
          $cid = isset($comment->cid) ? $comment->cid : 0;
        }
        $message['headers']['Message-Id'] = "<type=project&nid=$node->nid&cid=$cid&host=@$domain>";
        $message['headers']['In-Reply-To'] = "<type=project&nid=$node->nid&host=@$domain>";
        $message['headers']['References'] = "<type=project&nid=$node->nid&host=@$domain> <type=project&nid=$node->nid&cid=$previous_cid&host=@$domain> <type=project&nid=$node->nid&revcount=1&host=@$domain>";
      } else {
        // Only original issue in this email.
        $message['headers']['Message-Id'] = "<type=project&nid=$node->nid&host=@$domain>";
      }

      project_mail_output($node->title, 0);
      $message['subject'] = t('[!short_name] [!category] !title', array('!short_name' => $project->project['uri'], '!category' => $node->project_issue['category'], '!title' => $node->title));

      // Create link to related node
      $links = t('Issue status update for !link', array('!link' => "\n". url("node/$node->nid", array('absolute' => TRUE)))) ."\n";
      $links .= t('Post a follow up: !link', array('!link' => "\n". url("comment/reply/$node->nid", array('fragment' => 'comment-form-title', 'absolute' => TRUE)))) ."\n";
      $message['body'][] = $links;
      $message['body'][] = project_mail_generate_followup_mail_body($node, $history, $params['display_files']);

      break;

    case 'project_issue_critical_summary':
      $project = $params['project'];
      $message['headers'] += array(
        'Date' => date('r'),
        'X-Mailer' => 'Drupal Project Issues module (http://drupal.org/project/project_issue)',
        'List-Id' => "$project->title <". preg_replace('|.+://([a-zA-Z0-9\._-]+).*|', '\1', $base_url) .'-project-issues-digest>',
        'List-Archive' => '<'. url('project/issues', array('query' => array('priorities' => '1'), 'absolute' => TRUE)) .'>',
      );
      $message['subject'] = t('Release critical bugs for !date', array('!date' => date('F d, Y', time())));
      $message['body'][] = $params['body'];
      break;

    case 'project_issue_reminder':
      $sender->name = variable_get('site_name', '');
      $sender->mail = variable_get('site_mail', '');
      $message['headers'] += array(
        'Return-Path' => "<$sender->mail;>",
        'Date' => date('r'),
        'From' => "$sender->name <$sender->mail>",
        'X-Mailer' => 'Drupal Project Issues module (http://drupal.org/project/project_issue)',
        'List-Id' => "$sender->name <project-reminder-". preg_replace('|.+://([a-zA-Z0-9\._-]+).*|', '\1', $base_url) .'>',
        'List-Archive' => '<'. url('project', array('absolute' => TRUE)) .'>',
      );
      $message['subject'] = t('Your submitted bugs for !date', array('!date' => date('F d, Y', time())));
      $message['body'][] = $params['body'];
      break;
  }
}

/**
 * Format the body of an issue followup email.
 *
 * @param $node
 *   The issue node.
 * @param $history
 *   An array containing the history of issue followups.
 * @param $display_files
 *   Boolean indicating if file attachments should be displayed.
 * @return
 *   A string of the email body.
 */
function project_mail_generate_followup_mail_body($node, $history, $display_files) {
  global $user;
  static $output_with_files =  NULL, $output_without_files = NULL;

  // Return cached output if available.
  if ($display_files) {
    if (isset($output_with_files)) {
      return $output_with_files;
    }
  }
  else {
    if (isset($output_without_files)) {
      return $output_without_files;
    }
  }

  // Get most recent update.
  $entry = array_pop($history);

  $node->project_issue['updator'] = $entry->name ? $entry->name : $user->name;

  // Check if the latest entry is actually the initial issue.
  if (empty($history)) {
    $metadata_previous = new stdClass();
    // Have to get the metadata into the entry object.
    $metadata_entry = $entry->original_issue_metadata;
    $content = $entry->body;
  }
  else {
    $metadata_previous = end($history);
    // If the previous was the original issue, then we need to pull
    // out the metadata from project_issue.
    if (isset($metadata_previous->original_issue_metadata)) {
      $metadata_previous = $metadata_previous->original_issue_metadata;
    }
    $metadata_entry = $entry;
    $content = $entry->comment;
  }

  $fields = project_issue_field_labels('email');
  $comment_changes = project_issue_metadata_changes($node, $metadata_previous, $metadata_entry, $fields);

  // Since $node->name will always be the original issue author, and since
  // $node->project_issue['updator'] isn't a property of either $previous or
  // $entry, these two properties will never show up as being different when
  // project_issue_metadata_changes() is called, and therefore neither of
  // these will ever be elements of the $comment_changes array.  Since we do
  // want them to be printed in issue emails, we just need to add their labels
  // back into the $comment_changes array here, so that
  // theme_project_issue_mail_summary_field() will know to print the data for
  // these two fields.
  $comment_changes['name'] = array(
    'label' => $fields['name'],
  );
  $comment_changes['updator'] = array(
    'label' => $fields['updator'],
  );

  $summary = theme('project_issue_mail_summary', $entry, $node, $comment_changes, $display_files);

  // Create main body content
  project_mail_output($content, 1, $entry->format);
  $body = "$content\n$entry->name\n";

  $hr = str_repeat('-', 72);

  if (count($history)) {

    $body .= "\n\n";
    $body .= t('Original issue:') ."\n";
    $body .= project_mail_format_entry(array_shift($history), $display_files, TRUE);
    if (count($history)) {
      $body .= "\n". t('Previous comments (!count):', array('!count' => count($history))) ."\n";
      foreach ($history as $entry) {
        $body .= project_mail_format_entry($entry, $display_files);
      }
    }
  }

  $output = "$summary\n$body";

  // Set cached output.
  if ($display_files) {
    $output_with_files = $output;
  }
  else {
    $output_without_files = $output;
  }

  return $output;
}

/**
 * Themes the display of the issue metadata summary
 * that is shown at the top of an issue emai.
 *
 * @param $entry
 *  The object representing the current entry.  This will be a node object
 *  if the current entry is the original issue node; otherwise this will be
 *  a comment object.
 * @param $node
 *  The original issue node object.
 * @param $changes
 *  A nested array containing the metadata changes between the original
 *  issue and the first comment, or two consecutive comments.  This array
 *  is the output of the project_issue_metadata_changes() function.
 * @param $display_files
 *   Boolean indicating if file attachments should be displayed.
 * @return
 *   A string containing the themed text of the issue metadata table.
 */
function theme_project_issue_mail_summary($entry, $node, $changes, $display_files) {
  // Mail summary (status values).
  $summary = '';
  foreach ($changes as $field => $change) {
    $summary .= theme('project_issue_mail_summary_field', $node, $field, $change);
  }

  $summary .= project_mail_format_attachments($entry, $display_files);
  return $summary;
}

/**
 * Theme the email output of one project issue metadata field.
 *
 * @param $node
 *   The project issue node object.
 * @param $field_name
 *   The name of the field to theme.
 * @param $change
 *   A nested array containing changes to project issue metadata
 *   for the given issue or comment.
 * @return
 *  A themed line or lines of text ready for inclusion into the email body.
 */
function theme_project_issue_mail_summary_field($node, $field_name, $change) {
  // We need to run the label name through strip_tags here so that
  // the spacing isn't messed up if there are HTML tags in $change['label'].
  $text = str_pad(strip_tags($change['label']). ':', 14);
  $summary_row = '';
  if (!empty($change['label']) && isset($change['old']) && isset($change['new']) && $field_name != 'updator' && $field_name != 'name') {
    if (is_array($change['old']) || is_array($change['new'])) {
      $removed = array();
      if (is_array($change['old'])) {
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

      $summary_row = " $text". trim(implode(', ', $removed). '  ' .implode(', ', $added)) ."\n";
    }
    else {
      $summary_row .= "-$text". project_issue_change_summary($field_name, $change['old']) ."\n";
      $summary_row .= "+$text". project_issue_change_summary($field_name, $change['new']) ."\n";
    }
  }
  elseif (!empty($change['label'])) {
    if (!empty($change['new'])) {
      // This condition is necessary when building the first email message of an
      // issue, since in this case $change['old'] should not exist.
      if (is_array($change['new'])) {
        $summary_row .= " $text". implode(', ', $change['new']) ."\n";
      }
      else {
        $summary_row .= " $text". project_issue_change_summary($field_name, $change['new']) ."\n";
      }
    }
    else {
      // This condition is where fields that are stored in the $node object and
      // which haven't changed but should be printed anyway get processed.
      // For example, the project, category, etc. are printed in each email
      // whether or not they have changed.
      // @TODO: Should we really assume the field in is $node->project_issue[]?
      if (isset($node->project_issue[$field_name])) {
        $summary_row .= " $text". project_issue_change_summary($field_name, $node->project_issue[$field_name]) ."\n";
      }
    }
  }
  // HTML tags in the email will make it hard to read, so pass
  // this output through strip_tags().
  return strip_tags($summary_row);
}

/**
 * Formats attachments for issue notification e-mails.
 *
 * @param $entry
 *   An issue or followup object containing the file data.
 * @param $display_files
 *   Boolean indicating if file attachments should be displayed.
 * @return
 *   A formatted string of file attachments.
 */
function project_mail_format_attachments($entry, $display_files) {
  $output = '';
  if ($display_files && is_array($entry->files)) {
    foreach ($entry->files as $file) {
      // Comment upload has it's files in an array, so cast to an object
      // for consistency.
      $file = (object) $file;
      $output .= ' '. str_pad(t('Attachment') .':', 14) . file_create_url($file->filepath) .' ('. format_size($file->filesize) .")\n";
    }
  }
  return $output;
}

/**
 * Format an issue entry for display in an email.
 *
 * @param entry
 *   The entry to the formatted.
 * @param $display_files
 *   Boolean indicating if file attachments should be displayed.
 * @param is_original
 *   Whether this entry is the original issue or a followup. Followup issues
 *   will be automatically numbered.
 * @return
 *   Formatted text for the entry.
 */
function project_mail_format_entry($entry, $display_files, $is_original = FALSE) {
  static $history_count = 1;
  $hr = str_repeat('-', 72);
  $output = "$hr\n";

  // Nodes and comments have different stamp fields.
  $timestamp = isset($entry->created) ? $entry->created : $entry->timestamp;

  if (!$is_original) {
    $output .= "$entry->subject -- ";
  }

  $output .= format_date($timestamp, 'large') ." : $entry->name\n";

  if (!$is_original) {
    $output .= url("node/$entry->nid", array('fragment' => "comment-$entry->cid", 'absolute' => TRUE)) ."\n";
  }

  $output .= project_mail_format_attachments($entry, $display_files);

  // Must distinguish between nodes and comments -- here we do it
  // by looking for a revision ID.
  if (empty($entry->vid)) {
    $content = $entry->comment;
  }
  else {
    $content = $entry->body;
  }

  project_mail_output($content, 1, $entry->format);

  if ($content) {
    $output .= "\n$content";
  }
  return $output;
}

