<?php

/**
 * Validate whether an argument is a project node.
 *
 * This supports either numeric arguments (nid) or strings (project uri) and
 * converts either one into the project nid.  This validator also sets the
 * argument's title to the full title of the project node.
 */
class project_plugin_argument_validate_project_nid extends views_plugin_argument_validate {
  function validate_argument($argument) {
    if (is_numeric($argument)) {
      $result = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n WHERE n.nid = %d AND n.type = '%s'"), $argument, 'project_project'));
    }
    else {
      $result = db_fetch_object(db_query(db_rewrite_sql("SELECT n.nid, n.title FROM {node} n INNER JOIN {project_projects} p ON n.nid = p.nid WHERE p.uri ='%s'"), $argument));
    }
    if (!$result) {
      return FALSE;
    }
    $this->argument->argument = $result->nid;
    $this->argument->validated_title = check_plain($result->title);
    return TRUE;
  }
}

