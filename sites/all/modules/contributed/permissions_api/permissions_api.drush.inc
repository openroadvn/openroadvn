<?php
// $Id: permissions_api.drush.inc,v 1.1.2.5 2010/09/02 00:19:53 ebeyrent Exp $

/**
 * Implementation of hook_drush_help().
 *
 * This function is called whenever a drush user calls
 * 'drush help <name-of-your-command>'
 *
 * @param
 *   A string with the help section (prepend with 'drush:')
 *
 * @return
 *   A string with the help text for your command.
 */
function permissions_api_drush_help($section) {
    switch ($section) {
      case 'drush:perm-grant':
        return dt("Grant permissions to a role.");
      case 'drush:perm-revoke':
        return dt("Revoke permissions from a role.");
      case 'drush:perm-copy':
        return dt("Copy permissions from one role to another.");
      case 'drush:perm-list':
        return dt("List permissions assigned to a role.");
      case 'drush:role-create':
        return dt("Create a role.");
      case 'drush:role-delete':
        return dt("Delete a role.");
    }
}

/**
 * Implementation of hook_drush_command().
 *
 * In this hook, you specify which commands your
 * drush module makes available, what it does and 
 * description.
 *
 * Notice how this structure closely resembles how 
 * you define menu hooks.
 *
 * @return
 *   An associative array describing your command(s).
 */
function permissions_api_drush_command() {
  $items=array();

  $items['perm-grant'] = array(
    'callback' => 'drush_permissions_api_perm_grant',
    'description' => dt('Grant permissions to a role.'),
    'arguments' => array(
      'role' => dt('The name of the role you are granting to.'),
      'perm' => dt('The permissions you are granting, comma separated. "all" for all permissions.'),
    ),
    'options' => array(
      'module' => dt('Only work with permissions from specified module'),
    ),
    'examples' => array(
      'drush perm-grant Admin all' => dt('Grant all permissions to Admin role.'),
      'drush perm-grant Editor administer_nodes' => dt('Grant administer_nodes to Editor role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  $items['perm-revoke'] = array(
    'callback' => 'drush_permissions_api_perm_revoke',
    'description' => dt('Revoke permissions from a role.'),
    'arguments' => array(
      'role' => dt('The name of the role you are revoking from.'),
      'perm' => dt('The permissions you are revoking, comma separated.'),
    ),
    'options' => array(
      'module' => dt('Only work with permissions from specified module'),
    ),
    'examples' => array(
      'drush perm-revoke Editor site_configuration' => dt('Revoke site_configuration permissions from Editor role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  $items['perm-copy'] = array(
    'callback' => 'drush_permissions_api_perm_copy',
    'description' => dt('copy permissions from one role to another.'),
    'arguments' => array(
      'source role' => dt('The name of the role you are copying from.'),
      'destination role' => dt('The name of the role you are copying to.'),
    ),
    'examples' => array(
      'drush perm-copy Editor Manager' => dt('Copy permissons from Editor role to Manager role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  $items['role-create'] = array(
    'callback' => 'drush_permissions_api_role_create',
    'description' => dt('Create a role.'),
    'arguments' => array(
      'role' => dt('The name of the role you are creating.'),
    ),
    'examples' => array(
      'drush role-create Admin' => dt('Create Admin role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  $items['role-delete'] = array(
    'callback' => 'drush_permissions_api_role_delete',
    'description' => dt('Delete a role.'),
    'arguments' => array(
      'role' => dt('The name of the role you are deleting.'),
    ),
    'examples' => array(
      'drush role-delete Admin' => dt('Delete Admin role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  $items['perm-list'] = array(
    'callback' => 'drush_permissions_api_perm_list',
    'description' => dt('List permissions for a role.'),
    'arguments' => array(
      'role' => dt('The name of the role you are querying.'),
    ),
    'examples' => array(
      'drush perm-list Admin' => dt('List permissions for Admin role.'),
    ),
    'drupal dependencies' => array('permissions_api'),
  );

  return $items;
}

/**
 * Command callbacks.
 */
function drush_permissions_api_perm_grant() {
  $args = func_get_args();
  $role = $args[0];
  array_shift($args);
  $module = drush_get_option('module');

  if ($module) {
    permissions_grant_all_permissions_by_module($role, $module);
    drush_print(dt('Granted all !module permissions to !role.', array('!module' => $module, '!role' => $role)));
  }
  elseif ($args[0] == 'all') {
    permissions_grant_all_permissions($role);
    drush_print(dt('All permissions granted to !role.', array('!role' => $role)));
  }
  else {
    permissions_grant_permissions($role, $args);
    drush_print(dt('Specified Permissions granted to !role.', array('!role' => $role)));
  }
}

function drush_permissions_api_perm_revoke() {
  $args = func_get_args();
  $role = $args[0];
  array_shift($args);
  $module = drush_get_option('module');

  if ($module) {
    permissions_revoke_all_permissions_by_module($role, $module);
    drush_print(dt('Revoked all !module permissions from !role.', array('!module' => $module, '!role' => $role)));
  }
  else {
    permissions_revoke_permissions($role, $args);
    drush_print(dt('Specified Permissions revoked from !role.', array('!role' => $role)));
  }
}

function drush_permissions_api_perm_copy() {
  $args = func_get_args();
  permissions_role_inherit($args[1], $args[0]);
  drush_print(dt('Permissions copied from !from to !to.', array('!from' => $args[0], '!to' => $args[1])));
}

function drush_permissions_api_perm_list() {
  $args = func_get_args();
  foreach($args as $role) {
    $perms = permissions_get_permissions_for_role($role);
    drush_print(dt('Permissions for role !role:', array('!role' => $role)));
    foreach($perms as $perm) {
      drush_print('    '.$perm);
    }
  }
}

function drush_permissions_api_role_create() {
  $args = func_get_args();
  foreach($args as $role) {
    permissions_create_role($role);
    drush_print(dt('Role !role created.', array('!role' => $role)));
  }
}

function drush_permissions_api_role_delete() {
  $args = func_get_args();
  foreach ($args as $role) {
    permissions_delete_role($role);
    drush_print(dt('Role !role deleted.', array('!role' => $role)));
  }
}
