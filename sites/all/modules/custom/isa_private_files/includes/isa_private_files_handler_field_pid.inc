<?php
class isa_private_files_handler_field_pid extends views_handler_field_numeric {
  function query() {
    $table = $this->query->ensure_table('isa_download_statistics_count');
    $sql = "SELECT IFNULL(sum(count),0) FROM {isa_download_statistics_count}
            WHERE pid = node.nid";
    $this->query->add_field('', "($sql)", 'isa_download_statistics_project_downloads_count');
    $this->field_alias = 'isa_download_statistics_project_downloads_count';
  }

  function render($values) {
    $txt = $values->isa_download_statistics_project_downloads_count;
    if ($txt == 0) {
      $value = $txt . ' download';
    }
    else {
      $value = $txt . format_plural($txt, ' download', ' downloads');
    }
    return check_plain($this->options['prefix']) . $value . check_plain($this->options['suffix']);
  }
}
?>