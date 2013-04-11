<?php

function autotagging_settings_form() {
  $form = array();
  $implementing_modules = array();
  $form['global_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'Global Settings',
  );

  $vocabs = taxonomy_get_vocabularies();
  $vocab_options = array('<none>' => '<none>');
  foreach ($vocabs as $id => $vocab) {
    $vocab_options[$vocab->vid] = $vocab->name;
  }
  $form['global_settings']['autotagging_global_vocabulary'] = array(
    '#type' => 'select',
    '#title' => t('Global Vocabulary'),
    '#description' => t('If not overridden and not set to "none", terms found by autotagging modules will be merged into this category. '),
    '#options' => $vocab_options,
    '#default_value' => variable_get('autotagging_global_vocabulary', '')
  );

  $form['autotagging_settings'] = array(
    '#type' => 'fieldset',
    '#title' => 'AutoTagging Module Settings'
  );
  $types_options = drupal_map_assoc(array_keys(node_get_types()));

  $form['autotagging_settings']['autotagging_allowed_types'] = array(
    '#type' => 'select',
    '#title' => t('Auto-tag nodes of the following types'),
    '#description' => t('List of node types that autotagging should be enabled for.'),
    '#size' => count($types_options) > 5 ? 5 : count($types_options),
    '#options' => $types_options,
    '#multiple' => TRUE,
    '#default_value' => variable_get('autotagging_allowed_types', array())
  );

  $form['autotagging_settings']['autotagging_stop_words'] = array(
    '#type' => 'textarea',
    '#title' => t('Ignore the following words'),
    '#description' => t('Exclude the words in this list from being included as tags for a node.  Enter one word/phrase per line.'),
    '#default_value' => variable_get('autotagging_stop_words', '')
  );

  $form['autotagging_settings']['autotagging_error_handling'] = array(
    '#type' => 'select',
    '#title' => t('Error Handling'),
    '#description' => t('If an error occurs fetching terms from a third party service, define how to handle it.'),
    '#options' => drupal_map_assoc(array('Defer', 'Ignore')),
    '#default_value' => variable_get('autotagging_error_handling', 'Defer')
  );

  foreach (module_implements('autotagging_api_register') as $module_name) {
    $module = module_invoke($module_name, 'autotagging_api_register');
    if (!is_array($module)) {
      drupal_set_message('Autotagging module '. $module_name .' did not return a valid registration.', 'status');
      continue;
    }
    $implementing_modules[$module_name] = $module;

    $subform = module_invoke($module_name, 'autotagging_api_settings');
    if (is_array($subform)) {
      foreach ($subform as $key => $element) {
        $form[$key] = $element;
      }
    }
  }

  if (count($implementing_modules) == 0) {
    drupal_set_message(t('No modules that implement the autotagging api are installed.  Visit '. l('the Autotagging module project page', 'http://drupal.org/project/autotagging') .' for a list.'));
    unset($form['autotagging_settings']);
  }
  return system_settings_form($form);
}
