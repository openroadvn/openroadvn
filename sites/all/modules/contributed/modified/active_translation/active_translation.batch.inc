<?php
// $Id: active_translation.batch.inc,v 1.6.2.2 2009/07/12 17:54:03 drewish Exp $

/**
 * Build a batch to rebuild all the translation information.
 */
function active_translation_build_batch_all() {
  $batch = array(
    'title' => t('Rebuilding the active translation table'),
    'operations' => array(
      array('_active_translation_batch_operation', array()),
    ),
    'finished' => '_active_translation_batch_finished',
    'file' => drupal_get_path('module', 'active_translation') .'/active_translation.batch.inc',
  );

  return $batch;
}

function _active_translation_batch_operation(&$context) {
  if (empty($context['sandbox'])) {
    watchdog('active_translati', 'Starting to rebuild the active translation table.');

    // Initiate multistep processing. Call language_list() and reset its cache.
    $context['sandbox']['languages'] = array_keys(language_list('language', TRUE));
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['current_translation'] = 0;
    $context['sandbox']['max'] = db_result(db_query('SELECT COUNT(DISTINCT tnid) FROM {node} WHERE tnid IS NOT NULL AND tnid > 0'));

    // Drop and recreate the database table to ensure the columns match the
    // current list of languages. Clear the cache to get rid of any cached
    // schema info.
    drupal_uninstall_schema('active_translation');
    drupal_get_schema('active_translation', TRUE);
    drupal_install_schema('active_translation');

    // Insert records for language neutral and untranslated nodes.
    $fields = $values = array();
    // We need to put the node id into the column for each language.
    foreach ($context['sandbox']['languages'] as $lang_code) {
      $fields[] = db_escape_table($lang_code);
      $values[] = 'nid';
    }
    // We can't use db_placeholders() because we're using field names rather
    // than values.
    db_query('INSERT INTO {active_translation} (atid, '. implode(', ', $fields) .') SELECT DISTINCT(nid), '. implode(', ', $values) .' FROM {node} WHERE tnid = 0 OR tnid IS NULL');

    watchdog('active_translati', 'Created records for untranslated and language neutral nodes.');
  }

  // Process the next set of translations.
  $limit = 20;
  $result = db_query_range("SELECT DISTINCT tnid FROM {node} WHERE tnid > %d ORDER BY tnid ASC", $context['sandbox']['current_translation'], 0, $limit);
  while ($row = db_fetch_object($result)) {
    $node = node_load($row->tnid);
    if (empty($node->nid)) {
      watchdog('active_translati', 'Could not load the node %nid.', array('%nid' => $row->tnid), WATCHDOG_ERROR);
    }
    else {
      $translation = active_translation_recompute($node);
      drupal_write_record('active_translation', $translation);
    }
    $context['sandbox']['progress']++;
    $context['sandbox']['current_translation'] = $row->tnid;
  }

  // Multistep processing: report progress.
  if ($context['sandbox']['progress'] < $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _active_translation_batch_finished($success, $results, $operations) {
  if ($success) {
    watchdog('active_translati', 'Sucessfully rebuilt the active translation table.');
    drupal_set_message(t('The active translation table has rebuilt.'));
  }
  else {
    watchdog('active_translati', 'There was an error that prevented the active translation table from being properly rebuilt.', array(), WATCHDOG_ERROR);
    drupal_set_message(t('The active translation table has not been properly rebuilt.'), 'error');
  }
  cache_clear_all();
}

