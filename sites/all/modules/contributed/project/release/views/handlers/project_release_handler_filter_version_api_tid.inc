<?php
// $Id: project_release_handler_filter_version_api_tid.inc,v 1.1 2009/11/28 21:11:47 dww Exp $

/**
 * Filter release nodes based on their API compatibility term.
 */
class project_release_handler_filter_version_api_tid extends views_handler_filter_in_operator {
  function has_extra_options() { return TRUE; }

  function option_definition() {
    $options = parent::option_definition();
    $options['type'] = array('default' => 'select');
    return $options;
  }

  /**
   * Add an extra option to control if the filter should use all active terms.
   */
  function extra_options_form(&$form, &$form_state) {
    $form['type'] = array(
      '#type' => 'radios',
      '#title' => t('Selection type'),
      '#options' => array(
        'select' => t('Select terms'),
        'all' => t('All active terms'),
      ),
      '#default_value' => $this->options['type'],
    );
  }

  /**
   * Don't let admins expose the filter if it's configured to use all terms.
   */
  function can_expose() {
    return $this->options['type'] != 'all';
  }

  /**
   * Build the form for selecting what value to use for the filter.
   *
   * If were configured to select terms, just use parent::value_form(), which
   * will invoke get_value_options() and provide a wall of checkboxes, etc.
   * However, if we're configured to use all active terms, just print some
   * help text explaining why the user can't select anything here nor expose
   * the filter.
   */
  function value_form(&$form, &$form_state) {
    if ($this->options['type'] == 'select') {
      parent::value_form($form, $form_state);
    }
    elseif (empty($form_state['exposed'])) {
      $form['value'] = array(
        '#value' => t("This filter is configured to use all active API compatibility terms. If you want to expose this filter or restrict it to a subset of active terms, click the gear icon and change the 'Selection type' setting."),
      );
    }
  }

  /**
   * Only provide a list of active terms if we're not configured to use all.
   */
  function get_value_options() {
    if ($this->options['type'] == 'select') {
      $this->value_options = project_release_compatibility_list();
    }
  }

  /**
   * Handle the case where the filter automatically uses all active terms.
   */
  function query() {
    if ($this->options['type'] == 'all') {
      $this->value = drupal_map_assoc(array_keys(project_release_compatibility_list()));
    }
    parent::query();
  }

}

