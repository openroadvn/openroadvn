<?php
// $Id: nat.admin.inc,v 1.1.2.3 2010/03/04 21:51:11 anantagati Exp $

/**
 * @file
 * NAT module administrative forms.
 */

/**
 * Menu callback: NAT module settings form.
 */
function nat_settings_form() {
  $types = node_get_types();
  $vocabularies = _nat_get_vocabularies();

  if (empty($vocabularies)) {
    drupal_set_message(t('The NAT module requires at least one vocabulary to be defined.'), 'error');
    drupal_goto('admin/content/taxonomy');
  }

  $nat_config = _nat_variable_get();

  foreach ($types as $type => $type_object) {
    $collapsed = (!isset($nat_config['types'][$type])) || (empty($nat_config['types'][$type]));
    $form['nat_'. $type] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($type_object->name),
      '#collapsible' => TRUE,
      '#collapsed' => $collapsed
    );
    $form['nat_'. $type][$type] = array(
      '#type' => 'select',
      '#title' => t('Vocabularies'),
      '#options' => $vocabularies,
      '#default_value' => isset($nat_config['types'][$type]) ? $nat_config['types'][$type] : array(),
      '#multiple' => TRUE,
      '#description' => t('Creating a node of type %type will automatically create a term in any selected vocabularies.', array('%type' => $type)),
      '#parents' => array('types', $type)
    );
    $form['nat_'. $type]['body_'. $type] = array(
      '#type' => 'checkbox',
      '#title' => t('Associate node body with term description.'),
      '#default_value' => isset($nat_config['body'][$type]) ? $nat_config['body'][$type] : 0,
      '#parents' => array('body', $type)
    );
    $form['nat_'. $type]['delete_'. $type] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete associated term if a node is deleted.'),
      '#default_value' => isset($nat_config['delete'][$type]) ? $nat_config['delete'][$type] : 0,
      '#parents' => array('delete', $type)
    );
    $form['nat_'. $type]['related_'. $type] = array(
      '#type' => 'checkbox',
      '#title' => t('Allow users to define synonyms and related terms when they create and edit nodes.'),
      '#default_value' => isset($nat_config['related'][$type]) ? $nat_config['related'][$type] : 0,
      '#parents' => array('related', $type)
    );
    $form['nat_'. $type]['node_links_'. $type] = array(
      '#type' => 'checkbox',
      '#title' => t('Make NAT terms in %type node views point to the associated node rather than the taxonomy page.', array('%type' => $type)),
      '#default_value' => isset($nat_config['node_links'][$type]) ? $nat_config['node_links'][$type] : 0,
      '#parents' => array('node_links', $type)
    );
  }
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save configuration'));

  return $form;
}

/**
 * Process NAT settings form submissions.
 */
function nat_settings_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];

  unset($form_values['form_id'], $form_values['form_build_id'], $form_values['submit'], $form_values['op'], $form_values['form_token']);

  $form_values['body'] = array_filter($form_values['body']);
  $form_values['delete'] = array_filter($form_values['delete']);
  $form_values['node_links'] = array_filter($form_values['node_links']);
  $form_values['related'] = array_filter($form_values['related']);

  variable_set('nat_config', $form_values);

  drupal_set_message(t('Configuration settings saved.'));
}

/**
 * Sync the NAT table with the node table for associated vocabularies.
 */
function nat_sync_form() {
  $vocabularies = _nat_get_vocabularies();

  if (empty($vocabularies)) {
    drupal_set_message(t('The NAT module requires at least one vocabulary to be defined.'), 'error');
    drupal_goto('admin/content/taxonomy');
  }

  $nat_config = _nat_variable_get();
  $options = array();
  foreach ($nat_config['types'] as $type => $associations) {
    if (!empty($associations)) {
      foreach ($associations as $vid) {
        $options[$type .'|'. $vid] = t('@type &lsaquo;-&rsaquo; !vocabulary', array('@type' => $type, '!vocabulary' => $vocabularies[$vid]));
      }
    }
  }
  if (empty($options)) {
    drupal_set_message(t('There are no vocabularies available to sync.'));
    drupal_goto('admin/settings/nat');
  }

  $form['sync'] = array(
    '#type' => 'fieldset',
    '#title' => t('Sync associations'),
    '#description' => t('The Sync operation will create NAT associations (and terms) for nodes (marked for NAT association) not present in the NAT table.'),
    '#collapsible' => TRUE
  );
  $form['sync']['vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select the vocabularies to sync with associated node tables'),
    '#description' => t('Any nodes not already NAT associated with the selected vocabularies will be associated.'),
    '#required' => TRUE,
    '#options' => $options
  );
  $form['submit'] = array('#type' => 'submit', '#value' => t('Sync tables'));

  return $form;
}

/**
 * Process NAT sync form submissions.
 */
function nat_sync_form_submit($form, &$form_state) {
  _nat_sync_associations(array_filter($form_state['values']['vocabularies']));
}

/**
 * Synchronize NAT node-term relationships. Create associated terms for node
 * where missing.
 *
 * @param $associations
 *   Associative array denoting the node-vocabulary pair that is to be synced.
 */
function _nat_sync_associations($associations) {
  $nat_config = _nat_variable_get();

  $counter = 0;
  foreach ($associations as $association) {
    $association = explode('|', $association);
    // This query can possibly be improved.
    $result = db_query("SELECT n.nid, n.type, n.title, nr.body FROM {node} n INNER JOIN {node_revisions} nr USING (vid) LEFT JOIN {nat} n1 ON (n.nid = n1.nid AND n1.vid = %d) LEFT JOIN {nat} n2 ON (n.nid = n2.nid AND n2.nid <> n1.nid) WHERE n.type = '%s' AND n1.nid IS NULL", $association[1], $association[0]);
    while ($node = db_fetch_object($result)) {
      // Add node title as terms.
      $terms = _nat_add_terms($node, array($association[1]));

      // Save node-term association in the NAT table.
      _nat_save_association($node->nid, $terms);

      $counter++;
    }
  }
  drupal_set_message(t('NAT sync complete: %count nodes synced.', array('%count' => $counter)));
}
