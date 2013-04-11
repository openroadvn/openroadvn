<?php
// $Id: project_issue_handler_filter_issue_version.inc,v 1.11 2010/01/17 00:22:44 dww Exp $


/**
 * Filter issues based on their version.
 *
 * This filter only returns values to select if it's exposed and the view has
 * a project argument (either nid or uri), and that argument is first.
 *
 * This is almost a copy of project_issue_handler_filter_issue_component.
 */
class project_issue_handler_filter_issue_version extends views_handler_filter_in_operator {
  var $is_exposed = FALSE;
  var $project_versions = array();
  var $project_version_series = array();

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
    $this->get_value_options();
    if (!empty($this->value_options)) {
      parent::value_form($form, $form_state);
    }
    elseif (!empty($form_state['exposed'])) {
      // This project has no versions at all, hide this filter.      
      $this->view->filter['rid']->options['exposed'] = FALSE;
      // And exclude the field itself. Instead of assuming the field key is
      // 'version', search for a field using {project_release_nodes}.version
      // from the same relationship as this filter.
      foreach ($this->view->field as  $field) {
        if ($field->table == 'project_release_nodes' && $field->field == 'version') {
          $field->options['exclude'] = TRUE;
        }
      }
    }
    if (empty($form_state['exposed'])) {
      $form['warning'] = array(
        '#type' => 'markup',
        '#value' => t('You can only select options if you expose this filter and have a project argument'),
      );
    }
  }

  function init_version_info()  {
    if (empty($this->project_versions) && isset($this->view->argument['pid']) && module_exists('project_release')) {
      $arg = $this->view->argument['pid']->get_value();
      if (is_numeric($arg)) {
        $project_nid = $arg;
      }
      else {
        $project_nid = project_get_nid_from_uri($arg);
      }
      // This is needed by project_get_releases() and friends.
      $project = node_load($project_nid);
      $this->project_versions = project_release_get_releases($project, FALSE);
      if (!empty($this->project_versions)) {
        $active_tids = array_keys(project_release_compatibility_list());
        if (!empty($active_tids)) {
          $tid_placeholders = db_placeholders($active_tids);
          $nids = array_keys($this->project_versions);
          $nid_placeholders = db_placeholders($nids);
          $query_args = array_merge($active_tids, $nids);
          $query = db_query("SELECT td.name, td.tid, tn.nid FROM {term_data} td INNER JOIN {term_node} tn ON td.tid = tn.tid WHERE tn.tid IN ($tid_placeholders) AND tn.nid IN ($nid_placeholders) ORDER BY td.weight", $query_args);
          while ($release = db_fetch_object($query)) {
            $this->project_version_series[$release->name][] = $release->nid;
          }
        }
      }
    }
  }

  function get_value_options() {
    $this->value_options = array();
    // We only want to return real options if we're building an exposed filter
    // form, so we check the flag set in value_form().
    if ($this->is_exposed) {
      $this->init_version_info();
      if (!empty($this->project_version_series)) {
        foreach ($this->project_version_series as $series => $nids) {
          $this->value_options[$series] = t('- @series issues -', array('@series' => $series));
        }
      }
      if (!empty($this->project_versions)) {
        $this->value_options += $this->project_versions;
      }
    }
  }

  /**
   * Generate the query based on this filter.
   *
   * This is where the special logic for the "- $series issues -" choices is
   * converted back into the real release nid values for the underlying query.
   */
  function query() {
    if (isset($this->value)) {
      $this->init_version_info();
      $values = array();
      foreach ($this->value as $key => $value) {
        if (!empty($this->project_version_series[$value])) {
          $values += $this->project_version_series[$value];
        }
        else {
          $values[$key] = $value;
        }
      }
      $this->value = $values;
      parent::query();
    }
  }

}

