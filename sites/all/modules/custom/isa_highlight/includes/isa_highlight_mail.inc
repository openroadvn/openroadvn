<?php
/**
 * Dispatch function
 * Dispatches mails to their proper mail prep function, based on $type
 * @param string $type Mail target (facilitators or proposer)
 * @param string $status Add / Remove / Revoke / Accept / Reject
 * @param int $nid Highlighted item's NID
 * @param int $gid Community's NID
 */
function isa_highlight_prep_mail($type, $status, $nid, $gid) {
  $params = isa_highlight_get_tokens($nid, $gid);
  $type = strtolower($type);

  switch ($type) {
    case 'facilitators':
      isa_highlight_prep_mail_facilitators($status, $params, $gid);
      break;

    case 'proposer':
      isa_highlight_prep_mail_proposer($status, $params, $nid, $gid);
      break;
  }
}



/**
 * Sends mails concerning highlights to the proposer
 * @param string $status
 * @param array $params Array of parameters
 * @param int $nid Node ID
 * @param int $gid Community ID
 */
function isa_highlight_prep_mail_proposer($status, $params, $nid, $gid) {
  $highlight = isa_highlight_find($nid, $gid);
  $proposer = user_load($highlight->uid);
  $mail = $proposer->mail;

  $lang = user_preferred_language($proposer);
  $params['lang'] = $lang;
  $params['status'] = $highlight->status;
  $params['proposer'] = isa_highlight_get_username($proposer->uid);

  drupal_mail('isa_highlight', $status, $mail, $lang, $params);
}



/**
 * Sends mails concerning highlights to the facilitators
 * @param string $status Add / Remove / Revoke
 * @param array $params Array containing key - value pairs of tokens and their values
 */
function isa_highlight_prep_mail_facilitators($status, $params, $gid) {
  $facilitators = isa_highlight_get_facilitators($gid);

  // Prepare facilitator-specific parameters
  // And send mail to each facilitator
  foreach($facilitators as $fid) {
    $facilitator = user_load($fid);
    $params['facilitator'] = isa_highlight_get_username($fid);
    $mail = $facilitator->mail;

    // Get the language for the mail. Also set it as parameter,
    // as it'll be used in t() to translate the mail properly.
    $lang = user_preferred_language($facilitator);
    $params['lang'] = $lang;
    $params['status'] = $status;
    drupal_mail('isa_highlight', $status, $mail, $lang, $params);
  }
}



/**
 * Returns the same username as isa_icp_username
 * Only difference is that username here isn't returned as a link (useful for mails)
 * If this can be integrated into isa_icp_username, this function can be removed
 *
 * Is called by isa_highlight_prep_mail_facilitators (see right above)
 *
 * @param int $uid User ID
 * @return string Username
 */
function isa_highlight_get_username($uid) {
  $profile = content_profile_load( 'profile', $uid );
  $username = $profile->field_firstname[0]['value'] . ' ' . strtoupper($profile->field_lastname[0]['value']);
  return $username;
}



/**
 * Implementation of hook_mail
 */
function isa_highlight_mail($key, &$message, $params) {
  // Prepare the tokens
  $tokens = array(
              '!proposer' => $params['proposer'],
              '!type' => $params['type'],
              '!title' => $params['title'],
              '!node_link' => $params['node_link'],
              '!community' => $params['community'],
              '!community_link' => $params['community_link'],
              '!facilitator' => $params['facilitator'],
              '!status' => $params['status'],
            );
  // Prepare the name of the variable to get
  $key = strtolower($key);
  $subject = 'isa_highlight_' . $key . '_subject';
  $body = 'isa_highlight_' . $key . '_body';
  // Get subject and body from the variables table
  // And translate it, replacing all tokens with their proper values along the way
  $subject = variable_get($subject, '');
  $message['subject'] = t($subject, $tokens);
  $body = variable_get($body, '');
  $message['body'] = t($body, $tokens);
  // Set headers to allow HTML tags (useful for <p>, <br />, etc)
  $message['headers']['Content-Type'] = 'text/html; charset=UTF-8; format=flowed';
}



function isa_highlight_get_tokens($nid, $gid) {
  global $user;
  global $base_url;

  $node = node_load($nid);
  $community = node_load($gid);

  // Prepare parameters (used to replace tokens)
  $username = isa_highlight_get_username($user->uid);
  $params['proposer'] = $username;
  $params['type'] = $node->type;
  $params['title'] = $node->title;
  $params['node_link'] = $base_url . '/' . $node->type . '/' . $node->nid;
  $params['community'] = $community->title;
  $params['community_link'] = $base_url . '/community/' . $community->nid;

  return $params;
}



function isa_highlight_mails_form($node, $form_state) {
  $form['info'] = array(
    '#type' => 'item',
    '#value' => t('Usable tokens:
                              <ul>
                                <li>!proposer => name of person who proposed the action</li>
                                <li>!type => Type of the highlighted node</li>
                                <li>!title => Title of the highlighted node</li>
                                <li>!node_link => Link to the highlighted node</li>
                                <li>!community => Title of the community</li>
                                <li>!community_link => Link to the community</li>
                                <li>!facilitator => name of the facilitator receiving the mail</li>
                                <li>!status => Status of the highlight (Add, Remove or Approved)</li>
                              </ul>'),
  );

  $form['add'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Propose adding a highlight'),
  );

  $form['remove'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#title' => t('Propose removing a highlight'),
  );



  $form['add']['add_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail subject'),
    '#default_value' => variable_get('isa_highlight_add_subject', ''),
  );

  $form['remove']['remove_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Mail subject'),
    '#default_value' => variable_get('isa_highlight_remove_subject', ''),
  );



  $form['add']['add_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail body'),
    '#default_value' => variable_get('isa_highlight_add_body', ''),
  );

  $form['remove']['remove_body'] = array(
    '#type' => 'textarea',
    '#title' => t('Mail body'),
    '#default_value' => variable_get('isa_highlight_remove_body', ''),
  );

  return system_settings_form($form);
}



function isa_highlight_mails_form_submit(&$form, $form_state) {
  $add_subject = $form_state['values']['add_subject'];
  $add_body = $form_state['values']['add_body'];

  $remove_subject = $form_state['values']['remove_subject'];
  $remove_body = $form_state['values']['remove_body'];

  variable_set('isa_highlight_add_subject', $add_subject);
  variable_set('isa_highlight_add_body', $add_body);

  variable_set('isa_highlight_remove_subject', $remove_subject);
  variable_set('isa_highlight_remove_body', $remove_body);

  drupal_set_message(t('The changes have been saved'));
}