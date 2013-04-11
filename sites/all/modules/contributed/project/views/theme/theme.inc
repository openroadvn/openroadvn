<?php
// $Id: theme.inc,v 1.3 2009/01/15 00:29:29 dww Exp $


/**
 * @file
 * Theme functions related to Views support (style plugins, etc).
 */

/**
 * Theme the extra_options form for the project_sort_method filter.
 */
function theme_project_views_project_sort_method_options_form($form) {
  $header = array(
    array('data' => t('Enabled')),
    array('data' => t('ID')),
    array('data' => t('Name')),
    array('data' => t('Weight')),
    array('data' => t('Display name')),
  );
  $rows = array();
  foreach (element_children($form) as $key) {
    $row = array();
    $row[] = drupal_render($form[$key]['enabled']);
    $row[] = drupal_render($form[$key]['id']);
    $row[] = drupal_render($form[$key]['name']);
    $form[$key]['weight']['#attributes']['class'] = 'weight';
    $row[] = drupal_render($form[$key]['weight']);
    $row[] = drupal_render($form[$key]['display_name']);

    $rows[] = array('data' => $row, 'class' => 'draggable');
  }
  if (empty($rows)) {
    $rows[] = array(array('data' => t('No fields available.'), 'colspan' => '2'));
  }

  drupal_add_tabledrag('exposed-sort-weight', 'order', 'sibling', 'weight');
  $output = theme('table', $header, $rows, array('id' => 'exposed-sort-weight'));
  $output .= drupal_render($form);
  return $output;
}

/**
 * Template helper for theme_project_views_view_row_project_node
 */
function template_preprocess_project_views_view_row_project_node(&$vars) {
  $options = $vars['options'];
  $vars['node'] = ''; // make sure var is defined.
  $nid = $vars['row']->nid;
  if (!is_numeric($nid)) {
    return;
  }

  $node = node_load($nid);
  $node->view = $vars['view'];

  if (empty($node)) {
    return;
  }

  $id = $vars['id'];
  $vars['project']->class = ($id % 2) ? 'odd' : 'even';

  // Build the term links for the project.
  if (module_exists('taxonomy') && !empty($options['project_term_links'])) {
    if (!empty($vars['view']->project->project_type)) {
      // Hide the top-level project type term from the links.
      unset($node->taxonomy[$vars['view']->project->project_type->tid]);
    }
    $vars['project']->terms = theme('links', taxonomy_link('taxonomy terms', $node));
  }

  // Because $vars['id'] is one based but $vars['view']>result-># is zero based, subtract
  // one from $vars['id'] to get the corresponding index into $vars['view']->result.
  $result_id = $id - 1;

  $vars['project']->title = l($node->title, "node/$node->nid");
  $vars['project']->new_date = isset($vars['view']->project->project_new_date[$result_id]) ? $node->changed : FALSE;
  $vars['project']->changed = t('Last changed: !interval ago', array('!interval' => format_interval(time() - $node->changed, 2)));
  $vars['project']->body = check_markup($options['project_teaser'] ? $node->teaser : $node->body, $node->format, FALSE);
  if (!empty($options['project_release_download_table'])) {
    $vars['project_release']->download_table = $project->download_table = theme('project_release_table_overview', $node, 'recommended', 'all', t('Version'), FALSE);
  }

  // Link to a downloadable file.
  if (!empty($options['project_release_download_link'])) {
//  @TODO:  Given the potential for confusion with multiple supported versions, I think it might
//  be best to not display a download link at all on the project overview page.
//    if ($node->file_path) {
//      $vars['project_release']->download_link = theme('project_release_download_link', $node->file_path, t('Download'), 'array');
//    }
  }

  // Link to the project's issue queue.
  if (!empty($options['project_issue_issues_link'])) {
    if (isset($node->project_issue['issues'])) {
      $vars['project_issue']->issues_link = array(
        'title' => t('Bugs and feature requests'),
        'href' => 'project/issues/'. $node->project['uri'],
      );
    }
  }
}

/**
 * Preprocess theme function that is used for the project list style.
 */
function template_preprocess_project_views_view_project_list(&$vars) {
  $view = $vars['view'];

  // Sanitize all of the project type term information before placing
  // the values into $vars.
  if (!empty($view->project->project_type)) {
    $term = $view->project->project_type;
    $vars['project']['arguments'][0] = array(
      'tid' => $term->tid,
      'vid' => $term->vid,
      'name' => check_plain($term->name),
      'description' => filter_xss_admin($term->description),
      'weight' => $term->weight,
    );
  }
}

/**
 * Preprocess theme function that is used for the collapsible summary style.
 */
function template_preprocess_project_views_view_collapsible_summary(&$vars) {
  $view = $vars['view'];
  $argument = $view->argument[$view->build_info['summary_level']];

  $url_options = array();
  $terms = array();

  if (!empty($view->exposed_raw_input)) {
    $url_options['query'] = $view->exposed_raw_input;
  }

  $children = array();

  foreach ($vars['rows'] as $id => $row) {
    $name = $argument->summary_name($row);
    $count = intval($row->{$argument->count_alias});
    $tid = $row->term_node_tid;
    $term = taxonomy_get_term($tid);
    //$url = url($view->get_url($args), $url_options);
    $url = taxonomy_term_path($term);

  // @TODO:  We might not want to print terms with no projects so the user doesn't click on the
  // term and end up with a 404 error.

  // @TODO:  We want to include the description of the term as well.

    $vars['rows'][$id]->link = $name;
    $vars['rows'][$id]->url = $url;
    $vars['rows'][$id]->count = $count;
    $vars['rows'][$id]->term = $term;

    // Prepare terms for sending to theme_project_term_list().
    $l_options = array('attributes' => array('title' => strip_tags($term->description)), 'absolute' => TRUE);
    $l_options = array_merge($l_options, $url_options);
    $children[] = l("$name ($count)", $url, $l_options);
  }

  // For each row returned, we store the data in two different ways for use
  // and display by the template.
  $vars['project_terms'] = array(
    '#title' => t('Categories'),
    '#collapsible' => TRUE,
    '#collapsed' => isset($view->original_args[1]) ? TRUE : FALSE,
    '#children' => theme('item_list', $children),
  );
}

/**
 * Preprocess theme function to print a single project record from a row, using fields.
 */
function template_preprocess_project_views_view_row_project_fields(&$vars) {
  $view = $vars['view'];

  // Loop through the fields for this view.
  $inline = FALSE;
  $vars['fields'] = array(); // ensure it's at least an empty array.
  foreach ($view->field as $id => $field) {
    if (empty($field->options['exclude'])) {
      $object = new stdClass();

      $object->content = $view->field[$id]->theme($vars['row']);
      if (isset($view->field[$id]->field_alias) && isset($vars['row']->{$view->field[$id]->field_alias})) {
        $object->raw = $vars['row']->{$view->field[$id]->field_alias};
      }
      else {
        $object->raw = NULL; // make sure it exists to reduce NOTICE
      }

      $object->handler = $view->field[$id];
      $object->class = views_css_safe($id);
      $object->label = check_plain($view->field[$id]->label());
      $vars['fields'][$id] = $object;
    }
  }
}

