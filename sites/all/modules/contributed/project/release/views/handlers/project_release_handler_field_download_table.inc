<?php
// $Id: project_release_handler_field_download_table.inc,v 1.1 2009/01/13 17:47:09 dww Exp $


/**
 * Views field handler for the table to download releases.
 */
class project_release_handler_field_download_table extends views_handler_field {
  /**
   * Constructor; calls to base object constructor.
   */
  function construct() {
    parent::construct();

    $this->additional_fields = array();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['format'] = array('table' => 'node_revisions', 'field' => 'format');
    $this->additional_fields['uid'] = 'uid';
    $this->additional_fields['type'] = 'type';
  }

  function init(&$view, $options) {
    parent::init($view, $options);

    $this->options += array(
      'release_type' => $this->radios_get_default($this->get_type_options('release')),
      'table_type' => $this->radios_get_default($this->get_type_options('table')),
      'first_column_title' => '',
    );
  }

  /**
   * Helper function that returns an array of possible types.
   *
   * @param $type
   *   Indicates what type of options should be returned.  Possible
   *   values are 'release' to get release types or 'table' to get
   *   release table types.
   * @return
   *   An array of possible types.
   */
  function get_type_options($type){
    $types = array(
      'release' => array(
        'default' => 'official',
        'options' => array(
          'official' => array(
            'name' => t('Official'),
            'default_title' => t('Official releases'),
          ),
          'snapshot' => array(
            'name' => t('Snapshot'),
            'default_title' => t('Snapshot releases'),
          ),
          'all' => array(
            'name' => t('All'),
            'default_title' => t('Version'),
          ),
        ),
      ),
      'table' => array(
        'default' => 'supported',
        'options' => array(
          'recommended' => array(
            'name' => t('Recommended'),
          ),
          'supported' => array(
            'name' => t('Supported'),
          ),
          'all' => array(
            'name' => t('All'),
          ),
        ),
      ),
    );
    return (isset($types[$type])) ? $types[$type] : array();
  }

  /**
   * Get the default option.
   *
   * @param $types
   *   An array of types returned by $this::get_type_options().
   * @return
   *   A string indicating the key of $types that should be the default option.
   */
  function radios_get_default($types) {
    return isset($types['default']) ? $types['default'] : '';
  }

  /**
   * Get all possible options.
   *
   * @param $types
   *   An array of types returned by $this::get_type_options().
   * @return
   *   An array of possible options.
   */
  function radios_get_options($types) {
    $options = array();
    foreach ($types['options'] as $key => $info) {
      $options[$key] = $info['name'];
    }
    return $options;
  }

  /**
   * Default options form that provides the label widget that all fields
   * should have.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['table_type'] = array(
      '#type' => 'radios',
      '#prefix' => '<div class="clear-block">',
      '#suffix' => '</div>',
      '#title'=> t('Table type'),
      '#options' => $this->radios_get_options($this->get_type_options('table')),
      '#default_value'=> $this->options['table_type'],
    );
    $release_types = $this->get_type_options('release');
    $form['release_type'] = array(
      '#type' => 'radios',
      '#prefix' => '<div class="views-left-30">',
      '#suffix' => '</div>',
      '#title'=> t('Release type'),
      '#options' => $this->radios_get_options($release_types),
      '#default_value' => $this->options['release_type'],
    );
    $form['first_column_title']= array(
      '#type' => 'textfield',
      '#prefix' => '<div class="views-left-50">',
      '#suffix' => '</div>',
      '#title' => t('Title of first column of table'),
      '#max_length' => 20,
      '#default_value' => empty($this->options['first_column_title']) ? $release_types['options'][$this->options['release_type']]['default_title'] : $this->options['first_column_title'],
   );
  }

  /**
   * Provide extra data to the administration form.
   */
  function admin_summary() {
    $output = '';
    $release_types = $this->radios_get_options($this->get_type_options('release'));
    $table_types = $this->radios_get_options($this->get_type_options('table'));
    $output .= t('@release_type releases, Table type: @table_type', array('@release_type' => $release_types[$this->options['release_type']], '@table_type' => $table_types[$this->options['table_type']]));
    return $output;
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->uid = $values->{$this->aliases['uid']};
    $node->type = $values->{$this->aliases['type']};
    $node->format = $values->{$this->aliases['format']};

    // Load a few release related fields for the project itself.
    project_release_project_nodeapi_load($node);
    $node->status = 1; // unpublished nodes ignore access control
    if (empty($node->releases)) {
      return;
    }
    $output = '';
    $output .= project_release_table($node, $this->options['table_type'], $this->options['release_type'], $this->options['first_column_title']);

    return $output;
  }
}

