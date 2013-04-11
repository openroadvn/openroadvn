<?php
// $Id: project_release_handler_filter_project_release_api_version.inc,v 1.3 2009/02/04 01:18:19 dww Exp $


/**
 * Views filter handler for the release's API version compatibility term.
 *
 * In order to be able to limit the API compatability terms that are shown in
 * the filter to terms the administrator has marked as active on the
 * Project release settings page, we need to have a separate handler that
 * reimplements a few functions from the views_handler_filter_term_node_tid
 * class.
 */
class project_release_handler_filter_project_release_api_version extends views_handler_filter_term_node_tid {
  function option_definition() {
    $options = parent::option_definition();

    // We need to make sure that reduce duplicates is TRUE or otherwise
    // the query will sometimes return unexpected results.
    $options['reduce_duplicates'] = array('default' => TRUE);

    // This filter does not support an autocomplete textfield, so make
    // the default exposed filter widget type be a select box.
    $options['type'] = array('default' => 'select');

    // Make sure that the Project release API compatability vocabulary is
    // the default vocabulary.
    $options['vid'] = array('default' => _project_release_get_api_vid());
    return $options;
  }

  /**
   * Reduce duplicates must be TRUE or we will get unexpected results in some
   * cases, so don't let it be disabled.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    if (isset($form['reduce_duplicates'])) {
      unset($form['reduce_duplicates']);
    }
  }

  function extra_options_form(&$form, &$form_state) {
    parent::extra_options_form($form, $form_state);

    // Require the widget to be a select box.
    $form['type']['#options'] = array('select' => t('Dropdown'));
  }

  /**
   * This code is modified from views_handler_filter_term_node_tid::value_form()
   * to only present the API compatability terms which are marked as
   * active on the Project release settings page.
   */
  function value_form(&$form, &$form_state) {
    $vocabulary = taxonomy_vocabulary_load($this->options['vid']);
    if (!$vocabulary) {
      $form['markup'] = array(
        '#prefix' => '<div class="form-item">',
        '#suffix' => '</div>',
        '#value' => t('An invalid vocabulary is selected. Please change it in the options.'),
      );
      return;
    }

    if (!empty($this->options['hierarchy'])) {
      $tree = taxonomy_get_tree($vocabulary->vid);
      $options = array();

      if ($tree) {
        foreach ($tree as $term) {
          $choice = new stdClass();
          $choice->option = array($term->tid => str_repeat('-', $term->depth) . $term->name);
          $options[] = $choice;
        }
      }
    }
    else {
      $options = array();
      $terms = project_release_compatibility_list();
      foreach ($terms as $tid => $name) {
        $options[$tid] = $name;
      }
    }

    // Since we're overriding, allow the list of options to be reduced.
    if (!empty($form_state['exposed']) && !empty($this->options['expose']['reduce'])) {
      $options = $this->reduce_value_options($options);
    }

    $form['value'] = array(
      '#type' => 'select',
      '#title' => t('Select active terms from vocabulary @voc', array('@voc' => $vocabulary->name)),
      '#multiple' => TRUE,
      '#options' => $options,
      '#size' => min(9, count($options)),
    );

    // Only add a description to the select box if we're building the filter
    // options page, and not the actual exposed filter that the user sees.
    if (empty($form_state['exposed'])) {
      $form['value']['#description'] = t('Only terms marked as active on the <a href="@project_release_settings_link">Project release settings</a> page are displayed above.', array('@project_release_settings_link' => url('admin/project/project-release-settings')));
    }

    if (empty($form_state['exposed'])) {
      // Retain the helper option
      $this->helper->options_form($form, $form_state);
    }
  }
}

