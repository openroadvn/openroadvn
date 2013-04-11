<?php

/**
 * Handler for a filter that allows for different sorting methods to be
 * used for project node listings.
 */
class project_handler_filter_project_sort_method extends views_handler_filter {
  /**
   * Since we're not using the operator option, we have to override parent::init().
   */
  function init(&$view, $options) {
    views_handler::init($view, $options);
    $this->value = $this->options['value'];
  }

  function has_extra_options() { return TRUE; }

  function get_value_options() { /* don't overwrite the value options */ }

  function option_definition() {
    $options = parent::option_definition();

    // Make this filter be exposed by default.
    $options['exposed'] = array('default' => TRUE);

    // Provide a select box for selecting the sort methods to expose to the user.
    $options['sort_methods'] = array('default' => array());

    // We don't need an operator for this filter.
    unset($options['operator']);
    return $options;
  }

  /**
   * Provide the basic form which calls through to subforms.
   *
   * We override the parent function here because we only want
   * to show the exposed filter settings form.
   */
  function options_form(&$form, &$form_state) {
    $this->show_expose_form($form, $form_state);
  }

  /**
   * Provide a form for setting the sorting method.
   */
  function value_form(&$form, &$form_state) {
    $options = array();

    // Get an array with all sorting methods available for this display.
    $sorts = $this->view->display_handler->get_option('sorts');

    // Get an array with the sorting methods the admin has enabled to
    // be options for this filter.
    $enabled_sort_methods = array();
    $enabled_sort_weights = array();
    $sort_methods = $this->options['sort_methods'];
    foreach ($sort_methods as $id => $value) {
      if ($value['enabled']) {
        $enabled_sort_weights[$id] = $value['weight'];
      }
    }
    // Sort the array by weight.
    asort($enabled_sort_weights);
    foreach ($enabled_sort_weights as $id => $value) {
      $enabled_sort_methods[$id] = $sort_methods[$id]['display_name'];
    }

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Select the sorting method'),
      '#multiple' => FALSE,
      '#options' => $enabled_sort_methods,
    );
  }

  /**
   * Allow the user to select which search criteria
   * the user should be able to pick from.
   */
  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    $sort_method_options = $this->options['sort_methods'];
    $sorts = $this->view->display_handler->get_option('sorts');
    $form['sort_methods'] = array(
      '#prefix' => '<div class="form-item description">' . t('Below you will find a list of all available sort criteria. Enable those criteria you wish to be available for the user to select. You can change the order these sort criteria will be displayed in the dropdown box by changing the weight. Changing the weight on this form will not affect the relative order of the sorts. The value used in the "Display name" column is the name that will be displayed to the user.'),
      '#theme' => 'project_views_project_sort_method_options_form',
      '#suffix' => '</div>',
      '#tree' => TRUE,
    );

    $weights = array();
    foreach ($sorts as $id=> $value) {
      $handler = views_get_handler($sorts[$id]['table'], $sorts[$id]['field'], 'sort');
      $weights[$id] = isset($sort_method_options[$id]['weight']) ? $sort_method_options[$id]['weight'] : 0;
      $rows[$id]['enabled'] = array(
        '#type' => 'checkbox',
        '#default_value' => !empty($sort_method_options[$id]['enabled']) ? TRUE : FALSE,
      );

      $rows[$id]['id'] = array(
        '#type' => 'markup',
        '#value' => $id,
      );

      $rows[$id]['name'] = array(
        '#type' => 'markup',
        '#value' => $handler->ui_name(),
      );

      $rows[$id]['weight'] = array(
        '#type' => 'weight',
        '#delta' => 25,
        '#default_value' => $weights[$id],
      );

      $rows[$id]['display_name'] = array(
        '#type' => 'textfield',
        '#default_value' => isset($sort_method_options[$id]['display_name']) ? $sort_method_options[$id]['display_name'] : $sort_method_options[$id]['name'],
        '#size' => 25,
      );
    }

    // We have to display the rows in order by weight.
    asort($weights);
    foreach ($weights as $id => $row) {
      $form['sort_methods'][$id] = $rows[$id];
    }

    // Set up this form to use tabledrag.
    $form_state['js settings']['tableDrag']['exposed-sort-weight']['weight'][0] = array(
      'target' => 'weight',
      'source' => NULL,
      'relationship' => 'sibling',
      'action' => 'order',
      'hidden' => TRUE,
      'limit' => 0,
    );
  }

  /**
   * Display the filter on the administrative summary
   */
  function admin_summary() {
    if (!empty($this->options['exposed'])) {
      return t('exposed');
    }
  }

  /**
   * When the user selects a sorting method, remove all other sort criteria
   * that the user also chould have selected from the view.
   */
  function exposed_submit(&$form, &$form_state) {
    // Determine which sort criteria was selected.
    $filter_identifier = $this->options['expose']['identifier'];
    $sort_by = $form_state['values'][$filter_identifier];

    // Remove all other sort criteria that the user could have selected
    // but didn't from the view.
    $exposed_sort_methods = $form[$filter_identifier]['#options'];
    foreach ($exposed_sort_methods as $id => $value) {
      if ($id != $sort_by) {
        unset($this->view->sort[$id]);
      }
    }
  }

  /**
   * Since this handler isn't a "true" filter, we don't want to directly
   * modify the query here.
   */
  function query() { }
}
