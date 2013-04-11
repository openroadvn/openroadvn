<?php
// $Id: project_issue_handler_field_issue_queue_link.inc,v 1.3 2009/08/01 03:43:47 dww Exp $


/**
 * Field handler to link to the issue queue of a given project.
 */
class project_issue_handler_field_issue_queue_link extends views_handler_field {
  /**
   * Constructor to provide additional field to add.
   */
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['uri'] = array('table' => 'project_projects', 'field' => 'uri');
  }

  function option_definition() {
    $options = parent::option_definition();
    $options['link_type'] = array('default' => 'issue', 'translatable' => FALSE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['link_type'] = array(
      '#type' => 'radios',
      '#title' => t('Type of link to display'),
      '#options' => $this->get_link_type_options(),
      '#default_value' => $this->options['link_type'],
    );
  }

  function get_link_type_options() {
    return array(
      'issues' => t('Link to issue queue'),
      'search' => t('Link to advanced search'),
    );
  }

  function render($values) {
    $basepath = 'project/issues/';
    if ($this->options['link_type'] == 'search') {
      $basepath .= 'search/';
    }
    return l($values->{$this->field_alias}, $basepath . $values->{$this->aliases['uri']});
  }
}

