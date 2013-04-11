<?php
// $Id: project_release_handler_field_most_recent_release.inc,v 1.1 2009/01/13 17:47:09 dww Exp $


/**
 * Field handler for the project_release_most_recent_release field.
 */
class project_release_handler_field_most_recent_release extends views_handler_field_date {
  var $field_alias = 'unknown';

  /**
   * Called to add the field to a query.
   */
  function query() {
    $this->ensure_my_table();
    // Add the field.
    $field = !empty($this->relationship) ? $this->relationship : $this->table_alias . '.' . $this->real_field;
    $field_alias = !empty($this->relationship) ? $this->relationship . '_max' : $this->table_alias . '_' . $this->real_field . '_max';
    $this->field_alias = $this->query->add_field(NULL, "MAX($field)", $field_alias);
    $this->add_additional_fields();

    // Make sure that node.nid is part of the SELECT statement.
    $node_table_alias = $this->query->ensure_table('node', $this->relationship);
    $nid_field_alias = $this->query->add_field($node_table_alias, 'nid');
    $this->query->add_groupby($nid_field_alias);
  }
}

