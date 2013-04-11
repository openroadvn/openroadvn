<?php
// $Id: statistics.inc,v 1.1 2009/06/18 03:24:03 dww Exp $

/**
 * Page callback for the issue statistics page.
 */
function project_issue_statistics($project = NULL) {
  $states = project_issue_state();
  if ($project->nid) {
    $filter = sprintf(' AND p.pid = %d ', (int)$project->nid);
    project_project_set_breadcrumb($project, TRUE);
  }

  $output = '<div class="project-issue">';

  // Issue lifetime.
  $header = array(t('Category'), t('Overall'), t('Last month'));
  $rows = array();
  $duration = time() - 30 * 24 * 60 * 60;
  $result = db_query(db_rewrite_sql('SELECT p.category, SUM(n.changed - n.created) / COUNT(p.category) AS duration FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid WHERE n.status = 1%s AND p.sid > 1 GROUP BY p.category'), $filter);
  while ($stat = db_fetch_object($result)) {
    $rows[$stat->category][0] = project_issue_category($stat->category);
    $rows[$stat->category][1] = array('data' => format_interval($stat->duration, 2), 'class' => 'numeric');
    $rows[$stat->category][2] = array('data' => t('N/A'), 'class' => 'numeric');
  }

  $result = db_query(db_rewrite_sql('SELECT p.category, SUM(n.changed - n.created) / COUNT(p.category) AS duration FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid WHERE n.status = 1%s AND p.sid > 1 AND n.created > %d GROUP BY p.category'), $filter, $duration);
  while ($stat = db_fetch_object($result)) {
    if ($stat->duration > 0) {
      $rows[$stat->category][2] = array('data' => format_interval($stat->duration, 2), 'class' => 'numeric');
    }
  }
  $output .= '<h2>'. t('Average lifetime') .'</h2>';
  $output .= theme('table', $header, $rows);

  $header = array(
    array('data' => t('Status')),
    array('data' => t('Overall'), 'class' => 'project-issue-numeric'),
    array('data' => '%', 'class' => 'project-issue-numeric'),
    array('data' => t('Last month'), 'class' => 'project-issue-numeric'),
  );
  $rows = array();
  // Activity overall.
  $total = db_result(db_query(db_rewrite_sql('SELECT COUNT(p.nid) AS total FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid WHERE n.status = 1%s', 'p'), $filter));
  $result = db_query(db_rewrite_sql('SELECT COUNT(p.nid) AS total, p.sid FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid WHERE n.status = 1%s GROUP BY p.sid', 'p'), $filter);
  while ($stat = db_fetch_object($result)) {
    $rows[$stat->sid][0] = check_plain(project_issue_state($stat->sid));
    $rows[$stat->sid][1] = array('data' => $stat->total, 'class' => 'project-issue-numeric');
    $rows[$stat->sid][2] = array('data' => number_format($stat->total / $total * 100) .'%', 'class' => 'project-issue-numeric-light');
    $rows[$stat->sid][3] = array('data' => '0', 'class' => 'project-issue-numeric');
  }
  // Activity this month.
  $result = db_query(db_rewrite_sql('SELECT COUNT(p.nid) AS total, p.sid FROM {project_issues} p INNER JOIN {node} n ON n.nid = p.nid WHERE n.status = 1%s AND n.changed > %d GROUP BY p.sid', 'p'), $filter, $duration);
  while ($stat = db_fetch_object($result)) {
    $rows[$stat->sid][3] = array('data' => $stat->total, 'class' => 'project-issue-numeric');
  }
  $output .= '<h2>'. t('Issue activity') .'</h2>';
  $output .= theme('table', $header, $rows);

  // Project overview.
  if (!$project->nid) {
    // Even though we don't use the tablesorting logic in the query itself,
    // we include it anyways because we're going to leverage the $_GET arguments
    // to build our own tablesorting mechanism.
    $header = array();
    $header['project'] = array('data' => t('Project'), 'field' => 'title');
    foreach ($states as $key => $value) {
      $header[$key] = array('data' => check_plain($value), 'field' => $key);
    }
    $header['total'] = array('data' => t('Total'), 'field' => 'total');
    // Force sorting arrow to appear on active first.
    $header[1]['sort'] = 'desc';
    $args = array();

    // Since we're pulling the sid to sort by here individually in the first query
    // below, we can bastardize the tablesorting logic to get tablesorting.
    $where = ' AND p.sid = %d';
    $column = 'total';
    if (isset($_GET['order'])) {
      switch ($_GET['order']) {
        case 'Project':
          $where = '';
          $column = 'title';
          break;
        case 'Total':
          $where = '';
          break;
        default:
          if ($state = array_search($_GET['order'], $states)) {
            $args[] = $state;
          }
          else {
            $args[] = 1;
          }
          break;
      }
    }
    else {
      $args[] = 1;
    }
    $sort = (isset($_GET['sort']) && $_GET['sort'] == 'desc') || !isset($_GET['sort']) ? 'DESC' : 'ASC';

    $rows = array();
    $projects = pager_query(db_rewrite_sql("SELECT pn.nid, pn.title, COUNT(n.nid) AS total FROM {node} n INNER JOIN {project_issues} p ON n.nid = p.nid INNER JOIN {node} pn ON p.pid = pn.nid WHERE n.status = 1 AND pn.status = 1$where GROUP BY pn.nid, pn.title ORDER BY $column $sort"), 15, 0, db_rewrite_sql("SELECT COUNT(DISTINCT(n.nid)) FROM {node} n INNER JOIN {project_issues} p ON n.nid = p.pid INNER JOIN {node} pin ON p.nid = pin.nid WHERE n.status = 1 AND pin.status = 1$where"), $args);
    $orig = array('project' => array('data' => 0));
    foreach ($states as $key => $value) {
      $orig[$key] = array('data' => '', 'class' => 'project-issue-numeric');
    }
    $orig['total'] = array('data' => '', 'class' => 'project-issue-numeric');

    while ($project = db_fetch_object($projects)) {
      $rows[$project->nid] = $orig;
      $rows[$project->nid]['project']['data'] = l($project->title, "node/$project->nid");
      $stats = db_query("SELECT sid, COUNT(nid) as total FROM {project_issues} WHERE pid = %d GROUP BY sid", $project->nid);
      while ($stat = db_fetch_object($stats)) {
        $rows[$project->nid]['total']['data'] += $stat->total;
        $rows[$project->nid][$stat->sid]['data'] = $stat->total;
      }
    }
    $output .= '<h2>'. t('Project overview') .'</h2>';
    $output .= '<div class="project-issue-statistics-overview-table">';
    $output .= theme('table', $header, $rows);

    if ($pager = theme('pager', 15, 0)) {
      $output .= $pager;
    }
    $output .= '</div>';
  }

  $output .= '</div>';
  return $output;
}
