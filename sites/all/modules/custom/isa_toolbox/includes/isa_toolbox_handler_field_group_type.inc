<?php
class isa_toolbox_handler_field_group_type extends views_handler_field_custom {
  function query() {
  }

  function render($values) {
    $nid = $values->nid;
    if($values->node_node_data_field_project_common_using_nid) {
      $nid = $values->node_node_data_field_project_common_using_nid;
    }
    $node = node_load($nid);
    if ($node->group_type) {
      return $node->group_type;
    } else {
      $node = node_load(array_shift($node->og_groups));
      return $node->group_type;
    }
  }
}
?>