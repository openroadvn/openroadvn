<?php
// $Id: subscriptions.install.inc,v 1.7 2009/09/03 00:25:16 salvis Exp $

/**
 * @file
 * Subscriptions module installation load-on-demand reminder message.
 */

/**
 * User information message at installation and pre-5.x-2.0 upgrade time.
 */
function _subscriptions_install_information() {
  $t = get_t();
  $tr = $t;
  drupal_set_message($t('Note: For standard Subscriptions functionality you need to enable the following modules:')
                     .'<ul><li>'. $tr('Subscriptions UI') .'</li><li>'. $tr('Subscriptions Mail ') .
                      $t('(requires !mail_edit for customizing the templates)',
                        array(
                         '!mail_edit' => l($tr('Mail Editor'), 'http://drupal.org/project/mail_edit', array('target' => '_blank')),
                        ))
                     .'</li><li>'. $tr('Content Subscriptions') .'</li><li>'. $tr('Taxonomy Subscriptions') .'</li></ul>', 'warning', FALSE);
}

