<?php
// $Id: autocomplete_widgets.admin.inc,v 1.1.2.2 2009/10/04 17:03:17 markuspetrux Exp $

/**
 * @file
 * Administration related code for Autocomplete Widgets module.
 */

/**
 * Alter the CCK widget settings form.
 */
function _autocomplete_widgets_content_field_edit_form_alter(&$form, $form_state) {
  $widget_type = $form['#field']['widget']['type'];
  if (in_array($widget_type, array('autocomplete_widgets_allowvals', 'autocomplete_widgets_flddata'))) {
    // Implementation of allowed values list.
    if ($widget_type == 'autocomplete_widgets_allowvals') {
      $form['field']['allowed_values_fieldset']['#collapsed'] = FALSE;
      $form['field']['allowed_values_fieldset']['#description'] = '<p>'. t('Create a list of options as a list in <strong>Allowed values list</strong> or as an array in PHP code. These values will be the same for %field in all content types.', array('%field' => $form['#field']['widget']['label'])) .'</p>';

      // If no 'allowed values' were set yet, add a remainder in the messages area.
      if (empty($form_state['post'])
      && empty($form['field']['allowed_values_fieldset']['allowed_values']['#default_value'])
      && empty($form['field']['allowed_values_fieldset']['advanced_options']['allowed_values_php']['#default_value'])) {
        drupal_set_message(t("You need to specify the 'allowed values' for this field."), 'warning');
      }
    }
    else {
      // For the autocomplete 'field data' widget we do not use the 'Allowed values' list.
      unset($form['field']['allowed_values_fieldset']);
    }
    // Disable the text processing option of text fields. These widgets can
    // only use plain text fields.
    if (isset($form['field']['text_processing'])) {
      $form['field']['text_processing']['#type'] = 'value';
      $form['field']['text_processing']['#value'] = 0;
    }
  }
}

/**
 * Implementation of hook_widget_settings().
 */
function _autocomplete_widgets_widget_settings($op, $widget) {
  switch ($op) {
    case 'form':
      $form = array();
      $form['size'] = array(
        '#type' => 'textfield',
        '#title' => t('Size of textfield'),
        '#default_value' => (isset($widget['size']) ? $widget['size'] : 60),
        '#element_validate' => array('_element_validate_integer_positive'),
        '#required' => TRUE,
      );
      $form['autocomplete_match'] = array(
        '#type' => 'select',
        '#title' => t('Autocomplete matching'),
        '#default_value' => (isset($widget['autocomplete_match']) ? $widget['autocomplete_match'] : 'contains'),
        '#options' => array('starts_with' => t('Starts with'), 'contains' => t('Contains')),
        '#description' => t('Select the method used to collect autocomplete suggestions. Note that <em>Contains</em> can cause performance issues on sites with thousands of records.'),
      );
      $form['autocomplete_case'] = array(
        '#type' => 'radios',
        '#title' => t('Case sensitive'),
        '#default_value' => (isset($widget['autocomplete_case']) ? $widget['autocomplete_case'] : 1),
        '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
      );
      if ($widget['type'] == 'autocomplete_widgets_flddata') {
        $form['autocomplete_case']['#description'] = t('Case-insensitive queries are implemented using the <a href="!function-lower-url">LOWER()</a> function in combination with the <a href="!operator-like-url">LIKE</a> operator.', array(
          '!function-lower-url' => 'http://dev.mysql.com/doc/refman/5.1/en/string-functions.html#function_lower',
          '!operator-like-url' => 'http://dev.mysql.com/doc/refman/5.1/en/string-comparison-functions.html#operator_like',
        ));
        if (in_array($GLOBALS['db_type'], array('mysql', 'mysqli'))) {
          $form['autocomplete_case']['#description'] .= ' '. t('Note that MySQL might ignore case sensitivity depending on the collation used in your database definition (see <a href="!mysql-i18n-l10n-url">Internationalization and Localization</a> chapter in the MySQL manual). If you need case insensitive checks, it is recommended (for performance reasons) to use a case insensitive collation as well (such as utf8_general_ci), rather than disabling the case sensitive option here.', array(
            '!mysql-i18n-l10n-url' => 'http://dev.mysql.com/doc/refman/5.1/en/internationalization-localization.html',
          ));
        }
        elseif ($GLOBALS['db_type'] == 'pgsql') {
          $form['autocomplete_case']['#description'] .= ' '. t('You may want to create an expression index using the LOWER() function to speed up this kind of queries in PostgreSQL (See <a href="!indexes-expressional-url">Indexes on Expressions</a>).', array(
            '!indexes-expressional-url' => 'http://www.postgresql.org/docs/8.4/static/indexes-expressional.html',
          ));
        }
      }
      else {
        $form['autocomplete_case']['#description'] = t('Case-insensitive queries are implemented using the function <a href="!drupal-strtolower-url">drupal_strtolower()</a>.', array(
          '!drupal-strtolower-url' => 'http://api.drupal.org/api/function/drupal_strtolower/6',
        ));
      }
      if ($widget['type'] == 'autocomplete_widgets_flddata' && module_exists('i18n')) {
        $form['i18n_flddata'] = array(
          '#type' => 'radios',
          '#title' => t('Internationalization support'),
          '#default_value' => (isset($widget['i18n_flddata']) ? $widget['i18n_flddata'] : 0),
          '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
          '#description' => t('Enable this option to provide a different set of allowed values based on the language their nodes are assigned to. This option is only available when <a href="@i18n-project-page">Internationalization</a> module is enabled.', array('@i18n-project-page' => 'http://drupal.org/project/i18n')),
        );
      }
      else {
        $form['i18n_flddata'] = array(
          '#type' => 'value',
          '#value' => 0,
        );
      }
      return $form;

    case 'save':
      return array('size', 'autocomplete_match', 'autocomplete_case', 'i18n_flddata');
  }
}
