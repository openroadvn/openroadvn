<?php

/**
 * Allow taxonomy term ID(s) as argument and produces a summary which
 * prevents listing top level terms.
 * @TODO:  more phpdoc.
 *
 */
class project_handler_argument_term_node_tid_depth_project extends views_handler_argument_term_node_tid {
  function summary_query() {
    $field = $this->table . '.' . $this->field;
    $join = $this->get_join();

    if (!empty($this->options['require_value'])) {
      $join->type = 'INNER';
    }

    if (empty($this->options['add_table']) || empty($this->view->many_to_one_tables[$field])) {
      $this->table_alias = $this->query->ensure_table($this->table, $this->relationship, $join);
    }
    else {
      $this->table_alias = $this->helper->summary_join();
    }

    // Add the field.
    $this->base_alias = $this->query->add_field($this->table_alias, $this->real_field);

    if (isset($this->argument) && is_numeric($this->argument)) {
      // Add {term_hierarchy} table
      $th_alias = $this->query->add_table('term_hierarchy');
  
      // Limit returned results to terms whose parent is the argument.
      $this->query->add_where(0, "$th_alias.parent = %d", $this->argument);
    }

    $this->summary_name_field();

    return $this->summary_basics();
  }

  //function query() {
  //  $this->query->clear_fields();
  //}
}
