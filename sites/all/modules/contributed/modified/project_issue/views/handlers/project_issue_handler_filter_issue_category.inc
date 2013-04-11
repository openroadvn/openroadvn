<?php
// $Id: project_issue_handler_filter_issue_category.inc,v 1.2 2009/01/20 18:38:04 dww Exp $


/**
 * Filter issues based on their category.
 */
class project_issue_handler_filter_issue_category extends views_handler_filter_in_operator {
  function get_value_options() {
    module_load_include('inc', 'project_issue', 'issue');
    $pid = NULL;
    // All categories, not plural
    //if(sizeof(project_issue_count($pid))>0) //exists issues?
    $pid = variable_get('current_group', NULL);
    //$this->value_options = project_issue_category(FALSE, FALSE,$pid);
    $this->value_options = project_issue_category(FALSE, FALSE,$pid);
  }
}

