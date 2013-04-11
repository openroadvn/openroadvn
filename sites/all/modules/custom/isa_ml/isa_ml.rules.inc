<?php

/**
 * Implementation of hook_action_info().
 */
function isa_ml_rules_action_info() {
  return array(
    'isa_ml_action_new_list' => array(
      'label' => t('Create a mailing list'),
      'module' => 'Project',
      'parameter' => array(
        'node' => array(
          'type' => 'node',
          'label' => t('Parent project/community'),
        ),
      ),
    ),
  );
}

/**
 * Requires the creation of a mailing list for a given project/community
 * (actually for the node provided within the context)
 * @param $settings Array describing this action settings
 * @param $action   Array describing this action
 * @param $context  Array describing the context this action must execute into.
 */
function isa_ml_action_new_list($settings, $action, $context) {
  // The final, chosen approach simply consists in writing a record in a MySQL
  // table. It is then the responsibility of a cron job running on the Mailman
  // architecture to read this table regularly and create the mailing lists
  // accordingly. It will also configure them and subscribe a list of "initial
  // members".

  // retrieve the node associated with the given context
  $node = $context['variables']['node']->data->_data;

  // Ensure we do not create a mailing list for a translation node
  if (!($node->tnid == 0 || $node->nid == $node->tnid)) {
    return;
  }

  // we'll display a message to user
  $message = 'The creation of the `:mailing_list\' mailing list has been planned.';
  $error_message = 'Unable to require the creation of the `:ml_name\' mailing list: :reason';

  // Retrieve settings that will be applied to the newly created list
  // unlike normal actions, we fetch these settings from the {variable} table
  // because it can be easily overridden at deployment time in the settings.php
  // file.
  $ml_default_configuration = variable_get('isa_ml_default_configuration', array());
  $ml_initial_members       = variable_get('isa_ml_initial_members',       array());

  // Retrieve the name of the future mailing list
  if ($node->type == 'community') {
    $ml_name = $node->field_community_short_name[0]['value'];
  }
  else {
    $ml_name = $node->project['uri'];
  }

  if (!strlen(trim($ml_name))) {
    drupal_set_message(t($error_message, array(':ml_name' => $ml_name, ':reason' => t('invalid name'))), 'error');
    return FALSE;
  }

  // Retrieve the owner of the future mailing list
  $ml_owner = user_load($node->uid);
  if (!$ml_owner) {
    drupal_set_message(t($error_message, array(':ml_name' => $ml_name, ':reason' => t('invalid owner'))), 'error');
    return FALSE;
  }
  $ml_owner_addr = $ml_owner->mail;

  // first, we avoid creating a task record for an already required creation
  $action = 'ml_creation';
  $state = 'todo';
  $check_query = 'SELECT COUNT(*) FROM {ml_management} WHERE action = \'%s\' AND ml = \'%s\' AND state = \'%s\'';
  $check_res = db_query($check_query, $action, $ml_name, $state);
  if ($check_res) {
    if (db_result($check_res) > 0) {
      drupal_set_message(t($message, array(':mailing_list' => $ml_name)));
      return;
    }
  }

  // eventually, we insert the task in the table
  $insert_query = 'INSERT INTO {ml_management} (ml, p1, p2, p3, action, state) VALUES (\'%s\', \'%s\', \'%s\', \'%s\', \'%s\', \'%s\')';
  db_query($insert_query, $ml_name, $ml_owner_addr, serialize($ml_default_configuration), serialize($ml_initial_members), $action, $state);
  // we insert the mailing list in the mailman manager table
  $insert_query = 'INSERT INTO {mailman_lists} (name, command, admin) VALUES (\'%s\', \'%s\', \'%s\')';
  db_query($insert_query, $ml_name, $ml_name . '-request@' . variable_get('isa_ml_domain', 'sharepa.eu'), $ml_name . '-admin@' . variable_get('isa_ml_domain', 'sharepa.eu'));
  drupal_set_message(t($message, array(':mailing_list' => $ml_name)));
}
