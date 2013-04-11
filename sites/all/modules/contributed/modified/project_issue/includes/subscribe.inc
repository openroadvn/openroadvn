<?php
// $Id: subscribe.inc,v 1.1 2009/06/18 03:24:59 dww Exp $

function project_issue_subscribe($form_state, $project_nid = 0) {
  global $user;

  if (!valid_email_address($user->mail)) {
    drupal_set_message(t('You need to provide a valid e-mail address to subscribe to issue e-mails. Please edit your user information.'), 'error');
    drupal_goto('user/'. $user->uid .'/edit');
  }

  $levels = array(0 => t('None'), 1 => t('Own issues'), 2 => t('All issues'));

  if ($project_nid) {
    if (!is_numeric($project_nid)) {
      $project_nid = db_result(db_query(db_rewrite_sql("SELECT p.nid FROM {project_projects} p WHERE p.uri = '%s'", 'p'), $project_nid));
    }
    if (!$project_nid) {
      return drupal_not_found();
    }

    $project = node_load($project_nid);
    project_project_set_breadcrumb($project, TRUE);

    $level = db_result(db_query('SELECT level FROM {project_subscriptions} WHERE nid = %d AND uid = %d', $project->nid, $user->uid));
    $form['single'] = array(
      '#type' => 'value',
      '#value' => $project->nid,
    );
    $form['#project'] = array(
      '#type' => 'value',
      '#value' => $project,
    );
    $form['subscribe'] = array(
      '#type' => 'markup',
      '#value' => '<p>'. t('Subscribe to receive e-mail notification when an issue for this project is updated.') .'</p>',
    );
    $form['options']['#tree'] = TRUE;
    $form['options'][$project->nid] = array(
      '#type' => 'radios',
      '#title' => t('Subscribe to @project issues', array('@project' => $project->title)),
      '#default_value' => isset($level) ? $level : 0,
      '#options' => $levels,
    );

  }
  else {

    $form['buttons']['all'] = array(
      '#type' => 'markup',
      '#value' => t('All projects'),
    );
    foreach ($levels as $key => $level) {
      $form['buttons'][$level] = array(
        '#type' => 'submit',
        '#name' => 'all',
        '#value' => $level,
      );
    }
    $nids = array();

    $result = db_query(db_rewrite_sql("SELECT s.nid, n.title, s.level, p.uri FROM {project_subscriptions} s INNER JOIN {node} n ON n.nid = s.nid INNER JOIN {project_projects} p ON n.nid = p.nid WHERE n.type = 'project_project' AND n.status = 1 AND s.uid = %d ORDER BY n.title", 's'), $user->uid);
    while ($project = db_fetch_object($result)) {
      $form['project'][$project->nid]['title'] = array(
        '#value' => l($project->title, "node/$project->nid"),
      );
      foreach ($levels as $key => $level) {
        if ($project->level == $key) {
          $status[$project->nid] = $key;
        }
      }
      $nids[] = $project->nid;
    }

    if (empty($nids)) {
      $placeholders = '';
    }
    else {
      $placeholders = " AND n.nid NOT IN (". implode(',', array_fill(0, count($nids), '%d')) .")";
    }

    $result = db_query(db_rewrite_sql("SELECT n.nid, n.title, p.uri FROM {node} n INNER JOIN {project_projects} p ON n.nid = p.nid WHERE n.type = 'project_project' AND n.status = 1". ($nids ?  $placeholders : "") ." ORDER BY n.title"), $nids);
    while ($project = db_fetch_object($result)) {
      $form['project'][$project->nid]['title'] = array(
        '#value' => l($project->title, "node/$project->nid"),
      );
      $nids[] = $project->nid;
    }

    foreach ($nids as $nid) {
      $form['options']['#tree'] = TRUE;
      $form['options'][$nid] = array(
        '#type' => 'radios',
        '#default_value' => isset($status[$nid]) ? $status[$nid] : 0,
        '#options' => $levels,
      );
    }
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Subscribe'),
  );
  return $form;
}

function theme_project_issue_subscribe($form) {
  global $user;

  $output = '';

  if (!isset($form['single'])) {
    $levels = array(0 => t('None'), 1 => t('Own issues'), 2 => t('All issues'));
    $headers = array_merge(array(t('Project')), $levels);

    $row = array();
    foreach (element_children($form['buttons']) as $key) {
      $row[] = drupal_render($form['buttons'][$key]);
    }
    $rows = array($row);

    foreach (element_children($form['project']) as $key) {
      $row = array(drupal_render($form['project'][$key]['title']));
      foreach ($levels as $level => $name) {
        $row[] = drupal_render($form['options'][$key][$level]);
      }
      $rows[] = $row;
    }
    $output = theme('table', $headers, $rows);
  }

  $output .= drupal_render($form);
  return $output;
}

function project_issue_subscribe_submit($form, &$form_state) {

  global $user;
  $all = $form_state['clicked_button']['#value'];

  $levels = array(0 => t('None'), 1 => t('Own issues'), 2 => t('All issues'));

    // Remove previous subscriptions for user.
    if (isset($form_state['values']['single'])) {
      db_query('DELETE FROM {project_subscriptions} WHERE nid = %d AND uid = %d', $form_state['values']['single'], $user->uid);
    }
    else {
      db_query('DELETE FROM {project_subscriptions} WHERE uid = %d', $user->uid);
    }

    $_level = array_search($all, $levels);

    foreach ($form_state['values']['options'] as $nid => $level) {
      if ($_level !== 0 && $level !== 0) {
        db_query('INSERT INTO {project_subscriptions} (nid, uid, level) VALUES (%d, %d, %d)', $nid, $user->uid, $_level ? $_level : $level);
      }
    }
    drupal_set_message(t('Subscription settings saved.'));

    if (isset($form['single'])) {
      $form_state['redirect'] = 'project/issues/subscribe-mail/'. $form['#project']['#value']->project['uri'];
    }
    else {
      $form_state['redirect'] = 'project/issues/subscribe-mail';
    }
}
