<?php

/**
 * Filter by project type vocabulary.  This filter provides two checkboxes for
 * including top level project type terms and/or child project type terms.
 */
class project_handler_filter_project_type_vid extends views_handler_filter_in_operator {
  var $include_form_type = 'checkboxes';

  function init(&$view, $options) {
    parent::init($view, $options);
    if (!isset($options['include'])) {
      $options['include'] = array();
    }
    $this->include = $options['include'];
  }

  function construct() {
    parent::construct();
    $this->include_title = t('Include terms from');
    $this->include_options = NULL;
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['include'] = array('default' =>array());
    return $options;
  }

  function get_value_options() {
    if (isset($this->value_options)) {
      return;
    }

    $this->value_options = array();
    $vid = _project_get_vid();
    if (!empty($vid)) {
      $vocabulary = taxonomy_vocabulary_load($vid);
      $this->value_options[$vid] = $vocabulary->name;
    }
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $this->show_include_form($form, $form_state);
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);
    $this->include_submit($form, $form_state);
  }

  /**
   * Shortcut to display the operator form.
   */
  function show_include_form(&$form, &$form_state) {
    $this->include_form($form, $form_state);
  }

  /**
   * Provide only inclusive matching.
   */
  function operator_options() {
    return array(
      'in' => t('Is one of'),
    );
  }

  /**
   * Provide only inclusive matching.
   */
  function include_options() {
    return array(
      'top_level' => t('Top level'),
      'children' => t('Child'),
    );
  }

  function include_form(&$form, &$form_state) {
    $options = $this->include_options();

    $form['include'] = array(
      '#type' => $this->include_form_type,
      '#title' => $this->include_title,
      '#options' => $options,
      '#default_value' => (array) $this->include,
      // These are only valid for 'select' type, but do no harm to checkboxes.
      '#multiple' => TRUE,
      '#size' => count($this->include_options) > 8 ? 8 : $this->include_options,
    );
  }

  function include_submit($form, &$form_state) {
    $form_state['values']['options']['include'] = array_filter($form_state['values']['options']['include']);
  }

  function admin_summary() {
    $summary = parent::admin_summary();
    $this->include_options = $this->include_options();
    if (count($this->include) > 0) {
      $summary_options = array();
      foreach($this->include as $key => $value) {
        $summary_options[] = $this->include_options[$key];
      }
      $summary .= t('(including @summary_options terms)', array('@summary_options' => implode(', ', $summary_options)));
    }
    return $summary;
  }

  /**
   * Override $this->query() so we can limit to top level and/or child terms.
   */
  function query() {
    // If neither top level nor child terms should be displayed, then don't modify the query.
    if (empty($this->include)) {
      return;
    }

    // Allow the parent method to set the query.
    parent::query();

    // If both top level and children terms should be displayed, then we don't need
    // to limit the query any further.
    if (isset($this->include['top_level']) && isset($this->include['children'])) {
      return;
    }

    if (isset($this->include['top_level'])) {
      // Include only top level terms by adding node_hierarchy.parent = 0 to WHERE clause.
      $operator = '=';
    }
    else {
      // Include only children terms by adding node_hierarchy.parent <> 0 to WHERE clause.
      $operator = '<>';
    }
    $term_hierarchy_alias = $this->query->add_table('term_hierarchy');
    $this->query->add_where($this->options['group'], "$term_hierarchy_alias.parent $operator %d", 0);
  }
}
