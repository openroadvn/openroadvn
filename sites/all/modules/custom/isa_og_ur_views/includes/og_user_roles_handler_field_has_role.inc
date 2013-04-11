<?php
/**
 * This class provides a Views 2 field that ...
 */
class og_user_roles_handler_field_has_role extends views_handler_field_boolean {
  /**
   * Modify the query to add the necessary field(s)
   */
  function query() {
    $role_id = $this->options['og_role_id'];
    if (!is_numeric($role_id)) {
      return;
    }
    $field_name = sprintf('has_role_%d', $role_id);
    $field_query = sprintf('(select count(rid) from {og_users_roles} ogur WHERE ogur.uid = users.uid AND ogur.gid = og_uid.nid AND ogur.rid = %d)', $role_id);
    $this->query->add_field(NULL, $field_query, $field_name);
    $this->field_alias = $field_name;
  }
  
  /**
   * Handle the rendering of the field
   */
  function render($values) {
    $value = $values->{$this->field_alias};
    if ($value > 0) {
      return (strlen($this->options['yes_value']) ? $this->options['yes_value'] : t('Yes'));
    }
    else {
      return (strlen($this->options['no_value']) ? $this->options['no_value'] : t('No'));
    }
  }

  /**
   * @return the summary of the current configuration of this field when editing
   * the parent View.
   */
  function admin_summary() {
    $role_id = $this->options['og_role_id'];
    if (!is_numeric($role_id)) return (parent::admin_summary());

    $role_name = db_result(db_query('SELECT name FROM {role} WHERE rid = %d', $role_id));
    return ($role_name . ' ' . parent::admin_summary());
  }
  
  /**
   * @return a form that allow the user to choose the format of the field
   */
  function options_form(&$form, &$form_state) {
    // roles list
    $roles = array();
    $roles_res = db_query('SELECT rid, name FROM {role}');
    while ($role = db_fetch_object($roles_res)) {
      $roles[$role->rid] = $role->name;
    }
    $form['og_role_id'] = array(
      '#type' => 'select',
      '#title' => t('Role'),
      '#default_value' => $this->options['og_role_id'],
      '#options' => $roles,
    );
    $form['yes_value'] = array(
      '#type' => 'textfield',
      '#title' => t('Yes value'),
      '#description' => t('This value will be displayed instead of "Yes" if the user belongs to the given role within the group'),
      '#default_value' => $this->options['yes_value'],
    );
    $form['no_value'] = array(
      '#type' => 'textfield',
      '#title' => t('No value'),
      '#description' => t('This value will be displayed instead of "No" if the user does not belong to the given role within the group'),
      '#default_value' => $this->options['no_value'],
    );
    parent::options_form($form, $form_state);
    return ($form);
  }
};
