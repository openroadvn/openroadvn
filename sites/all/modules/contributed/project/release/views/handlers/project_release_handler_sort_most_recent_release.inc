<?php
// $Id: project_release_handler_sort_most_recent_release.inc,v 1.1 2009/01/13 17:47:09 dww Exp $


/**
 * Sort handler for the project_release_most_recent_release field.
 */
class project_release_handler_sort_most_recent_release extends views_handler_sort_date {
  /**
   * Called to add the sort to a query.
   */
  function query() {
    $this->ensure_my_table();
    $table_alias = !empty($this->relationship) ? $this->relationship : $this->table_alias;
    $field =  $table_alias. '.' . $this->real_field;
    $max_field_alias = !empty($this->relationship) ? $this->relationship . '_max' : $this->table_alias . '_' . $this->real_field . '_max';
    switch ($this->options['granularity']) {
      case 'second':
      default:
        $this->query->add_orderby(NULL, "MAX($field)", $this->options['order'], $max_field_alias);
        $position = array_search($max_field_alias, $this->query->groupby);
        if (isset($position)) {
          unset($this->query->groupby[$position]);
        }
        return;
      case 'minute':
        $formula = views_date_sql_format('YmdHi', "$this->table_alias.$this->real_field");
        break;
      case 'hour':
        $formula = views_date_sql_format('YmdH', "$this->table_alias.$this->real_field");
        break;
      case 'day':
        $formula = views_date_sql_format('Ymd', "$this->table_alias.$this->real_field");
        break;
      case 'month':
        $formula = views_date_sql_format('Ym', "$this->table_alias.$this->real_field");
        break;
      case 'year':
        $formula = views_date_sql_format('Y', "$this->table_alias.$this->real_field");
        break;
    }

    // Add the field.
    $max_field_alias = !empty($this->relationship) ? $this->relationship . '_max' : $this->table_alias . '_' . $this->real_field . '_' . $this->options['granularity'] . '_max';
    $this->query->add_orderby(NULL, "MAX($formula)", $this->options['order'], $max_field_alias);
    $position = array_search($max_field_alias, $this->query->groupby);
    if (isset($position)) {
      unset($this->query->groupby[$position]);
    }
  }
}

