<?php
// $Id: login_security.menu.inc,v 1.1.2.1 2009/09/21 14:10:08 ilo Exp $

/**
 * @file
 * login_security.menu.inc
 *
 * Declare the menu entries for this module.
 */

/**
 * Implementation of hook_menu().
 */
function _login_security_menu() {
  return array(
    // Administer >> Site configuration >> Login Security settings
    'admin/settings/login_security' => array(
      'title' => 'Login Security',
      'description' => 'Configure security settings in the login form submission.',
      'access arguments' => array('administer site configuration'),
      'page callback' => 'drupal_get_form',
      'page arguments' => array('login_security_admin_settings'),
      'file' => 'login_security.admin.inc',
    ),
  );
}
