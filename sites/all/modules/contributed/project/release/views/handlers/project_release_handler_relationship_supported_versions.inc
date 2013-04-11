<?php
// $Id: project_release_handler_relationship_supported_versions.inc,v 1.1 2009/08/06 00:53:31 dww Exp $

/**
 * Views handler for relationships between releases and supported version info.
 */
class project_release_handler_relationship_supported_versions extends views_handler_relationship {
  function init(&$view, $options) {
    parent::init($view, $options);
    // This relationship only works with {project_release_supported_versions}
    $this->definition['base'] = 'project_release_supported_versions';
  }

  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    $this->ensure_my_table();
    $join = new views_join();
    $join->definition = array(
      'table' => $this->definition['base'],
      'field' => 'nid',
      'left_table' => $this->table_alias,
      'left_field' => $this->field,
      'extra' => array(
        array(
          'field' => 'major',
          'value' => $this->table_alias . '.version_major',
          'numeric' => TRUE,
        ),
        array(
          'field' => 'tid',
          'value' => $this->table_alias . '.version_api_tid',
          'numeric' => TRUE,
        ),
      ),
    );
    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }
    $join->construct();
    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];
    $this->alias = $this->query->add_relationship($alias, $join, $this->definition['base'], $this->relationship);
  }
}

