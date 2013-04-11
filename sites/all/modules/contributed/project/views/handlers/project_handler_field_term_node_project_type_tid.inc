<?php

/**
 * Provide a field that contains all of the project type terms for a node.
 * Optionally the terms can be limited to non top-level terms.
 *
 * Based on the class views_handler_field_term_node_tid in taxonomy.views.inc.
 */
class project_handler_field_term_node_project_type_tid extends views_handler_field_term_node_tid {
  function option_definition() {
    $options = parent::option_definition();

    $options['exclude_top_level_terms'] = array('default' => TRUE);

    return $options;
  }

  /**
   * Provide "exclude top level terms" option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['exclude_top_level_terms'] = array(
      '#title' => t('Exclude top level project type terms'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['exclude_top_level_terms']),
    );

    // Override the parent vocabulary selection field so that the project
    // type vocabulary is always selected.
    $options = array();
    $project_type_vid = _project_get_vid();
    if (!empty($project_type_vid)) {
      $vocabulary = taxonomy_vocabulary_load($project_type_vid);
      $options[$vocabulary->vid] = $vocabulary->name;

      unset($form['vids']);
      $form['vids'] = array(
        '#prefix' => '<div><div id="edit-options-vid">',
        '#suffix' => '</div></div>',
        '#type' => 'checkboxes',
        '#title' => t('Project type vocabulary'),
        '#options' => $options,
        '#default_value' => $this->options['vids'],
        '#process' => array('expand_checkboxes', 'views_process_dependency'),
        '#dependency' => array('edit-options-limit' => array(TRUE)),
      );
    }
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->add_additional_fields();
  }

// @TODO:  Finish this handler.
  function pre_render($values) {
    $this->field_alias = $this->aliases['vid'];
    $vids = array();
    foreach ($values as $result) {
      $vids[] = $result->{$this->aliases['vid']};
    }

    if ($vids) {
      $voc = '';
      if (!empty($this->options['limit']) && !empty($this->options['vids'])) {
        $voc = " AND td.vid IN (" . implode(', ', array_keys(array_filter($this->options['vids']))) . ")";
      }

      // Exclude top level terms.
      $top_level_join = '';
      $top_level_where = '';
      if (!empty($this->options['exclude_top_level_terms'])) {
        $top_level_join = " INNER JOIN {term_hierarchy} th ON td.tid = th.tid";
        $top_level_where = "AND th.parent <> 0";
      }

      $result = db_query("SELECT tn.vid AS node_vid, td.* FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid$top_level_join WHERE tn.vid IN (" . implode(', ', $vids) . ")$voc $top_level_where ORDER BY td.weight, td.name");

      while ($term = db_fetch_object($result)) {
        if (empty($this->options['link_to_taxonomy'])) {
          $this->items[$term->node_vid][$term->tid] = check_plain($term->name);
        }
        else {
          $this->items[$term->node_vid][$term->tid] = l($term->name, taxonomy_term_path($term));
        }
      }
    }
  }

  function admin_summary() {
    if (!empty($this->options['exclude_top_level_terms'])) {
      return t('top level terms excluded');
    }
  }
}
