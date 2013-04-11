<?php
// $Id: subscriptions_mail.cron.inc,v 1.11 2010/12/27 16:22:49 salvis Exp $

/**
 * @file
 * Subscriptions module mail gateway (cron functions).
 */

/**
 * Implementation of hook_cron().
 *
 * Takes items from {subscriptions_queue} and generates notification emails.
 */
function _subscriptions_mail_cron() {

  // set_time_limit(3600);  drupal_set_message('DON\'T FORGET TO REMOVE THE TIME LIMIT EXTENSION!!!');

  global $user, $language;
  drupal_init_language();
  include_once drupal_get_path('module', 'subscriptions_mail') .'/subscriptions_mail.templates.inc';

  $mails_allowed = variable_get('subscriptions_number_of_mails', 0);
  $from = _subscriptions_mail_site_mail();
  $old_uid = 0;
  $single_count = 0;
  $digest_count = 0;
  $loaded_objects = array();
  $fields = array();

  // Strategy for cron:
  // Use a defined percentage of the total cron time (default: 50%), but leave at least 5s.
  $total_seconds = ini_get('max_execution_time');
  if (empty($total_seconds)) {
    // Work-around for D6.14, which broke this in #193383-106 with http://cvs.drupal.org/viewvc.py/drupal/drupal/includes/common.inc?r1=1.756.2.65&r2=1.756.2.66&pathrev=DRUPAL-6
    // D6.15 will revert this to a hard-coded 240 in http://cvs.drupal.org/viewvc.py/drupal/drupal/includes/common.inc?r1=1.756.2.70&r2=1.756.2.71&pathrev=DRUPAL-6
    $total_seconds = 240;
  }
  $lost_seconds = timer_read('page')/1000;
  $available_seconds = $total_seconds - $lost_seconds;
  $usable_seconds = min(array($available_seconds - 5, $total_seconds*subscriptions_mail_get_cron_percentage()/100));
  //TEST: watchdog('cron', "Subscriptions has $available_seconds of $total_seconds seconds available.");
  if ($usable_seconds <= 0) {
    $variables = array(
      '!Module' => 'Subscriptions',
      '@link'   => url('admin/settings/subscriptions', array('fragment' => 'edit-subscriptions-cron-percent')),
    );
    $watchdog = 'watchdog';  // keep potx from translating 'cron'
    if ($usable_seconds == 0) {
      $watchdog('cron', t('!Module cannot send any notifications because its <a href="@link">cron job time</a> is 0!', $variables), NULL, WATCHDOG_WARNING);
    }
    else {
      $watchdog('cron', t('!Module cannot send any notifications because the cron run has less than 5 available seconds left!', $variables), NULL, WATCHDOG_WARNING);
    }
  }

  while ((empty($mails_allowed) || $single_count + $digest_count < $mails_allowed)
          && timer_read('page')/1000 < $lost_seconds + $usable_seconds) {
    $result = db_query_range('SELECT * FROM {subscriptions_queue} WHERE suspended = 0 AND last_sent + send_interval < %d ORDER BY sqid', time(), 0, 1);
    if (!($s = db_fetch_array($result))) {
      break;  // No more ready queue items, terminate loop.
    }

    $saved_user = $user;
    $saved_language = $language;
    session_save_session(FALSE);
    $user = subscriptions_user_load($s['uid']);
    drupal_init_language();
    $langcode = $language->language;
    $digest_data = array();

    do {  // once and repeat while adding to a digest
      if ($user->status && $user->access) {
        $s['mail'] = $user->mail;
        $cids = array();
        $load_function = $s['load_function'];
        $index = $load_args = $s['load_args'];
        if (!isset($loaded_objects[$user->uid][$load_function][$load_args])) {
          if (is_numeric($load_args)) {
            $object = $load_function($load_args, $s['sqid'], $s['is_new']);
          }
          else {
            $load_args = unserialize($load_args);
            $load_args[] = $s['is_new'];
            $object = call_user_func_array($load_function, $load_args);
          }
          if (!empty($object)) {
            $access = module_invoke_all('subscriptions', 'access', $load_function, $load_args, $object);
            // One FALSE vote is enough to deny. Also, we need a non-empty array.
            $allow = !empty($access) && array_search(FALSE, $access) === FALSE;
            $loaded_objects[$user->uid][$load_function][$index] = $allow ? $object : FALSE;
          }
        }
        if ($object = $loaded_objects[$user->uid][$load_function][$index]) {
          $sender = subscriptions_user_load($object->uid);
          $module = $s['module'];
          $ori_field = $field = $s['field'];
          $ori_value = $value = $s['value'];
          if (!isset($fields[$langcode][$module])) {
            $fields[$langcode][$module] = module_invoke_all('subscriptions', 'fields', $module);
          }
          if ($module == 'node' && $field == 'nid' && (!empty($object->_subscriptions_is_updated) || !empty($object->_subscriptions_is_new)) && user_access('subscribe to content types', $user)) {
            $unlisteds = variable_get('subscriptions_unlisted_content_types', array());
            if (isset($object->type) && !in_array($object->type, $unlisteds)) {
              // Convert node-nid to node-type because that can give more specific information.
              $field = 'type';
              $value = $object->type;
            }
          }
          $mailvars_function = $fields[$langcode][$module][$field]['mailvars_function'];
          $mailkey = $fields[$langcode][$module][$field]['mailkey'];
          if (!is_numeric($value)) {
            $mailkey .= '-'. $value;
          }
          $digest = $s['digest'] > 0 || $s['digest'] == -1 && _subscriptions_get_setting('digest', 0) > 0;
          if ($digest) {
            $body_template = subscriptions_mail_template_load(SUBSCRIPTIONS_DIGEST_MAILKEY .'+item', $langcode, 'body', 'DITEM');
          }
          else {
            $body_template = subscriptions_mail_template_load($mailkey, $langcode, 'body', 'BODY');
            $subject_template = subscriptions_mail_template_load($mailkey, $langcode, 'subject', 'SUBJ');
          }
          init_theme();
          $show_node_info = (isset($object->type) ? theme_get_setting('toggle_node_info_'. $object->type) : TRUE);
          $base = 'user/'. $s['uid'];
          $s['name'] = strip_tags(theme('username',$s['uid'])); //GKN
          $sender_name = strip_tags(theme('username',$sender->uid)); //GKN
          $mailvars = array(
            '!site' => variable_get('site_name', 'drupal'),
            '!recipient_name' => $s['name'],
            '!recipient_page' => url($base, array('absolute' => TRUE)),
            '!recipient_uid' => $s['uid'],
            '!sender_name' => ($show_node_info ? $sender_name/*($sender->uid ? $sender->name : variable_get('anonymous', '!sender_name'))*/ : '!sender_name'),
            '!sender_page' => ($show_node_info && $sender->uid ? url("user/$sender->uid", array('absolute' => TRUE)) : '!sender_page'),
            '!sender_contact_page' => ($show_node_info ? (empty($sender->contact) ? t('(disabled)') : url("user/$sender->uid/contact", array('absolute' => TRUE))) : '!sender_contact_page'),
            '!sender_has_contact_page' => ($show_node_info ? (empty($sender->contact) ? 0 : 1) : 0),
            '!manage_url' => url($base .'/subscriptions', array('absolute' => TRUE)),
            '!name' => $s['name'],
            '!subs_type' => $fields[$langcode][$module][$field]['!subs_type'],
            '!unsubscribe_url' => url("s/del/$module/$ori_field/$ori_value/". $s['author_uid'] .'/'. $s['uid'] .'/'. md5(drupal_get_private_key() . $module . $ori_field . $ori_value . $s['author_uid'] . $s['uid']), array('absolute' => TRUE)),
          );
          if (function_exists('realname_make_name')) {
            $recipient = (object) array('uid' => $s['uid']);
            $mailvars += array(
              '!recipient_realname' => realname_make_name($recipient),
              '!sender_realname' => realname_make_name($sender),
            );
          }
          $mailvars_function($mailvars, $object, $field, $s);
          $mailvars += module_invoke_all('subscriptions_get_mailvars', $object);
          if ($digest && !empty($object->_subscriptions_comments) && module_exists('subscriptions_content')) {
            $digest_comment_template = subscriptions_mail_template_load(SUBSCRIPTIONS_DIGEST_MAILKEY .'+comment', $langcode, 'body', 'DITEMCMT');
            $mailvars['!comments'] = _subscriptions_content_format_comments($object, $digest_comment_template, '');
          }
          $body = strtr(subscriptions_mail_template_preprocess($body_template, $mailvars), $mailvars);
          if (variable_get('subscriptions_showmailkeys', 0)) {
            $body .= t('| Mailkey: @mailkey/@langcode', array('@mailkey' => $mailkey, '@langcode' => $langcode)) ."\n";
          }
          if ($digest) {
            $digest_data['bodies'][] = $body;
            $digest_data['send'] = array(
              'name' => $s['name'],
              'mail' => $s['mail'],
              'from' => $from,
              '!name' => $mailvars['!name'],
              '!manage_url' => $mailvars['!manage_url'],
            );
            $digest_data['send_intervals'][$s['send_interval']] = $s['send_interval'];
          }
          else {
            $subject = strtr(subscriptions_mail_template_preprocess($subject_template, $mailvars), $mailvars);
            _subscriptions_mail_send('passthru', $s['name'], $s['mail'], $subject, $body, $from, $s['uid'], array($s['send_interval']));
            ++$single_count;
          }
        }
      }
      db_query("DELETE FROM {subscriptions_queue} WHERE load_function = '%s' AND load_args = '%s' AND uid = %d", $s['load_function'], $s['load_args'], $s['uid']);

      if ($digest) {
        // Get next ready queue item for this user.
        $result = db_query_range('SELECT * FROM {subscriptions_queue} WHERE uid = %d AND last_sent + send_interval < %d ORDER BY sqid', $user->uid, time(), 0, 1);
        if (!($s = db_fetch_array($result))) {
          // No more ready queue items for this user, finish off this digest.
          $s = $digest_data['send'];
          $subject_template = subscriptions_mail_template_load(SUBSCRIPTIONS_DIGEST_MAILKEY, $langcode, 'subject', 'DSUBJ');
          $body_template = subscriptions_mail_template_load(SUBSCRIPTIONS_DIGEST_MAILKEY, $langcode, 'body', 'DBODY');
          $separator = subscriptions_mail_template_load(SUBSCRIPTIONS_DIGEST_MAILKEY .'+item', $langcode, 'subject', 'SEP');
          $mailvars['!bodies'] = implode($separator, $digest_data['bodies']);
          $mailvars['!name'] = $s['!name'];
          $mailvars['!manage_url'] = $s['!manage_url'];
          $digest_data['mailvars'] = $mailvars;
          foreach (module_implements('subscriptions_digest_alter') as $module) {
            $function = $module .'_subscriptions_digest_alter';
            $function($digest_data);
            if (empty($digest_data)) {
              break;  // forget it
            }
          }
          if (!empty($digest_data)) {
            $mailvars = $digest_data['mailvars'];
            $subject = strtr(subscriptions_mail_template_preprocess($subject_template, $mailvars), $mailvars);
            $body = strtr(subscriptions_mail_template_preprocess($body_template, $mailvars), $mailvars);
            _subscriptions_mail_send(SUBSCRIPTIONS_DIGEST_MAILKEY, $s['name'], $s['mail'], $subject, $body, $s['from'], $user->uid, $digest_data['send_intervals']);
            ++$digest_count;
          }
          $digest = false;
        }
      }
    } while ( $digest );

    $user = $saved_user;
    $language = $saved_language;
    session_save_session(TRUE);
  }

  if ($single_count + $digest_count > 0) {
    $current_seconds = timer_read('page')/1000;
    $variables = array(
      '!Module'            => 'Subscriptions',
      '!single_count'      => $single_count,
      '!digest_count'      => $digest_count,
      '!used_seconds'      => round($current_seconds - $lost_seconds),
      '!total_seconds'     => $total_seconds,
      '!remaining_items'   => db_result(db_query("SELECT COUNT(*) FROM {subscriptions_queue} WHERE last_sent + send_interval < %d AND suspended = 0", time())),
      '!suspended_items'   => db_result(db_query("SELECT COUNT(*) FROM {subscriptions_queue} WHERE last_sent + send_interval < %d AND suspended <> 0", time())),
      '!remaining_seconds' => round($total_seconds - $current_seconds),
      '%varname'           => 'subscriptions_mail_trash_silently',
      '!cron'              => 'cron',
    );
    if (variable_get('subscriptions_mail_trash_silently', 0)) {
      $message = t('!Module DISCARDED !single_count single and !digest_count digest notifications in !used_seconds of !total_seconds seconds, due to the %varname variable being set.', $variables);
    }
    else {
      $message = t('!Module sent !single_count single and !digest_count digest notifications in !used_seconds of !total_seconds seconds.', $variables);
    }
    $message .= ' '. t('!remaining_items queue items remaining (plus !suspended_items suspended), !remaining_seconds seconds left for other @cron client modules.', $variables);
    $watchdog = 'watchdog'; // keep potx from translating 'cron'
    $watchdog('cron', $message);
  }
}

/**
 * Implementation of hook_mail().
 */
function subscriptions_mail_mail($key, &$message, $params) {
  global $base_url;

  $url = parse_url($base_url);
  $list_id = variable_get('site_name', '') .' '. t('Subscriptions') .' <subscriptions.'. $url['host'] .'>';

  $message['subject'] = $params['context']['subject'];
  $message['body'] = $params['context']['message'];
  $message['headers']['List-Id'] = $list_id;
}


/**
 * Send the notification by mail.
 */
function _subscriptions_mail_send($mailkey, $name, $to, $subject, $body, $from, $uid, $send_intervals) {
  global $user;

  if (variable_get('subscriptions_mail_trash_silently', 0)) {
    // Block notification mail; useful for staging and development servers.
    return;
  }

  $mail_success = drupal_mail('subscriptions_mail', $mailkey, $to, user_preferred_language($user), array(
    'account' => $user,
    'object' => NULL,
    'context' => array(
      'recipient' => $to,
      'subject' => $subject,
      'message' => $body,
    ),
  ), $from, TRUE);

  $watchdog_params = array('@name' => $name, '@to' => "<$to>");
  if (!empty($mail_success['result'])) {
    if (variable_get('subscriptions_watchgood', 1)) {
      watchdog('subscriptions', 'notification for @name at @to', $watchdog_params);
    }
    foreach ($send_intervals as $send_interval) {
      db_query("UPDATE {subscriptions_last_sent} SET last_sent = %d WHERE uid = %d AND send_interval = %d", time(), $uid, $send_interval);
      if (!db_affected_rows()) {
        @db_query("INSERT INTO {subscriptions_last_sent} (uid, send_interval, last_sent) VALUES(%d, %d, %d)", $uid, $send_interval, time());
      }
    }
  }
  else {
    watchdog('subscriptions', 'error mailing notification for @name at @to', $watchdog_params, WATCHDOG_ERROR);
  }
}

/**
 * Preprocess a mail template (subject or body), detecting conditional clauses
 * that conform to a prescribed syntax
 *
 * @param string $template
 *  the template for preprocessing
 * @param array $mailvars
 *  an associatvie array of currently existing variables that are to be
 *  interpolated into the template later , and which can be used by this
 *  function for preprocessing
 *
 * This function allows the administrator to specify ternary-type conditions
 * to determine what text is used in a mail in a particular situation, using
 * the variables that are currently available for that mail for reference.
 * The syntax is standard PHP/C-style ternary syntax, but only allows the
 * "==" and "!=":
 * {{!variable_name==sometext?text for true condition:text for false condition}}
 *
 * sometext must not contain a question mark, and the true_text no colon.
 * true_text and false_text may contain newlines as well as a nested
 * conditional expression (one level of recursion).
 */
function subscriptions_mail_template_preprocess($template, $mailvars) {
  preg_match_all('/{{(?P<condition>[^?]+?)\?(?P<true>({{(.|\n)*?}})|([^:]*?)):(?P<false>({{(.|\n)*?}})|((.|\n)*?))}}/', $template, $conditions);

  // locate the actual operators/operand for each
  $replacement = '';
  foreach ($conditions[0] as $k => $v) {
    preg_match('/(?P<operand_1>!.+)\s*(?P<operator>==|!=)\s*(?P<operand_2>.+)/', $conditions['condition'][$k], $matches);
    $operand1 = (isset($mailvars[$matches['operand_1']]) ? $mailvars[$matches['operand_1']] : $matches['operand_1']);
    if ($matches['operator'] == '==') {
      $replacement = ($operand1 == $matches['operand_2']) ? $conditions['true'][$k] : $conditions['false'][$k];
    }
    elseif ($matches['operator'] == '!=') {
      $replacement = ($operand1 != $matches['operand_2']) ? $conditions['true'][$k] : $conditions['false'][$k];
    }
    else {
      continue;
    }
    // replace the condition with the result of its evalutation
    $template = str_replace($v, $replacement, $template);
  }
  return (empty($conditions[0]) ? $template : subscriptions_mail_template_preprocess($template, $mailvars));
}

/**
 * Implementation of hook_mail_alter().
 *
 * Remove any trailing spaces (must run after mail_edit_mail_alter()!).
 */
function subscriptions_mail_mail_alter(&$message) {
  $message['body'] = preg_replace('/ +(\r?\n)/', '\\1', $message['body']);
}

