<?php

function isa_ml_action_new_list_form($settings, &$form, &$form_state) {
  $initial_members_value = implode("\n", variable_get('isa_ml_initial_members', ''));
  $form['initial_members'] = array(
    '#type' => 'textarea',
    '#title' => t('Initial members for all newly created lists'),
    '#default_value' => $initial_members_value,
    '#description' => t('One email address per line; please ensure you enter plain text.'),
  );
}

function isa_ml_action_new_list_validate($form, $form_state) {
  $settings = $form_state['#post']['initial_members'];
  // placeholder
}

function isa_ml_action_new_list_submit(&$settings, &$form, &$form_state) {
  // $settings['initial_members'] = $form_state['clicked_button']['#post']['initial_members'];
  $new_initial_members = preg_split("/\n/", $form_state['clicked_button']['#post']['initial_members'], -1, PREG_SPLIT_NO_EMPTY);
  variable_set('isa_ml_initial_members', $new_initial_members);
}
