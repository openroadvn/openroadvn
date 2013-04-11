<?php
// $Id

/**
 * Context Help Module administration settings
 */
function contexthelp_admin_settings() {
  $options = array();
  if (($node_types = node_get_types()) && is_array($node_types)) {
    foreach ($node_types as $key => $values) {
      $options[$key] = $values->name;
    }
  }

  $form['contexthelp'] = array('#type' => 'fieldset', '#title' => t('Context Help Settings'), '#collapsible' => TRUE, '#collapsed' => FALSE);
  $form['contexthelp']['contexthelp_node_type'] = array(
    '#type' => 'select',
    '#title' => t('Context Help Node Type'),
    '#default_value' => variable_get('contexthelp_node_type', 'contexthelp'),
    '#options' => $options,
    '#description' => t('Node type that we will utilize as a context help node type for saving the help and also displaying the data'),
  );
  $form['contexthelp']['contexthelp_faq_node_type'] = array(
    '#type' => 'select',
    '#title' => t('Context Help FAQ Node Type'),
    '#default_value' => variable_get('contexthelp_faq_node_type', 'contexthelp_faq'),
    '#options' => $options,
    '#description' => t('Node type that we will utilize as a the FAQ portion of the context help module'),
  );
  $form['contexthelp']['contexthelp_help_button_in_theme'] = array(
    '#type' => 'checkbox',
    '#title' => t('Help Button is in theme files'),
    '#default_value' => variable_get('contexthelp_help_button_in_theme', false),
    '#description' => t('Check this option if the help button is going to be displayed in the theme rather than by the module itself'),
  );
  $form['contexthelp']['contexthelp_disable_help_button'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable help button on pages with no help'),
    '#default_value' => variable_get('contexthelp_disable_help_button', false),
    '#description' => t('Click if you wish to disable the context help button on pages that do not have any help for the current page'),
  );
  $form['contexthelp']['contexthelp_disable_all_help'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check to disable the All Help in the popup'),
    '#default_value' => variable_get('contexthelp_disable_all_help', false),
    '#description' => t('This allows the user to not display the All Help portion in the popup. The Current Help will just be displayed and possibly FAQ unless it is turned off also'),
  );
  $form['contexthelp']['contexthelp_disable_faq'] = array(
    '#type' => 'checkbox',
    '#title' => t('Check to disable the FAQ tab in the popup'),
    '#default_value' => variable_get('contexthelp_disable_faq', false),
    '#description' => t('This allows the user to not display the All Help portion in the popup. The Current Help will just be displayed and possibly All Help unless it is turned off also'),
  );
  $form['contexthelp']['contexthelp_pager_count'] = array(
    '#type' => 'textfield',
    '#title' => t('Pager Display Rows'),
    '#default_value' => variable_get('contexthelp_pager_count', 20),
    '#description' => t('The number of rows in the table that should be displayed when administering the contexthelp nodes'),
  );
  $form['contexthelp']['contexthelp_disable_on_specific_pages'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable Help on Specific Pages'),
    '#default_value' => variable_get('contexthelp_disable_on_specific_pages', false),
    '#description' => t('If checked the list you provide on !url will be checked against', array('!url' => l(t('Url Mapping Page'), 'admin/contexthelp/url/mappings'))),
  );
  // If the Locale Module exists allow the user to specify the default langauge for help content display
  if (module_exists('locale')) {
    $form['contexthelp']['contexthelp_default_locale'] = array(
      '#type' => 'radios',
      '#title' => t('Default Context Help Language'),
      '#default_value' => variable_get('contexthelp_default_locale', 'en'),
      '#options' => locale_language_list('name'),
      '#description' => t('Language the contexthelp module will default too if the users locale settings cannot be resolved'),
    );
  }
  return system_settings_form($form);
}
