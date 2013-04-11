<?php
// $Id: project_release_handler_field_release_file_name.inc,v 1.1 2009/11/30 09:40:25 dww Exp $

/**
 * Field handler for release files that honors project_release_download_link.
 */
class project_release_handler_field_release_file_name extends views_handler_field_file {

  /**
   * Conditionally provide additional fields to add.
   */
  function init(&$view, &$options) {
    parent::init($view, $options);
    $this->additional_fields['filename'] = array('table' => 'files', 'field' => 'filename');
    if (!empty($options['link_to_file'])) {
      $this->additional_fields['filepath'] = array('table' => 'files', 'field' => 'filepath');
    }
  }

  /**
   * Render whatever the data is as a link to the file.
   *
   * Data should be made XSS safe prior to calling this function.
   * This honors theme_project_release_download_link() which supports the
   * 'project_release_download_base' setting.
   */
  function render_link($data, $values) {
    if (!empty($this->options['link_to_file']) && $data !== NULL && $data !== '') {
      $this->options['alter']['make_link'] = TRUE;
      $filepath = $values->{$this->aliases['filepath']};
      $link = theme('project_release_download_link', $filepath, NULL, TRUE);
      $this->options['alter']['path'] = $link['href'];
    }
    return check_plain($values->{$this->aliases['filename']});
  }

}

