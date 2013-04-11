<?php
function isa_federated_forges_get_mail_recipients() {
  $roles = user_roles(TRUE, RECEIVE_FORGES_MAIL);
  $users = array();

  $users_added = array();

  $sql = 'SELECT uid FROM {users_roles} ur WHERE ur.rid = %d';
  foreach($roles as $rid => $role) {
    $query = db_query($sql, $rid);
    while($uid = db_result($query)) {
      if(!in_array($uid, $users_added)) { // Avoid doubles
        $user = user_load($uid);
        $users[] = $user;
        $users_added[] = $uid;
      }
    }
  }

  return $users;
}



/**
 * Implementation of hook_form().
 * Allows users to define form subject + text
 */
function isa_federated_forges_mail_form($form_state) {
  isa_federated_forges_get_mail_recipients();
  $form['info'] = array(
    '#type' => 'item',
    '#value' => t('Usable tokens:
                              <ul>
                                <li>!added => List of all created projects, per forge</li>
                                <li>!changed => List of all updated projects, per forge</li>
                                <li>!deleted => List of all deleted projects, per forge</li>
                                <li>---</li>
                                <li>!username => Name of the recipient</li>
                              </ul>'),
  );

  $form['subject'] = array (
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => variable_get('federated_forges_mail_title', ''),
  );

  $form['body'] = array (
    '#type' => 'textarea',
    '#title' => t('Mail'),
    '#default_value' => variable_get('federated_forges_mail_body', ''),
  );

  $form['submit'] = array (
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}



/**
 * Implementation of hook_form_submit().
 * Saves mail settings
 */
function isa_federated_forges_mail_form_submit(&$form, $form_state) {
  $subject = $form_state['values']['subject'];
  $body = $form_state['values']['body'];

  variable_set('federated_forges_mail_title', $subject);
  variable_set('federated_forges_mail_body', $body);

  drupal_set_message(t('The configuration has been saved.'));
}



/**
 * Sends a report of created / updated / deleted forges to each person with the correct permissions
 */
function isa_federated_forges_send_mails($params) {
  $users = isa_federated_forges_get_mail_recipients();
  $params = isa_federated_forges_prepare_mail($params);

  foreach($users as $user) {
    $name = strip_tags(theme('username',$user->uid));
    $params['body'] = str_replace('!username', $name, $params['body']);
    
    drupal_mail('isa_federated_forges', 'report', $user->mail, 'en', $params);
  }
}



/**
 * Implementation of hook_mail().
 */
function isa_federated_forges_mail($key, &$message, $params) {
  $message['subject'] = token_replace($params['subject']);
  $params['body'] = token_replace($params['body']);
  
  
  $message['body'] = "<html>\n" .
    "<head>\n" .
    "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n" .
    "</head>\n" .
    "<body>" . $params['body'] . "</body>\n" .
    "</html>\n";

  $message['headers'] = array(
    'MIME-Version' => '1.0',
    'Content-Type' => 'text/html; charset=UTF-8;',
    'Content-Transfer-Encoding' => '8Bit',
    'X-Mailer' => 'Drupal',
    'From' => variable_get('site_mail', ini_get('sendmail_from')),
  );
}



/**
 * Prepares the $params array for sending the report mail
 * @return array $params Array containing the mail subject and body
 */
function isa_federated_forges_prepare_mail($params) {
  $subject = variable_get('federated_forges_mail_title', '');
  $body = variable_get('federated_forges_mail_body', '');
  isa_federated_forges_create_list('added', $params, $body);
  isa_federated_forges_create_list('changed', $params, $body);
  isa_federated_forges_create_list('deleted', $params, $body);
 
  $params['subject'] = $subject;
  $params['body'] = $body;
  return $params;
}



/**
 * Turns the array containing projects into a HTML list
 * Then replaces the corresponding placeholder in the mail's body's text with this list
 * @param string $op String containing which op to get (added, changed, deleted)
 * @param array $projects The global projects array
 * @param string $body String that serves for the mail's main text
 */
function isa_federated_forges_create_list($op, $projects, &$body) {
  global $base_url;
  $poss_ops = array('added', 'changed', 'deleted');

  if (in_array(strtolower($op), $poss_ops)) {
    if (count($projects[$op]) > 0) {
      $list = '';
      foreach ($projects[$op] as $ff_title => $ff) {
        $federated_forge = new federated_forge;
        $federated_forge->load_from_title($ff_title);
        $list .= l( $ff_title , 'node/'.$federated_forge->get_nid() , array('absolute' => TRUE))  . ':<br /><ul>';
        foreach ($ff as $fp) {
          if ($op != 'deleted'){
            $list .= '<li>' . l($fp->title, 'node/' . $fp->nid, array('absolute' => TRUE)) . '</li>';
          }else{
            $list .= '<li>' . $fp->title . '</li>';
          }
          
        }
        $list .= '</ul>';
      }
    } else {
      $list = t('No projects have been ' . strtolower($op));
    }
    $search = '!' . $op;
    $body = str_replace($search, $list, $body);
  }
}