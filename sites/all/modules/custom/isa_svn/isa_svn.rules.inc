<?php

function isa_svn_rules_action_info() {
  return array(
      'isa_svn_action_new_repo' => array(
          'label' => t('Create a source code repository'),
          'module' => 'Project',
          'parameter' => array(
              'node' => array(
                  'type' => 'node',
                  'label' => t('Recipient'),
              ),
          ),
      ),
  );
}

function isa_svn_action_new_repo($settings, $arg2, $context) {
  $node = $context['variables']['node']->data->_data;
  if ($node->tnid == 0 || $node->nid == $node->tnid) {
    $repository_name = $node->project['uri'];

    // In a previous implementation, we used to execute svnadmin commands on a
    // distant server through SSH. However, this approach has some disavantages:
    // 1 - It creates a potential security breach for the Subversion-dedicated
    // architecture because it brings an exploitable, automated SSH access.  2 -
    // If the remote server is unavailable, it may make the current Drupal page
    // time out, with the risk of the user refreshing the page, thus creating a
    // new attempt to initialize the Subversion repository, without any
    // synchronization with the previous attempt.
    // The final, chosen approach simply consists in writing a record in a MySQL
    // table. It is then the responsibility of a cron job (a dedicated daemon
    // would have been technically better but would have required much more work
    // for little gain) running on the Subversion architecture to read this table
    // regularly and create the Subversion repositories accordingly.
    // we'll display a message to the user
    $message = t('The creation of the ":repository_name" SVN repository has been planned.', array(':repository_name' => $repository_name));

    // first, we avoid creating a task record for an already required creation
    $action = 'svn_creation';
    $state = 'todo';
    $check_query = 'SELECT COUNT(*) FROM {repositories_management} WHERE action = \'%s\' AND repository = \'%s\' AND state = \'%s\'';
    $check_res = db_query($check_query, $action, $repository_name, $state);
    if ($check_res) {
      if (db_result($check_res) > 0) {
        drupal_set_message($message);
        return;
      }
    }

    // eventually, we insert the task in the table
    $insert_query = 'INSERT INTO {repositories_management} (repository, action, state) VALUES (\'%s\', \'%s\', \'%s\')';
    db_query($insert_query, $repository_name, $action, $state);
    drupal_set_message($message);
  }
}
