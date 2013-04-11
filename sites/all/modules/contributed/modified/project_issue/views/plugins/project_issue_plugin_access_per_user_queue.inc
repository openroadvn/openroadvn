<?php
// $Id: project_issue_plugin_access_per_user_queue.inc,v 1.1 2009/03/23 17:24:53 dww Exp $

/**
 * Validate whether an argument is a real user and return 403 if not.
 *
 * We use this for per-user issue queues ("My projects" and "My issues") to
 * return a 403 on cases where anonymous users visit the view without
 * providing a username or UID in the URL.
 */
class project_issue_plugin_access_per_user_queue extends views_plugin_access {

  /**
   * Check access outside of the menu system (e.g. used for block displays).
   */
  function access($account) {
    $this->view->set_display($this->display->id);
    $this->view->init_handlers();
    $user_arg = $this->options['project_issue_user_argument'];
    $argument = $this->view->argument[$user_arg];
    $arg_uid = $argument->get_value();
    return !empty($arg_uid);
  }

  function get_access_callback() {
    return array('project_issue_views_user_access', array($this->view->name, $this->display->id, $this->options['project_issue_user_argument']));
  }

  function summary_title() {
    return t('Has a per-user issue queue');
  }

  function option_defaults(&$options) {
    $options['project_issue_user_argument'] = '';
  }
  
  function options_form(&$form, &$form_state) {
    $arguments = array();
    foreach ($this->view->display_handler->get_handlers('argument') as $id => $handler) {
      $arguments[$id] = $handler->definition['title'];
    }
    $form['project_issue_user_argument'] = array(
      '#type' => 'select',
      '#options' => $arguments,
      '#title' => t('Project issue user argument'),
      '#description' => t('Select which argument represents the user whose issues this view will display.'),
      '#default_value' => $this->options['project_issue_user_argument'],
    );
  }

}

