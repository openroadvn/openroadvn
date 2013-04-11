<?php
class isa_toolbox_handler_field_group_short_name extends views_handler_field_custom {
  function query() {
    // select communities short name
    $sql = "SELECT ctc.field_community_short_name_value
            FROM {content_type_community} ctc
            WHERE ctc.nid = node.nid";
    $this->query->add_field('', "($sql)", 'isa_toolbox_community_short_name');
    $this->field_alias = 'isa_toolbox_community_short_name';
    //select community short name of a content
    $sql = "SELECT ctc.field_community_short_name_value
            FROM {content_type_community} ctc,{og_ancestry} oga
            WHERE oga.nid = node.nid
            AND ctc.nid = oga.group_nid";
    $this->query->add_field('', "($sql)", 'isa_toolbox_parent_community_short_name');
    $this->field_alias = 'isa_toolbox_parent_community_short_name';
    // select projects short name
    $sql = "SELECT pp.uri
            FROM {project_projects} pp
            WHERE pp.nid = node.nid";
    $this->query->add_field('', "($sql)", 'isa_toolbox_project_short_name');
    $this->field_alias = 'isa_toolbox_project_short_name';
     // select project short name of a content (don't work for issues and releases)
    $sql = "SELECT pp.uri
            FROM {project_projects} pp,{og_ancestry} oga
            WHERE oga.nid = node.nid
            AND pp.nid = oga.group_nid";
    $this->query->add_field('', "($sql)", 'isa_toolbox_parent_project_short_name');
    $this->field_alias = 'isa_toolbox_parent_project_short_name';
  }

  function render($values) {
    $group_short_name = $values->isa_toolbox_community_short_name;
    if ($group_short_name == '')
      $group_short_name = $values->isa_toolbox_project_short_name;
    if ($group_short_name == '')
      $group_short_name = $values->isa_toolbox_parent_community_short_name;
    if ($group_short_name == '')
      $group_short_name = $values->isa_toolbox_parent_project_short_name;
    if ($group_short_name == '')
      $group_short_name = $values->nid;
    return check_plain($group_short_name);
  }
}
?>