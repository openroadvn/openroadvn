<?php
// $Id: project_issue_table_plugin_style.inc,v 1.2 2009/02/18 18:23:43 dww Exp $

/**
 * @file
 * Class for the project_issue_table Views style plugin.
 *
 * We mostly re-use all of the code of the core Views table style, but
 * by having our own style plugin, we can inject our own custom classes into
 * the table HTML to style the rows by issue status and priority.
 *
 * This class also does some hacks for setting the right breadcrumbs for the
 * default issue views.
 */

class project_issue_table_plugin_style extends views_plugin_style_table { 
  function render() {
    // Set the project breadcrumb.
    if ($this->view->name == 'project_issue_project' || $this->view->name == 'project_issue_search_project') {
      project_project_set_breadcrumb(node_load($this->view->argument['pid']->argument), TRUE);
    }

    return parent::render();
  }
}
