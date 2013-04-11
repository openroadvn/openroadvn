<?php
// $Id: docs.php,v 1.1 2010/02/03 21:01:16 salvis Exp $
/**
 * @file
 * This file contains no working PHP code; it exists to provide additional documentation
 * for doxygen as well as to document hooks in the standard Drupal manner.
 */

/**
 * @mainpage Subscriptions API Manual
 *
 * This is preliminary documentation and needs further work.
 *
 * Topics:
 * - @ref subscriptions_hooks
 */

/**
 * @defgroup subscriptions_hooks Subscriptions' hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend 
 * Subscriptions.
 */

/**
 * Provide a list of token names and descriptions for Mail Editor to display.
 *
 * The parameters are passed straight through from Mail Editor's
 *   hook_mail_edit_tokens_list($mailkey, $options = array())
 */
function hook_subscriptions_tokens_list($mailkey, $options = array()) {
  $tokens = array();
  switch ($mailkey) {
    case 'digest':
      // For digests:
      $tokens += array(
        '!digest_count' => t('The number of notifications in the digest.'),
      );
      break;
    default:
      // For all other Subscriptions mailkeys:
      $tokens += array(
        '!nid' => t('The NID of the node.'),
      );
  }
  return $tokens;
}

/**
 * Provide a list of token names and actual values for building notifications
 * for a given object.
 */
function hook_subscriptions_get_mailvars($object) {
  $mailvars = array();
  // Check whether it's a node!
  if (isset($node->nid)) {
    $mailvars['!myvariable'] = $node->nid;
  }
  return $mailvars;
}

/**
 * This alter hook is called immediately before sending of a digest.
 *
 * You get the digest bodies as well as the prepared mailvars just before they
 * are merged into a digest, and you can do with them whatever you like. 
 * At the very extreme, you can remove digest items or even set $digest_data
 * to NULL to suppress sending this digest.
 */
function hook_subscriptions_digest_alter(&$digest_data) {
  $digest_data['mailvars']['!digest_count'] = count($digest_data['bodies']);
}

/**
 * @}
 */
