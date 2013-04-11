<?php
// $Id: project_issue_handler_filter_issue_component.inc,v 1.2 2010/01/17 00:22:44 dww Exp $


/**
 * Filter issues based on their component.
 *
 * This filter only returns values to select if it's exposed and the view has
 * a project argument (either nid or uri), and that argument is first.
 */
class project_issue_handler_filter_issue_component extends views_handler_filter_in_operator {
  var $is_exposed = FALSE;

  function value_form(&$form, &$form_state) {
    if (!empty($form_state['exposed'])) {
      // Set a flag so get_value_options() knows to return real values.
      $this->is_exposed = TRUE;
      // Due to some unknown bug in views, it appears as though this filter
      // always acts as if it's a "force single" filter, even if it's not.
      // Therefore, it's (sometimes?) setting its own value to to 'All', when
      // GET is empty, which gives an invalid choice when you load the form.
      $this->value = NULL;
    }
    parent::value_form($form, $form_state);
    if (empty($form_state['exposed'])) {
      $form['warning'] = array(
        '#type' => 'markup',
        '#value' => t('You can only select options if you expose this filter and have a project argument'),
      );
    }
  }

  function get_value_options() {
    static $components = array();
    // We only want to return real options if we're building an exposed filter
    // form and we have a project argument (either nid or uri).  So, we check
    // the flag set in value_form() to see if this is an exposed filter.
    if ($this->is_exposed) {
      if (empty($components)) {
        $arg = reset($this->view->args);
        if (is_numeric($arg)) {
          $project = db_fetch_object(db_query("SELECT pip.components FROM {project_issue_projects} pip WHERE pip.nid = %d", $arg));
        }
        else {
          $project = db_fetch_object(db_query("SELECT pip.components FROM {project_issue_projects} pip INNER JOIN {project_projects} pp ON pip.nid = pp.nid WHERE pp.uri = '%s'", $arg));
        }
        if (!empty($project->components)) {
          foreach (unserialize($project->components) as $component) {
            $components[$component] = check_plain($component);
          }
        }
      }
      $this->value_options = $components;
    }
    else {
      $this->value_options = array();
    }
  }

}

