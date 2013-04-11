<?php
// $Id: active_translation.admin.inc,v 1.3 2008/06/13 19:48:52 drewish Exp $

/**
 * Settings form.
 */
function active_translation_settings_form() {
  $form['active_translation_show_status'] = array(
    '#type' => 'radios',
    '#title' => t('Show Translation Status Message'),
    '#options' => array(1 => t('Yes'), 0 => t('No')),
    '#default_value' => variable_get('active_translation_show_status', 1),
    '#description' => t('Setting this to yes will provide status information to visitors. The messages will indicate when the original node is being shown because a translation is not available '),
  );
  return system_settings_form($form);
}

/**
 * Rebuild translation table form.
 */
function active_translation_rebuild_form() {
  $form['help'] = array(
    '#type' => 'item',
    '#value' => t('If you are experiencing problems with missing translations you should try rebuilding the Active Translation table.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild'),
  );
  $form['#submit'] = array('active_translation_rebuild_on_submit');
  return $form;
}
