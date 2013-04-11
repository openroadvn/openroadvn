<?php

/**
 * Provide a filter that allows a user to filter on the project type terms associated
 * with a project.  The terms that this filter makes available depend on an argument
 * which must be a top level term.  Only child terms under this top level term will
 * be included in the filter.
 */
class project_handler_filter_term_node_project_type_tid extends views_handler_filter_term_node_tid {
  function option_definition() {
    $options = parent::option_definition();

    // Make the default exposed filter widget type be a select box.
    $options['type'] = array('default' => 'select');

    $options['expose'] = array(
      'default' => array(
        'operator' => 'or',
        'identifier' => t('type'),
        'label' => t('Project type'),
        'optional' => TRUE,
        'single' => TRUE,
        'remember' => FALSE,
        'reduce' => TRUE,
      ),
    );


    // Make this filter be exposed by default.
    $options['exposed'] = array('default' => TRUE);

    // Set the default vid to be the project type vid.
    $vid = _project_get_vid();
    if (!empty($vid)) {
      $options['vid'] = array('default' => $vid);
    }
    else {
      $options['vid'] = array('default' => 0);
    }

    // Allow user to specify which argument in the view this filter
    // should interact with.
    $options['associated_argument'] = array('default' => '');

    // Allow user to specify that this filter should only be added
    // if it has 1 or more options (not including All).
    $options['remove_if_no_options'] = array('default' => 1);
    return $options;
  }

  /**
   * Provide the basic form which calls through to subforms.
   * Since we want this filter to always be exposed, we have
   * to override this function so we can prevent the expose/hide
   * button from being built.
   */
  function options_form(&$form, &$form_state) {
    $form['op_val_start'] = array('#value' => '<div class="clear-block">');
    $this->show_operator_form($form, $form_state);
    $this->show_value_form($form, $form_state);
    $form['op_val_end'] = array('#value' => '</div>');
    $this->show_expose_form($form, $form_state);
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    // Remove the 'Show hierarchy in dropdown' option.
    unset($form['hierarchy']);

    // Require the widget to be a select box.
    $form['type']['#options'] = array('select' => t('Dropdown'));

    // Remove 'markup_end' form element so we can first add in some
    // additional options.  We add it back in at the end of this function.
    unset($form['markup_end']);

    // Option to select which argument to process.
    $arguments = $this->view->display_handler->get_option('arguments');
    $argument_options = array();
    $i = 0;
    foreach ($arguments as $key => $value) {
      //$argument_options[$key] = t('Argument !num (@name)', array('!num' => $i, '@name' => $key)); // @TODO:  Fix this
      $argument_options[$i] = t('Argument !num (@name)', array('!num' => $i, '@name' => $key));
      $i++;
    }
    $form['associated_argument'] = array(
      '#type' => 'select',
      '#title' => t('Associated argument'),
      '#description' => t('Select the argument this filter should be associated with. If you later change your argument(s) or their order, you must come back to these options and confirm that the correct argument is still selected.'),
      '#options' => $argument_options,
      '#default_value' => $this->options['associated_argument'],
    );

    // Allow this filter to be un-exposed if it contains no options.
    $form['remove_if_no_options'] = array(
      '#type' => 'checkbox',
      '#title' => t('Remove filter if empty'),
      '#description' => t('If checked, this filter will be removed from the view if the argument this filter is associated with is a term that has no child terms.'),
      '#default_value' => !empty($this->options['remove_if_no_options']),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('radio:options[type]' => array('select')),
    );

    $form['markup_end'] = array(
      '#value' => '</div>',
    );
  }

  function value_form(&$form, &$form_state) {
    parent::value_form($form, $form_state);
    $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
    if (!$vocabulary) {
      $form['markup'] = array(
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#value' => t('An invalid vocabulary is selected. Please change it in the options.'),
      );
      return;
    }

    // At this point, we know that the argument is valid, because it's already
    // been run through the validation handler.  However, the first run through
    // the validation handler is for the menu system to see if views wants to
    // handle this partcular path, and after views tells the menu system that it
    // should handle this path, the $view object is destroyed.  So we can't store
    // the processed argument (eg. the tid) in the $view object from within the
    // argument handler, because it won't stay there.
    $associated_argument = $this->options['associated_argument'];
    $argument = isset($this->view->args[$associated_argument]) ? $this->view->args[$associated_argument] : NULL;
    if (isset($argument)) {
      // Build the term object of the term used as the argument.  We don't
      // actually need to join on the term_hierarchy table here, but we'll
      // store the term object we get from these queries in the $view
      // object so that when the argument validator function runs again, it can
      // just use the stored objects instead of re-running it's own queries
      // again.
      if (is_numeric($argument)) {
        $argument_term = db_fetch_object(db_query(db_rewrite_sql("SELECT td.tid, td.vid, td.name, td.description, td.weight, th.parent FROM {term_data} td LEFT JOIN {term_hierarchy} th ON th.tid = td.tid WHERE td.tid = %d", 'td', 'tid'), $argument));
      }
      else {
        $argument_term = db_fetch_object(db_query(db_rewrite_sql("SELECT td.tid, td.vid, td.name, td.description, td.weight, th.parent FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid LEFT JOIN {term_hierarchy} th ON th.tid = td.tid WHERE (td.name = '%s' OR ts.name = '%s') AND td.vid = %d", 'td', 'tid'), $argument, $argument, $this->options['vid']));
      }

      if ($argument_term) {
        // Store the term object in the $view object for later use by other
        // handlers.
        $this->view->project->argument_term_objects[$associated_argument] = $argument_term;
      }
      else {
        // The only way execution would get to this point is if the argument
        // this filter handler is inspecting is not the argument that has
        // been validated by the project type validator function.
        // This function is expected to add something $form['value'], so we just
        // make it a field of type value.
        $this->options['exposed'] = FALSE;
        $form['value'] = array(
          '#type' => 'value',
        );
        $field = $this->field;
        unset($this->view->filter->$field);
        return;
      }

      // Currently this handler only supports using a select box for the project type terms.
      if ($this->options['type'] == 'select') {
        $options = array();
        $result = db_query(db_rewrite_sql("SELECT * FROM {term_data} td INNER JOIN {term_hierarchy} th ON th.tid = td.tid WHERE th.parent = %d ORDER BY weight, name", 'td', 'tid'), $argument_term->tid);
        while ($term = db_fetch_object($result)) {
          $options[$term->tid] = $term->name;
        }

        // If there are no $options, that means that the term used in the argument
        // has no child terms.  If that's the case, we may want remove this
        // filter from the current view.
        if (count($options) < 1 && !empty($this->options['remove_if_no_options'])) {
          $this->options['exposed'] = FALSE;
          $form['value'] = array(
            '#type' => 'value',
          );
          $field = $this->field;
          unset($this->view->filter->$field);
          return;
        }

        // Since we're overriding, allow the list of options to be reduced.
        if (!empty($form_state['exposed']) && !empty($this->options['expose']['reduce'])) {
          $options = $this->reduce_value_options($options);
        }

        $form['value'] = array(
          '#type' => 'select',
          '#title' => t('Select terms from vocabulary @voc', array('@voc' => $vocabulary->name)),
          '#multiple' => TRUE,
          '#options' => $options,
          '#size' => min(9, count($options)),
          '#default_value' => !empty($this->value) ? $this->value : array(),
        );
      }

      if (empty($form_state['exposed'])) {
        // Retain the helper option
        $this->helper->options_form($form, $form_state);
      }
      return;
    }

    // Execution will only get to this point if the argument wasn't found.  This
    // can happen when the filter's settings are being edited in the Views UI,
    // and can also happen if the user selected a value for $argument_position
    // that is invalid.  In either case we have to add something to $form['value'].
    // In addition, if we're in a view that's being executed, we need to
    // un-expose this particular filter and also remove it from the view.
    if (!empty($this->view->args)) {
      $this->options['exposed'] = FALSE;
      $field = $this->field;
      unset($this->view->filter->$field);
    }
    $form['value'] = array(
      '#type' => 'value',
    );
  }
}
