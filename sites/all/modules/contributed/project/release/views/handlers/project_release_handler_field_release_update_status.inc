<?php
// $Id: project_release_handler_field_release_update_status.inc,v 1.2 2009/11/30 23:34:25 dww Exp $

/**
 * Field handler to display the current update status of a release.
 */
class project_release_handler_field_release_update_status extends views_handler_field {
  function option_definition() {
    $options = parent::option_definition();
    $options['status_text_0'] = array('default' => t('Up to date'));
    $options['show_recommended_0'] = array('default' => FALSE);
    $options['show_icon_0'] = array('default' => FALSE);

    $options['status_text_1'] = array('default' => t('Update available'));
    $options['show_recommended_1'] = array('default' => TRUE);
    $options['show_icon_1'] = array('default' => TRUE);

    $options['status_text_2'] = array('default' => t('Not secure'));
    $options['show_recommended_2'] = array('default' => TRUE);
    $options['show_icon_2'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    $status_codes = project_release_update_status();
    foreach ($status_codes as $status => $name) {
      $form["status_text_$status"] = array(
        '#title' => t('Status text for %status', array('%status' => $name)),
        '#type' => 'textfield',
        '#size' => '30',
        '#default_value' => $this->options["status_text_$status"],
        '#description' => t('What text, if any, should be displayed in for the %status status.', array('%status' => $name)),
      );
      $form["show_recommended_$status"] = array(
        '#title' => t('Show recommended release for %status', array('%status' => $name)),
        '#type' => 'checkbox',
        '#default_value' => !empty($this->options["show_recommended_$status"]),
        '#description' => t('If checked, any release with the %status status will have the recommended release from the corresponding branch displayed.', array('%status' => $name)),
      );
      $form["show_icon_$status"] = array(
        '#title' => t('Show icon for %status', array('%status' => $name)),
        '#type' => 'checkbox',
        '#default_value' => !empty($this->options["show_icon_$status"]),
        '#description' => t('If checked, any release with the %status status will have a status icon displayed. See theme_project_release_update_status_icon() for details.', array('%status' => $name)),
      );
    }
  }

  /**
   * Always include {prn}.nid for the index in pre_render().
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['nid'] = 'nid';
  }

  /**
   * Conditionally find recommended releases based on update status.
   */
  function pre_render($values) {
    $this->items = array();
    foreach ($values as $result) {
      $status = $result->{$this->field_alias};
      $nid = $result->{$this->aliases['nid']};
      $this->items[$nid]['text'] = $this->options["status_text_$status"];
      if (!empty($this->options["show_recommended_$status"])) {
        $recommended_nids[] = $nid;
      }
      if (!empty($this->options["show_icon_$status"])) {
        $this->items[$nid]['icon'] = theme('project_release_update_status_icon', $status);
      }
    }

    // See if we need to query for any recommended releases, too.
    if (!empty($recommended_nids)) {
      $placeholders = db_placeholders($recommended_nids);
      $query = db_query("SELECT prn.nid, r_prn.nid as recommended_nid, r_prn.version as recommended_version FROM {project_release_nodes} prn INNER JOIN {project_release_supported_versions} prsv ON prsv.nid = prn.pid AND prsv.tid = prn.version_api_tid AND prsv.major = prn.version_major INNER JOIN {project_release_nodes} r_prn ON prsv.recommended_release = r_prn.nid WHERE prn.nid IN ($placeholders)", $recommended_nids);
      while ($release = db_fetch_object($query)) {
        $recommended_link = l($release->recommended_version, 'node/' . $release->recommended_nid);
        $this->items[$release->nid]['recommended'] = $recommended_link;
        // TODO: could make this another configuration knob.
        $this->items[$release->nid]['recommended-text'] = t('(Recommended: !recommended)', array('!recommended' => $recommended_link));
      }
    }
  }

  function render($values) {
    $output = '';
    $nid = $values->{$this->aliases['nid']};
    $item = array();
    if (!empty($this->items[$nid])) {
      $item = $this->items[$nid];
    }
    if (!empty($item['icon'])) {
      $output = $item['icon'];
    }
    if (!empty($item['text'])) {
      if (!empty($output)) {
        $output .= ' ';
      }
      $output .= $item['text'];
    }
    if (!empty($item['recommended'])) {
      if (!empty($output)) {
        $output .= ' ';
      }
      $output .= $item['recommended-text'];
    }
    return $output;
  }

/*
  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-text' . ']'] = t('The text to use for this status.');
    $tokens['[' . $this->options['id'] . '-icon' . ']'] = t('The icon to display for this status.');
    $tokens['[' . $this->options['id'] . '-recommended-version' . ']'] = t('The recommended release based on this status');
    $tokens['[' . $this->options['id'] . '-recommended-text' . ']'] = t('The test to display the recommended release based on this status');
  }

  function add_self_tokens(&$tokens, $item) {
    // TODO: This doesn't work, yet.
    $tokens['[' . $this->options['id'] . '-text' . ']'] = $item['text'];
    $tokens['[' . $this->options['id'] . '-icon' . ']'] = $item['icon'];

    $tokens['[' . $this->options['id'] . '-recommended-version' . ']'] = $item['recommended'];
    // TODO: could make this another configuration knob.
    $tokens['[' . $this->options['id'] . '-recommended-text' . ']'] = t('Recommended: !recommended', array('!recommended' => $item['recommended']));
  }
*/

}

