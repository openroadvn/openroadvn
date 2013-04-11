<?php

/**
 * Plugin which performs a node_view on the resulting object.
 *
 * Most of the code on this object is in the theme function.
 *
 */
class project_plugin_row_project_node_view extends views_plugin_row {
  function option_definition() {
    $options = parent::option_definition();

    $options['project_teaser'] = array('default' => TRUE);
    if (module_exists('taxonomy')) {
      $options['project_term_links'] = array('default' => TRUE);
    }
    if (module_exists('project_release')) {
      $options['project_release_download_table'] = array('default' => TRUE);
      $options['project_release_download_link'] = array('default' => TRUE);
    }
    if (module_exists('project_issue')) {
      $options['project_issue_issues_link'] = array('default' => TRUE);
    }
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['project_teaser'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display only teaser instead of the full node'),
      '#default_value' => $this->options['project_teaser'],
    );

    if (module_exists('taxonomy')) {
      $form['project_term_links'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display project type term links'),
        '#default_value' => $this->options['project_term_links'],
      );
    }
    if (module_exists('project_release')) {
      $form['project_release_download_table'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display the release download table'),
        '#default_value' => $this->options['project_release_download_table'],
      );
      $form['project_release_download_link'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display a link to a file download when the version filter is in use'),
        '#default_value' => $this->options['project_release_download_link'],
      );
    }
    if (module_exists('project_issue')) {
      $form['project_issue_issues_link'] = array(
        '#type' => 'checkbox',
        '#title' => t('Display a link to issues for the project'),
        '#default_value' => $this->options['project_issue_issues_link'],
      );
    }
  }
  
  /**
   * Determine what project type is being viewed and store it
   * for use later on in the rendering process.
   *
   * @param $result
   *   The full array of results from the query.
   */
  function pre_render($result) {
    if (!isset($this->view->project->project_type) && isset($this->view->args[0])) {
      $this->view->project->project_type = _project_views_get_project_type($this->view->args[0]);
    }
  }
}

