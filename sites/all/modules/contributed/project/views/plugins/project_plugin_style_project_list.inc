<?php

/**
 * Style plugin to render a list of projects.
 */
class project_plugin_style_project_list extends views_plugin_style {
  /**
   * Store the project type term being browsed for later usage.
   */
  function pre_render($result) {
    if (isset($this->view->args[0])) {
      $this->view->project->project_type = _project_views_get_project_type($this->view->args[0]);
    }
    
    // If the first sort criteria is the changed field (for sorting by date),
    // then we need to add some information to the results set
    // so that they can later be grouped by date in the theming functions.
    // Views doesn't allow us to do this via the UI because views can
    // only handle grouping if fields, not nodes, are being displayed.
    if (is_array($this->view->sort) && isset($this->view->sort['changed']) && $this->view->sort['changed']->position == 0) {
      $field_alias = $this->view->sort['changed']->table_alias . "_" . $this->view->sort['changed']->field;
      $granularity = $this->view->sort['changed']->options['granularity'];
      switch ($granularity) {
        case 'second':
          $date_format_string = 'YmdHis';
          break;
        case 'minute':
          $date_format_string = 'YmdHi';
          break;
        case 'hour':
          $date_format_string = 'YmdH';
          break;
        case 'day':
          $date_format_string = 'Ymd';
          break;
        case 'month':
          $date_format_string = 'Ym';
          break;
        case 'year':
          $date_format_string = 'Y';
          break;
        default:
          $date_format_string = 'Ymd';
      }

      $last_date = '';
      foreach ($this->view->result as $i => $value) {
        $date = format_date($value->$field_alias, 'custom', $date_format_string);
        // If this date is a new date, once the granularity selected has been taken
        // into account, set a special property in the results of the view so that
        // later on in template_preprocess_project_views_view_row_project_node() we
        // can flag a row as having a different changed date than the last row
        // so that the actual date can be displayed as a header from the template.
        // @TODO:  As written, this only works with second granularity.
        if ($date != $last_date) {
          $this->view->project->project_new_date[$i] = TRUE;
        }
        $last_date = $date;
      }
    }
    parent::pre_render($result);
  }
}
