<?php
// $Id: project_release_handler_field_release_file_timestamp.inc,v 1.2 2009/12/08 17:24:14 dww Exp $

class project_release_handler_field_release_file_timestamp extends views_handler_field_date {
  function option_definition() {
    $options = parent::option_definition();
    $options['file_order'] = array('default' => 'MAX');
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $form['file_order'] = array(
      '#title' => t('File timestamp order'),
      '#description' => t('Should the maximum (newest) or minimum (oldest) timestamp be used?'),
      '#type' => 'select',
      '#options' => array(
        'MAX' => t('Newest file'),
        'MIN' => t('Oldest file'),
      ),
      '#default_value' => $this->options['file_order'],
    );
  }

  /**
   * Find the aggregate file timestamp data for each release in the result.
   */
  function pre_render($values) {
    $this->items = array();
    $nids = array();
    foreach ($values as $result) {
      $nids[] = $result->{$this->field_alias};
    }
    if (!empty($nids)) {
      $function = $this->options['file_order'];
      $placeholders = db_placeholders($nids);
      $sql = "SELECT $function(f.timestamp) as timestamp, prf.nid FROM {project_release_file} prf INNER JOIN {files} f ON prf.fid = f.fid WHERE prf.nid IN ($placeholders) GROUP BY prf.nid";
      $query = db_query($sql, $nids);
      while ($file = db_fetch_object($query)) {
        $this->items[$file->nid] = $file->timestamp;
      }
    }
  }

  function render($values) {
    $nid = $values->{$this->field_alias};
    if (!empty($this->items[$nid])) {
      // Setup the timestamp that views_handler_field_date::render() expects.
      $values->{$this->field_alias} = $this->items[$nid];
    }
    else {
      $values->{$this->field_alias} = 0;
    }
    return parent::render($values);
  }

}

