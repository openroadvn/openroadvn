<?php
// $Id: issue_node_view.inc,v 1.1 2009/06/18 03:28:55 dww Exp $

/**
 * @file
 * Code required when viewing an issue node.
 */

/**
 * Private helper to implement hook_view().
 */
function _project_issue_view($node, $teaser = FALSE, $page = FALSE) {
  $node = node_prepare($node, $teaser);

  if (!$teaser && ($page || project_issue_is_comment_reply())) {

    $node->content['#prefix'] = '<div class="project-issue">';
    $node->content['#suffix'] = '</div>';

    $project = node_load(array('nid' => $node->project_issue['pid'], 'type' => 'project_project'));
    $release->nid = $node->project_issue['rid'];
    if (module_exists('project_release')) {
      $release = project_release_load($release);
    }
    $assigned = ($node->project_issue['assigned'] && ($account = user_load(array('uid' => $node->project_issue['assigned']))) ? $account->name : t('Unassigned'));

    $current_data = array();
    $current_data['pid'] = array(
      'label' => t('Project'),
      'current' => $project->title,
    );
    if (!empty($release->project_release['version'])) {
      $current_data['rid'] = array(
        'label' => t('Version'),
        'current' => $release->project_release['version'],
      );
    }
    $current_data['component'] = array(
      'label' => t('Component'),
      'current' => $node->project_issue['component'],
    );
    $current_data['category'] = array(
      'label' => t('Category'),
      'current' => project_issue_category($node->project_issue['category'], 0,$node->project_issue['pid']),
    );
    $current_data['priority'] = array(
      'label' => t('Priority'),
      'current' => project_issue_priority($node->project_issue['priority']),
    );
    $current_data['assigned'] = array(
      'label' => t('Assigned'),
      'current' => $assigned,
    );
    $current_data['sid'] = array(
      'label' => t('Status'),
      'current' => project_issue_state($node->project_issue['sid']),
    );
    $current_data['nid'] = array(
      'label' => t('Identifier'),
      'current' => $node->project_issue['nid'],
    );

    // Allow modules to alter the metadata displayed in the table on the actual
    // issue node itself (at the very top of the issue). Modules should accept
    // the $current_data parameter by reference and add additional
    // elements for additional lines in the table.
    //
    // Modules implementing this hook should take the following parameters:
    // @param $view
    //  A string representing the metadata view being generated.  For the issue
    //  node main table, this will be 'current'.
    // @param $node
    //  The project_issue node object.
    // @param $current_data
    //  An associative array of rows in the project issue metadata table that
    //  will be displayed, with the following key/value pairs:
    //    'label' => The metadata label.
    //    'current' => The current metadata value.
    //  This parameter should be accepted by reference.
    foreach (module_implements('project_issue_metadata') as $module) {
      $function = $module .'_project_issue_metadata';
      $function('current', $node, $current_data);
    }

    $node->content['project_issue_summary'] = array(
      '#value' => theme('project_issue_summary', $current_data, project_issue_internal_links($node)),
      '#weight' => -5,
    );

    $node->content['project_issue_header'] = array(
      '#value' => '<div class="header">'. t('Description') .'</div>',
      '#weight' => -3,
    );

    project_issue_set_breadcrumb($node, $project);
  }
  return $node;
}

/**
 * Themes the metadata table and internal page links for issue nodes.
 *
 * @param $current_data
 *   An array of current issue data for the metadata table.
 * @param $summary_links
 *   An array of internal page links.
 * @return
 *   An HTML string of the summary section.
 */
function theme_project_issue_summary($current_data, $summary_links) {
  $allowed_tags = array('a', 'em', 'strong', 'div', 'span');
  // Fields that should be rendered as plain text, not filtered HTML.
  $plain_fields = array('title', 'pid', 'rid');

  $rows = array();
  foreach ($current_data as $name => $values) {
    $row = array();
    $row[] = filter_xss($values['label'], $allowed_tags) .':';
    if (in_array($name, $plain_fields)) {
      $row[] = check_plain($values['current']);
    }
    else {
      $row[] = filter_xss($values['current'], $allowed_tags);
    }
    $rows[] = $row;
  }

  $output = '<div id="project-summary-container" class="clear-block">';
  $output .= '<div id="project-issue-summary-table" class="summary">'. theme('table', array(), $rows) .'</div>';
  if (!empty($summary_links)) {
    $output .= '<div id="project-issue-summary-links">'. theme('item_list', $summary_links, t('Jump to:'), 'ul', array('class' => 'internal-links')) .'</div>';
  }
  $output .= '</div>';

  return $output;
}

/**
 * Generates internal page links for issue pages.
 *
 * @param $node
 *   The issue node.
 * @return
 *   An array of internal page links.
 */
function project_issue_internal_links($node) {
  $links = array();

  if ($node->comment != COMMENT_NODE_DISABLED) {
    // Link to the first unread, or most recent comment.
    if (comment_num_new($node->nid)) {
      // There are unread replies; link to first unread comment.
      $links[] = l(t('First unread comment'), "node/$node->nid", array('fragment' => 'new'));
    }
    else {
      // No unread replies; link to most recent comment.
      $comment_cid = db_result(db_query_range("SELECT pic.cid FROM {project_issue_comments} pic INNER JOIN {node} n on pic.nid = n.nid WHERE n.status = 1 AND n.nid = %d ORDER BY pic.cid DESC", $node->nid, 0, 1));
      if ($comment_cid) {
        $links[] = l(t('Most recent comment'), "node/$node->nid", array('fragment' => "comment-$comment_cid"));
      }
    }
    // Link for most recent patch.
    $file_cid = db_result(db_query_range("SELECT cu.cid FROM {comment_upload} cu INNER JOIN {node} n on cu.nid = n.nid WHERE n.status = 1 AND n.nid = %d ORDER BY cu.fid DESC", $node->nid, 0, 1));
    if ($file_cid) {
      $links[] = l(t('Most recent attachment'), "node/$node->nid", array('fragment' => "comment-$file_cid"));
    }
  }

  // Link straight to comment form.
  if ($node->comment == COMMENT_NODE_READ_WRITE && (user_access('post comments') || user_access('post comments without approval'))) {
    // TODO: This conditional needs to be ripped out in D6.
    $comment_form_location = isset($node->project_issue['comment_form_location']) ? $node->project_issue['comment_form_location'] : variable_get('comment_form_location_' . $node->type, COMMENT_FORM_SEPARATE_PAGE);

    // Comment form isn't on the page, link to the comment reply page.
    if ($comment_form_location == COMMENT_FORM_SEPARATE_PAGE) {
      $links[] = l(t('Add new comment'), "comment/reply/$node->nid");
    }
    // Comment form is on the page, generate an internal page link for it.
    else {
      $links[] = l(t('Add new comment'), "node/$node->nid", array('fragment' => "comment-form-title"));
    }
  }

  return $links;
}

