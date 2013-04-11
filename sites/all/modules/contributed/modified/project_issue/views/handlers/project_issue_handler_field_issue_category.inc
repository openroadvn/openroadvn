<?php
// $Id: project_issue_handler_field_issue_category.inc,v 1.2 2009/01/20 18:38:04 dww Exp $


/**
 * Field handler to display the current category of an issue.
 * 
 */
class project_issue_handler_field_issue_category extends views_handler_field {
  function render($values) {
    //TBR add this change to permit see options in db
    $pid = NULL;
    $pid = variable_get('current_group', NULL);
    module_load_include('inc', 'project_issue', 'issue');
    return check_plain(project_issue_category($values->{$this->field_alias},1,$pid));
 
  }
}

