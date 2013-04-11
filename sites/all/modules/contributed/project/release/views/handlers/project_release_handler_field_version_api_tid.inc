<?php
// $Id: project_release_handler_field_version_api_tid.inc,v 1.1 2009/11/28 21:11:47 dww Exp $

/**
 * Field handler to display the API compatibility term for a release.
 */
class project_release_handler_field_version_api_tid extends views_handler_field {
  var $api_terms = array();

  function option_definition() {
    $options = parent::option_definition();
    $options['link_to_taxonomy'] = array('default' => FALSE);
    return $options;
  }

  /**
   * Provide link to taxonomy option
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_to_taxonomy'] = array(
      '#title' => t('Link this field to its taxonomy term page'),
      '#description' => t('This will override any other link you have set.'),
      '#type' => 'checkbox',
      '#default_value' => !empty($this->options['link_to_taxonomy']),
    );
  }

  /**
   * Helper function to render a specific term. Handles the term link.
   */
  function render_term($tid, $values) {
    if (empty($this->api_terms)) {
      $tree = project_release_get_api_taxonomy();
      foreach ($tree as $term) {
        $this->api_terms[$term->tid] = $term;
      }
    }
    $current_term = $this->api_terms[$tid];
    if (!empty($this->options['link_to_taxonomy'])) {
      $this->options['alter']['make_link'] = TRUE;
      $this->options['alter']['path'] = taxonomy_term_path($current_term);
    }
    return check_plain($current_term->name);
  }

  function render($values) {
    return $this->render_term($values->{$this->field_alias}, $values);
  }

}

