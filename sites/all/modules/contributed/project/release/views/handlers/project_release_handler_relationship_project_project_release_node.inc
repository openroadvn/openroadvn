<?php
// $Id: project_release_handler_relationship_project_project_release_node.inc,v 1.1 2009/01/13 17:47:09 dww Exp $


/**
 * Specialized handler for relationships between projects and release nodes.
 */
class project_release_handler_relationship_project_project_release_node extends views_handler_relationship {
  /**
   * Called to implement a relationship in a query.
   */
  function query() {
    // Figure out what base table this relationship brings to the party.
    $join = new views_join();
    $join->definition = array(
      'table' => 'project_release_nodes',
      'field' => 'pid',
      'left_table' => 'node',
      'left_field' => 'nid',
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    $join->construct();

    $this->first_join = $this->query->add_table('project_release_nodes', $this->relationship, $join);

    // Then, create a relationship on that table:
    $join = new views_join();
    $join->definition = array(
      'left_table' => $this->first_join,
      'left_field' => 'nid',
      'table' => 'node',
      'field' => 'nid',
    );

    if (!empty($this->options['required'])) {
      $join->definition['type'] = 'INNER';
    }

    $join->construct();

    $alias = $join->definition['table'] . '_' . $join->definition['left_table'];

    $this->alias = $this->query->add_relationship($alias, $join, 'node', $this->relationship);
  }
}

