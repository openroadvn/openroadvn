<?php
// $Id: 

/**
 * Implementation of hook_views_handlers().
 */
function isa_ogur_views_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'isa_ogur_views') .'/includes',
    ),
    'handlers' => array(
      'og_user_roles_handler_field_has_role' => array(
        'parent' => 'views_handler_field_boolean',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data().
 */
function isa_ogur_views_views_data() {
  $data = array();
  $data['og_uid']['has_role'] = array(
    'title' => t('Group role'),
    'help' => t('Whether or not the current user has a given role within the current group'),
    'field' => array(
      'handler' => 'og_user_roles_handler_field_has_role',
      'click sortable' => FALSE,
    ),
  );
  return $data;
}

