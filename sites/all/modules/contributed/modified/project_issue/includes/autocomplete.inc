<?php
// $Id: autocomplete.inc,v 1.2 2009/02/07 04:50:28 dww Exp $


/**
 * @file
 * Autocomplete callback functions for the Project issue tracking module.
 *
 * Each function returns a JSON string for use with JS autocomplete fields.
 */

/**
 * Return valid issue-enabled project names for comma-separated input.
 */
function project_issue_autocomplete_issue_project($string = '') {
  $matches = array();

  // The user enters a comma-separated list of projects. We only autocomplete
  // the last one.
  $array = drupal_explode_tags($string);
  $last_string = trim(array_pop($array));

  if ($last_string != '') {
    $result = db_query_range(db_rewrite_sql("SELECT n.title FROM {node} n INNER JOIN {project_issue_projects} p ON n.nid = p.nid WHERE n.status = %d AND LOWER(n.title) LIKE LOWER('%%%s%%') AND p.issues = %d"), 1, $last_string, 1, 0, 10);

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    while ($project = db_fetch_object($result)) {
      $title = $project->title;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $title = '"'. str_replace('"', '""', $project->title) .'"';
      }
      $matches[$prefix . $title] = check_plain($project->title);
    }
  }

  drupal_json($matches);
}


/**
 * Return valid issue-enabled project names based on a user's own projects.
 *
 * Only returns matches for project titles from issues the user has either
 * submitted or commented on.
 */
function project_issue_autocomplete_user_issue_project($uid, $string = '') {
  $matches = array();

  // The user enters a comma-separated list of projects. We only autocomplete
  // the last one.
  $array = drupal_explode_tags($string);
  $last_string = trim(array_pop($array));

  if ($last_string != '') {
    // We have to do a DISTINCT() here because with the LEFT JOIN on
    // {comments}, we can get a lot of duplicate matches, and then our range
    // limit will prevent us from showing all the distinct options.
    $result = db_query_range(db_rewrite_sql("SELECT DISTINCT(n.title) FROM {node} n INNER JOIN {project_issue_projects} pip ON n.nid = pip.nid INNER JOIN {project_issues} pi ON n.nid = pi.pid INNER JOIN {node} pin ON pi.nid = pin.nid LEFT JOIN {comments} c ON c.nid = pi.nid WHERE pip.issues = %d AND n.status = %d AND pin.status = %d AND (pin.uid = %d OR c.uid = %d) AND LOWER(n.title) LIKE LOWER('%%%s%%')"), 1, 1, 1, $uid, $uid, $last_string, 0, 10);

    $prefix = count($array) ? implode(', ', $array) .', ' : '';
    while ($project = db_fetch_object($result)) {
      $title = $project->title;
      // Commas and quotes in terms are special cases, so encode 'em.
      if (strpos($title, ',') !== FALSE || strpos($title, '"') !== FALSE) {
        $title = '"'. str_replace('"', '""', $project->title) .'"';
      }
      $matches[$prefix . $title] = check_plain($project->title);
    }
  }

  drupal_json($matches);
}

