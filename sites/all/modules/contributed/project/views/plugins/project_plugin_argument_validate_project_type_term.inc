<?php

/**
 * Validate whether an argument is a term in the project type vocabulary and if so possibly do something special
 * depending on whether or not the term is a parent term with children, a parent term without children,
 * or a child term.
 */
class project_plugin_argument_validate_project_type_term extends views_plugin_argument_validate {
  function validate_form(&$form, &$form_state) {
    // Vocabularies
    $form['validate_argument_project_term_vocabulary'] = array(
      '#type' => 'checkboxes',
      '#prefix' => '<div id="edit-options-validate-argument-project-term-vocabulary-wrapper">',
      '#suffix' => '</div>',
      '#title' => t('Vocabularies'),
      '#options' => _project_views_plugin_argument_validate_project_vocabulary(),
      '#default_value' => isset($this->argument->options['validate_argument_project_term_vocabulary']) ? $this->argument->options['validate_argument_project_term_vocabulary'] : array(),
      '#description' => t('If you wish to validate for specific vocabularies, check them; if none are checked, all arguments will fail validation.'),
      '#process' => array('expand_checkboxes', 'views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    // Argument type
    $form['validate_argument_project_term_argument_type'] = array(
      '#type' => 'select',
      '#title' => t('Argument type'),
      '#options' => array(
        'tid' => t('Term ID'),
        'name' => t('Term name or synonym'),
        'convert' => t('Term name/synonym converted to Term ID'),
      ),
      '#default_value' => isset($this->argument->options['validate_argument_project_term_argument_type']) ? $this->argument->options['validate_argument_project_term_argument_type'] : 'tid',
      '#description' => t('Select the form of this argument; if using term name, it is generally more efficient to convert it to a term ID and use Taxonomy: Term ID rather than Taxonomy: Term Name" as an argument.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    // @TODO:  Project term actions fieldset/grouping?    

    $project_term_actions = array(
      'pass' => t('Pass validation'),
      'fail_delete_argument' => t('Fail validation and delete argument'),
      'fail' => t('Fail validation'),
    );
    // Top level term without children
    $form['validate_argument_project_term_argument_action_top_without'] = array(
      '#type' => 'select',
      '#title' => t('Term is a top level term without children'),
      '#options' => $project_term_actions,
      '#default_value' => isset($this->argument->options['validate_argument_project_term_argument_action_top_without']) ? $this->argument->options['validate_argument_project_term_argument_action_top_without'] : 'pass',
      '#description' => t('Terms that are top level project type terms but which have no child terms will be handled as selected here.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    // Top level term with children
    $form['validate_argument_project_term_argument_action_top_with'] = array(
      '#type' => 'select',
      '#title' => t('Term is a top level term with children'),
      '#options' => $project_term_actions,
      '#default_value' => isset($this->argument->options['validate_argument_project_term_argument_action_top_with']) ? $this->argument->options['validate_argument_project_term_argument_action_top_with'] : 'pass',
      '#description' => t('Terms that are top level project type terms but which do have child terms will be handled as selected here.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );

    // Child term
    $form['validate_argument_project_term_argument_action_child'] = array(
      '#type' => 'select',
      '#title' => t('Term is a child of another term'),
      '#options' => $project_term_actions,
      '#default_value' => isset($this->argument->options['validate_argument_project_term_argument_action_child']) ? $this->argument->options['validate_argument_project_term_argument_action_child'] : 'pass',
      '#description' => t('Terms that are not top level project type terms will be handled as selected here.'),
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
    );
  }

  function validate_argument($argument) {
    $vids = isset($this->argument->options['validate_argument_project_term_vocabulary']) ? array_filter($this->argument->options['validate_argument_project_term_vocabulary']) : array();
    $type = isset($this->argument->options['validate_argument_project_term_argument_type']) ? $this->argument->options['validate_argument_project_term_argument_type'] : 'tid';
    $action_top_without = isset($this->argument->options['validate_argument_project_term_argument_action_top_without']) ? $this->argument->options['validate_argument_project_term_argument_action_top_without'] : 'pass';
    $action_top_with = isset($this->argument->options['validate_argument_project_term_argument_action_top_with']) ? $this->argument->options['validate_argument_project_term_argument_action_top_with'] : 'pass';
    $action_child = isset($this->argument->options['validate_argument_project_term_argument_action_child']) ? $this->argument->options['validate_argument_project_term_argument_action_child'] : 'pass';

    // If no vocabularies were selected in the options form, we can fail right away.
    if (empty($vids)) {
      return FALSE;
    }

    // First we check to determine whether the argument is a term in the project type vocabulary.
    // If not, we know the argument won't validate, and we can return FALSE.  The other added benefit
    // is that in the switch statement below we ensure that we have the actual term ID of the argument,
    // which makes the subsequent query easier to write and faster.
    switch ($type) {
      case 'tid':
        if (!is_numeric($argument)) {
          return FALSE;
        }
        $result = db_fetch_object(db_query(db_rewrite_sql("SELECT td.tid, td.vid, td.name, td.description, td.weight, th.parent FROM {term_data} td LEFT JOIN {term_hierarchy} th ON th.tid = td.tid WHERE td.tid = %d", 'td', 'tid'), $argument));
        if (!$result) {
          return FALSE;
        }
        elseif (empty($vids[$result->vid])) {
          return FALSE;
        }
        break;

      case 'name':
      case 'convert':
        $query_args = array($argument, $argument);
        $query_args = array_merge($query_args, $vids);
        $result = db_fetch_object(db_query(db_rewrite_sql("SELECT td.tid, td.vid, td.name, td.description, td.weight, th.parent FROM {term_data} td LEFT JOIN {term_synonym} ts ON ts.tid = td.tid LEFT JOIN {term_hierarchy} th ON th.tid = td.tid WHERE (td.name = '%s' OR ts.name = '%s') AND td.vid IN(" . db_placeholders($vids) . ")", 'td', 'tid'), $query_args));
        if (!$result) {
          return FALSE;
        }

        if ($type == 'convert') {
          $this->argument->argument = $result->tid;
        }
        break;
    }

    if ($result->parent == 0) {
      // This term is a top level term.
      // Before we know what to do, we need to determine if this term
      // has any child terms or not.
      $has_children = db_result(db_query("SELECT COUNT(*) FROM {term_hierarchy} WHERE parent = %d", $result->tid));
      if ($has_children) {
        $action = $action_top_with;
      }
      else {
        $action = $action_top_without;
      }
    }
    else {
      // This term is a child term.
      $action = $action_child;
    }

    switch ($action) {
      case 'pass':
        return TRUE;
      case 'fail_delete_argument':
        $this->argument->options['validate_fail'] = $this->argument->options['default_action'];
        return FALSE;
      case 'fail':
        return FALSE;
    }
  }
}

/**
 * Return an array containing the project type vocabulary
 * formatted to be used in the argument validator options form.
 */
function _project_views_plugin_argument_validate_project_vocabulary() {
  $vid = _project_get_vid();
  if (!empty($vid)) {
    $vocabulary = taxonomy_vocabulary_load($vid);
  }
  
  $options = array();
  if (!empty($vocabulary)) {
    $options[$vocabulary->vid] = $vocabulary->name;
  }
  return $options;
}
