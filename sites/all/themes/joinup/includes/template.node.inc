<?php

/**
 * Override or insert variables into the "node.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 * @see node.tpl.php
 * @see template.php
 */
function _joinup_preprocess_node(&$vars) {
  $function_name = '_joinup_preprocess_' . $vars['node']->type;
  if (function_exists($function_name)) {
    $function_name($vars);
  }
  // don't display $submitted, flag and rating if he's wiki licence wizard
  if ($vars['node']->type == ISA_WIKI_TYPE && isa_toolbox_is_license_wizard_wiki ($vars['node'])){
    unset ($vars['submitted']);
    
  }else{
    
  // Prepare flag to display
  // machine name for flags you want displayed
  $flags_to_display = array('editor_choice', 'call_for_review');
  $flags_counts = flag_get_counts('node', $vars['node']->nid);
  if ($flags_counts) {
    $flags = '';
    foreach ($flags_counts as $flag_name => $count) {
      if (in_array($flag_name, $flags_to_display)) {
        $flag = flag_get_flag($flag_name);
        $flag_class = str_replace(array(" ", "'"), array('-', ''), strtolower($flag->title));
        $flags .= '<div class="' . $flag_class . '" title="' . $flag->title . '">' . $flag->title . '</div>';
      }
    }
    if ($flags) {
      $vars ['flags_view'] = $flags;
    }
  }
  $statistics_counter = $vars['node']->links['statistics_counter']['title'];
  // Prepare vote rating display
  if (variable_get('fivestar_title_' . $vars['node']->type, -1 ) != -1 ) {
    $vars['vote_rating'] = joinup_prepare_vote_rating($vars['node']);
    if ($statistics_counter){
      $vars['vote_rating'] .= ' | ' . $statistics_counter;
    }
  }else{
    $vars['vote_rating'] = $statistics_counter; 
  }
  }
}

/**
 * Override or insert variables into the "node-profile.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_profile(&$vars) {
  $node = $vars['node'];
  $vars['kudos'] = l(userpoints_get_current_points($vars['uid']), isa_toolbox_get_kudos_page(), array('attributes' => array('title' => t('What are kudos?'), 'class' => 'popups')));
  $vars['created'] = format_interval(time() - $vars['node']->created, 2);
  isa_toolbox_get_user_country($vars['node']);

  $include = array(
    variable_get("languages_vid", NULL),
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL));
  isa_toolbox_create_taxonomy_list($node, $include);
  $vars['taxonomy_terms'] = $node->taxonomy_terms;
  drupal_set_title($vars['node']->field_firstname[0]['value']
      . ' '
      . $vars['node']->field_lastname[0]['value']);


  $vars ['profile_cv'] = joinup_prepare_document_attachement($vars['node']->field_profile_cv);
}

/**
 * Override or insert variables into the "node-project.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_project_project(&$vars) {
  global $base_url;
  $node = $vars['node'];
  //og remove $vars['submitted'] (og.module l 449), but is useful
  $vars['submitted'] = theme('node_submitted', $node);
  $vars['mailing_list'] = isa_toolbox_protect_email($node->project['uri'] . '@' . variable_get('isa_ml_domain', NULL));
  $svn_url = str_replace('@shortname@', $node->project['uri'], variable_get('svn_url', $base_url . '/openray/@shortname@'));
  $svn_url = "openray/".$node->project['uri'];  
$vars['svn_url']      = l($svn_url, $svn_url);
  $webdav_url = str_replace('@shortname@', $node->project['uri'], variable_get('webdav_url', $base_url . '/joinup-webdav/@shortname@'));
  $vars['webdav_url']   = l($webdav_url, $webdav_url) . ' (optional)';
  
  if ($node->tnid == 0){
    $flags = flag_get_counts('node', $node->nid);
  }else{
    $flags = flag_get_counts('node', $node->tnid);
  }
  if (isset ($flags['i_use_this_project'])){
      $vars['i_use_this_project'] =   '<strong>' . $flags['i_use_this_project'] . '</strong> ' . t('people uses this project');
  }else{
    $vars['i_use_this_project'] = t('No people using this project');
  }
 
  
  if ($node->group_type == ISA_SOFTWARE_TYPE) {
    $default_language = variable_get('language_default', NULL);
    $default_language = $default_language->language;
    if ($node->language != $default_language) {
      $vars['node']->content['og_mission']['#value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->content['og_mission']['#value']);
      $vars['node']->field_project_soft_features['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_features['0']['value']);
      $vars['node']->field_project_soft_future_plans['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_future_plans['0']['value']);
      $vars['node']->field_project_soft_get_involved['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_get_involved['0']['value']);
      $vars['node']->field_project_soft_public_admin['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_public_admin['0']['value']);
    }

    // use node-software.tpl.php for displays software
    array_push($vars['template_files'], 'node-' . $node->group_type);
    $vocabulary_include = array(
      variable_get("development_status_vid", NULL),
      variable_get("intended_audience_vid", NULL),
      variable_get("license_vid", NULL),
      variable_get("natural_language_vid", NULL),
      variable_get("operating_system_vid", NULL),
      variable_get("programming_language_vid", NULL),
      variable_get("categories_vid", NULL),
    );
    isa_toolbox_create_taxonomy_list($node, $vocabulary_include);
    // sort the taxonomy terms
    foreach ($vocabulary_include as $key => $value) {
      $vocab = taxonomy_vocabulary_load($value);
      if (isset($node->taxonomy_terms[$vocab->name])) {
        $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
      }
    }
    $more_links = array();
    foreach ($node->field_project_soft_more_lin as $key => $value) {
      if (!empty($value['view'])) {
        $more_links[] = $value['view'];
      }
    }
    $vars['more_links'] = implode(", ", $more_links);
    $maven_url = str_replace('@shortname@', $node->project['uri'], variable_get('maven_url', $base_url . '/nexus'));
    $vars['maven_url']  = l($maven_url, $maven_url) . ' (optional)';
    if (user_access('administer nodes')) {
      $vars['edit_link'] = l(t('Edit this software'), "node/{$node->nid}/edit");
    }
    //joinup_set_terms_in_vars($vars, $node);
  }
  elseif ($node->group_type == ISA_ASSET_TYPE) {
    $default_language = variable_get('language_default', NULL);
    $default_language = $default_language->language;
    if ($node->language != $default_language) {
      $vars['node']->content['og_mission']['#value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->content['og_mission']['#value']);
    }
    // use node-asset.tpl.php for displayes asset temlplate
    array_push($vars['template_files'], 'node-' . $node->group_type);

    $vocabulary_include = array(
      variable_get("license_vid", NULL),
      variable_get("natural_language_vid", NULL),
      variable_get("represented_countries_vid", NULL),
      variable_get("asset_status_vid", NULL),
    );
    isa_toolbox_create_taxonomy_list($node, $vocabulary_include);

    // sort the taxonomy terms
    foreach ($vocabulary_include as $key => $value) {
      $vocab = taxonomy_vocabulary_load($value);
      if (isset($node->taxonomy_terms[$vocab->name])) {
        $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
      }
    }
    if (user_access('administer nodes')) {
      $vars['edit_link'] = l(t('Edit this asset'), "node/{$node->nid}/edit");
    }
  }
}

/**
 * Override or insert variables into the "node-federated_project.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_federated_project(&$vars) {
  $node = $vars['node'];
  $default_language = variable_get('language_default', NULL);
  $default_language = $default_language->language;
    if ($node->tnid == 0){
    $flags = flag_get_counts('node', $node->nid);
  }else{
    $flags = flag_get_counts('node', $node->tnid);
  }
  if (isset ($flags['i_use_this_project'])){
      $vars['i_use_this_project'] =   '<strong>' . $flags['i_use_this_project'] . '</strong> ' . t('people uses this project');
  }else{
    $vars['i_use_this_project'] = t('No people using this project');
  }
  if ($node->language != $default_language) {
    $vars['node']->content['body']['#value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->content['body']['#value']);
  $vars['node']->field_project_soft_features['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_features['0']['value']);
  $vars['node']->field_project_soft_future_plans['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_future_plans['0']['value']);
  $vars['node']->field_project_soft_get_involved['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_get_involved['0']['value']);  
  $vars['node']->field_project_soft_public_admin['0']['value'] = str_replace('<p>', "<p lang='" . $node->language . "'>", $vars['node']->field_project_soft_public_admin['0']['value']);  
  }
  
  $vars['translations'] = isa_toolbox_get_translations_list($node->nid);
  $vocabulary_include = array(
    variable_get("development_status_vid", NULL),
    variable_get("intended_audience_vid", NULL),
    variable_get("license_vid", NULL),
    variable_get("natural_language_vid", NULL),
    variable_get("operating_system_vid", NULL),
    variable_get("programming_language_vid", NULL),
    variable_get("keywords_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include);
  // sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
    $more_links = array();

      foreach ($node->field_project_soft_more_links as $key => $value) {
        if (!empty($value['view'])) {
          $more_links[] = $value['view'];
        }
      }
      $vars['more_links'] = implode(", ", $more_links);
}

/**
 * Override or insert variables into the "node-federated_forge.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_federated_forge(&$vars) {
  $node = $vars['node'];
  // Reload the cached node
  $vars['translations'] = isa_toolbox_get_translations_list($node->nid);
  $url = $node->field_forges_homepage['0']['url'];
  if (substr($url, 0, 5) != 'http:') {
    $url = 'http://' . $url;
  }
  //$vars['homepage']['url'] = l($node->field_forges_homepage['0']['title'], $url, array('external' => TRUE, 'abbsolute' => TRUE));
  
}

/**
 * Override or insert variables into the "node-community.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_community(&$vars) {
  $node = $vars['node'];
  $community_shortname = $vars['field_community_short_name']['0']['value'];
  $vars['submitted'] = theme('node_submitted', $node);
  $vars['mailing_list'] = isa_toolbox_protect_email($community_shortname . '@' . variable_get('isa_ml_domain', NULL));
  if (user_access('administer nodes')) {
    $vars['edit_link'] = l(t('Edit this community'), "node/{$node->nid}/edit");
  }
}

/**
 * Override or insert variables into the "node-topic.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_topic(&$vars) {
  $node = $vars['node'];
  $profile = content_profile_load('profile', $vars['uid']);
  $picture = $profile->picture;
  $vars['picture'] = $picture;
  $vars['user_company_name'] = $profile->field_company_name['0']['value'];
  $vars['postnumber'] = isa_toolbox_get_users_topics_count($node->uid);

  //set the taxonomy terms for user
  isa_toolbox_create_taxonomy_list($profile, array(variable_get('country_vid', NULL)));
  $country = taxonomy_vocabulary_load(variable_get('country_vid', NULL));
  if (isset($profile->taxonomy_terms[$country->name])) {
    $vars['user_countries'] = $profile->taxonomy_terms[$country->name];
  }
  //set the taxonomy terms for community
  isa_toolbox_create_taxonomy_list($node, array(variable_get('domains_vid', NULL), variable_get('forum_vid', NULL), variable_get('languages_vid', NULL)));
  $domains = taxonomy_vocabulary_load(variable_get('domains_vid', NULL));
  $vars['domains'] = $node->taxonomy_terms[$domains->name];
  $languages = taxonomy_vocabulary_load(variable_get('languages_vid', NULL));
  $vars['languages'] = $node->taxonomy_terms[$languages->name];

  $forum = taxonomy_vocabulary_load(variable_get('forum_vid', NULL));
  if (isset($node->taxonomy_terms[$forum->name])) {
    $gid = $vars['og_groups'];
    $gid = array_shift($gid);
    $group = node_load($gid);

    foreach ($node->taxonomy as $tid => $value) {
      if ($value->vid == variable_get('forum_vid', NULL)) {
        $forum_name = $value->name;
      }
    }
    $group_short_name = isa_links_get_group_short_name($group);
    $vars['forum'] = l($node->taxonomy_terms[$forum->name], "$group->group_type/$group_short_name/forum/$forum_name");
  }
  $keywords = taxonomy_vocabulary_load(variable_get('keywords_vid', NULL));
  if (isset($node->taxonomy_terms[$keywords->name])) {
    $vars['keywords'] = $node->taxonomy_terms[$keywords->name];
  }
  //set the taxonomy term for topic
  isa_toolbox_create_taxonomy_list($node);
  $domains = taxonomy_vocabulary_load(variable_get('domains_vid', NULL));
  if (isset($node->taxonomy_terms[$domains->name])) {
    $vars['domains'] = $node->taxonomy_terms[$domains->name];
  }
  $languages = taxonomy_vocabulary_load(variable_get('languages_vid', NULL));
  if (isset($node->taxonomy_terms[$languages->name])) {
    $vars['languages'] = $node->taxonomy_terms[$languages->name];
  }
  //set the taxonomy term for user
  //joinup_set_terms_in_vars($vars, $node);
  //add highlight link
  $hl[] = $node->links['hl_link'];
  $vars['highlight_link'] = theme_links($hl);
  //add subscription link
  $sub[] = $node->links['subscription_link'];
  $vars['subscription_link'] = theme_links($sub);
  //add statistics reads
  $statistic = statistics_get($node->nid);
  $vars['statistic_counter'] = $statistic['totalcount'];
  //$vars['highlight_link'] =  $node->links['statistics_counter']['title'];

  $group = $vars['og_groups'];
  $group_id = array_shift(array_keys($group));
  if (og_is_group_member($group_id, FALSE, $vars['uid'])) {
    $user = user_load($vars['uid']);
    $group = node_load($group_id);
    if ($group->uid == $vars['uid']) {
      $joined_group = $group->created;
    }
    else {
      $joined_group = $user->og_groups[$group_id]['created'];
    }
    $vars['joined_group'] = date("d/m/Y", $joined_group);
  }
}

/**
 * Override or insert variables into the "node-document.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_document(&$vars) {
  $node = $vars['node'];

  // Document categories
  $vocabulary_include = array(
    variable_get("nature_of_documentation_vid", NULL),
    variable_get("document_license_vid", NULL),
    variable_get("country_vid", NULL),
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL)
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  // Sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
  // Document Information
  $vars['document_info'] = array(
    'Publication date' => date('d F Y', strtotime($node->field_publication_date['0']['value'])),
    'Publisher' => $node->field_publisher['0']['value'],
    'ISBN Number' => $node->field_isbn['0']['value'],
    'Description of license' => $node->field_description_of_license['0']['value']
  );
  $vars['original_url']['url'] = $node->field_original_url['0']['url'];
  $vars['original_url']['title'] = $node->field_original_url['0']['title'];
  $vars['email_contact'] = $node->field_email['0']['value'];

  if (!empty($node->field_additional_doc_file[0]['fid'])) {
    global $base_url;
    $vars['additional_doc'] = joinup_prepare_document_attachement ($node->field_additional_doc_file);
  }
}

/**
 * Override or insert variables into the "node-case.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_case(&$vars) {
  $node = $vars['node'];

  // Document categories
  $vocabulary_include = array(
    variable_get("country_vid", NULL),
    variable_get("initiative_type_vid", NULL),
    variable_get("scope_vid", NULL),
    variable_get("case_status_vid", NULL),
    variable_get("service_type_vid", NULL),
    variable_get("overall_implementation_approach_vid", NULL),
    variable_get("technology_choice_vid", NULL),
    variable_get("funding_source_vid", NULL),
    variable_get("implementation_cost_vid", NULL),
    variable_get("yearly_cost_vid", NULL),
    variable_get("return_investment_vid", NULL),
    variable_get("keywords_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include);
  // Sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
  // Case Information
  $vars['case_info'] = array(
    'Acronym' => $node->field_acronym['0']['value'],
    'Presentation URL' => (!empty($node->field_presentation_url['0']['title'])) ? '<a href="' . $node->field_presentation_url['0']['url'] . '">' . $node->field_presentation_url['0']['title'] . '</a>' : '<a href="' . $node->field_presentation_url['0']['url'] . '">' . $node->field_presentation_url['0']['url'] . '</a>',
    'Website URL' => (!empty($node->field_website_url['0']['title'])) ? '<a href="' . $node->field_website_url['0']['url'] . '">' . $node->field_website_url['0']['title'] . '</a>' : '<a href="' . $node->field_website_url['0']['url'] . '">' . $node->field_website_url['0']['url'] . '</a>',
    'City/Region' => $node->field_city_region['0']['value'],
    'Start date' => date('d F Y', strtotime($node->field_start_end_date['0']['value'])),
    'End date' => date('d F Y', strtotime($node->field_start_end_date['0']['value2'])),
    'Operational date' => date('d F Y', strtotime($node->field_operational_date['0']['value']))
  );
  // Case long text information
  $vars['case_long_text_info'] = array(
    'Policy context' => array('class' => 'policy', 'text' => $node->field_policy_context['0']['value']),
    'Target users' => array('class' => 'target-user', 'text' => $node->field_target_users['0']['value']),
    'Description of target users and groups' => array('class' => 'target-desc', 'text' => $node->field_desc_target_users_groups['0']['value']),
    'Description of the way to implement the initiative' => array('class' => 'implementation-desc', 'text' => $node->field_desc_implementation['0']['value']),
    'Technology solution' => array('class' => 'tech-solution', 'text' => $node->field_tech_solution['0']['value']),
    'Main results, benefits and impacts' => array('class' => 'benefits', 'text' => $node->field_main_results['0']['value']),
    'Return on investment description' => array('class' => 'investment-desc', 'text' => $node->field_roi_desc['0']['value']),
    'Track record of sharing' => array('class' => 'track-record', 'text' => $node->field_track_record_sharing['0']['value']),
    'Lessons learnt' => array('class' => 'lessons-learnt', 'text' => $node->field_lessons_learnt['0']['value'])
  );
  //Prepare document attachement
  if (!empty($node->field_case_documentation[0]['fid'])) {
     $vars['documents'] = joinup_prepare_document_attachement ($node->field_case_documentation);
  }
}

/**
 * Override or insert variables into the "node-news.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see joinup_preprocess_node
 */
function _joinup_preprocess_project_issue(&$vars) {
  $node = $vars['node'];
  $project = node_load(array('nid' => $node->project_issue['pid'], 'type' => 'project_project'));
  $release->nid = $node->project_issue['rid'];
    if (module_exists('project_release')) {
      $release = project_release_load($release);
    }
    $assigned = ($node->project_issue['assigned'] && ($account = user_load(array('uid' => $node->project_issue['assigned']))) ? $account->name : t('Unassigned'));

    $current_data = array();
    $current_data['pid'] = array(
      'label' => t('Project'),
      'current' => $project->title,
    );
    if (!empty($release->project_release['version'])) {
      $current_data['rid'] = array(
        'label' => t('Version'),
        'current' => $release->project_release['version'],
      );
    }
    $current_data['component'] = array(
      'label' => t('Component'),
      'current' => $node->project_issue['component'],
    );
    $current_data['category'] = array(
      'label' => t('Category'),
      'current' => project_issue_category($node->project_issue['category'], 0,$node->project_issue['pid']),
    );
    $current_data['priority'] = array(
      'label' => t('Priority'),
      'current' => project_issue_priority($node->project_issue['priority']),
    );
    $current_data['assigned'] = array(
      'label' => t('Assigned'),
      'current' => $assigned,
    );
    $current_data['sid'] = array(
      'label' => t('Status'),
      'current' => project_issue_state($node->project_issue['sid']),
    );
    $current_data['nid'] = array(
      'label' => t('Identifier'),
      'current' => $node->project_issue['nid'],
    );
  $vars['issue_data']= $current_data;
  
  //Prepare document attachement
  if (!empty($node->field_project_issues_attachement[0]['fid'])) {
     $vars['documents'] = joinup_prepare_document_attachement ($node->field_project_issues_attachement);
  }
}


function _joinup_preprocess_blog(&$vars) {
  _joinup_preprocess_news($vars);
}

function _joinup_preprocess_news(&$vars) {
  $node = $vars['node'];

  //prepare taxonomy
  $vocabulary_include = array(
    variable_get("country_vid", NULL),
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  //sort taxonomy
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }

  // prepare author name
  $vars['author'] = theme('username', $node);

  //prepare date created
  $vars['created_view'] = date('d/m/Y - G:i', $node->created);
  
  //Prepare document attachement
  if (!empty($node->field_documentation[0]['fid'])) {
     $vars['documents'] = joinup_prepare_document_attachement ($node->field_documentation);
  }
}

/**
 * Override or insert variables into the "node-factsheet.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_factsheet(&$vars) {
  $node = $vars['node'];
  //prepare taxonomy
  $vocabulary_include = array(
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL),
    variable_get("factsheet_topic_vid", NULL),
    variable_get("country_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include);
  // sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
    if (!empty($node->	field_factsheet_upload_files[0]['fid'])) {
     $vars['documents'] = joinup_prepare_document_attachement ($node->field_factsheet_upload_files);
  }
  //prepare link
//  $vars['statistics_counter'] = $node->links['statistics_counter']['title'];
//  unset($node->links['statistics_counter']);
//  $vars['links'] = theme_links($node->links);
}

/**
 * Override or insert variables into the "node-event.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_event
 */
function _joinup_preprocess_event(&$vars) {
  $node = $vars['node'];

  //prepare link
  $vars['links'] = theme_links($node->links);
  // Document categories
  $vocabulary_include = array(
    variable_get("country_vid", NULL),
    variable_get("keywords_vid", NULL),
    variable_get("domains_vid", NULL),
    variable_get("scope_vid", NULL),
    variable_get("free_event_vid", NULL),
    variable_get("open_event_vid", NULL),
    variable_get("organisation_type_vid", NULL)
  );

  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  $vars['taxonomy_terms_links'] = array();
  // sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
  $display_website = ($node->field_event_website['0']['title'] != "" && $node->field_event_website['0']['url'] != "");

  // event Information
  $vars['event_info'] = array(
    'Start date' => isa_toolbox_get_timezone_date($node->field_event_dates['0']['value'], $node->field_event_dates['0']['timezone']),
    'End date' => isa_toolbox_get_timezone_date($node->field_event_dates['0']['value2'], $node->field_event_dates['0']['timezone']),
    'City' => $node->field_event_city['0']['value'],
    'GMap location' => $node->field_event_gmap_location['0']['value'],
    'Organiser' => $node->field_event_organiser['0']['value'],
    'Website' => $display_website ? l($node->field_event_website['0']['title'], $node->field_event_website['0']['url']) : "",
    'Contact email' => isa_toolbox_protect_email($node->field_event_contact_email['0']['value']),
  );

  // Case long text information
  $vars['event_long_text_info'] = array(
    'Venue' => array('class' => 'policy', 'text' => $node->field_event_venue['0']['value']),
    'Address/Location' => array('class' => 'policy', 'text' => $node->field_event_address_location['0']['value']),
    'Agenda' => array('class' => 'policy', 'text' => $node->field_event_agenda['0']['value']),
    'Expected participants' => array('class' => 'policy', 'text' => $node->field_event_exp_participants['0']['value']),
    'Fees description' => array('class' => 'policy', 'text' => $node->field_event_fees_description['0']['value']),
  );
}

function _joinup_preprocess_newsletter(&$vars) {
  $node = $vars['node'];
  $vocabulary_include = array(
    variable_get("newsletter_vid", NULL),
    variable_get("domains_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include);
  // Sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
}

/**
 * Override or insert variables into the "node-video.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_video
 */
function _joinup_preprocess_video(&$vars) {
  $node = $vars['node'];
  $vars['group_description'] = $node->content['body']['#value'];
  //prepare link
  $vars['links'] = theme_links($node->links);
  // Video categories
  $vocabulary_include = array(
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL)
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  // Sort the taxonomy terms
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
  // Video Information
  $vars['video'] = $node->field_video_link_to_video[0]['view'];
}

function _joinup_preprocess_wiki(&$vars) {
  $node = $vars['node'];
  $path_alias = explode('/', $vars['node']->path);
  if ($path_alias[1] == 'license-wizard')
    unset($vars['node']->content['fivestar_widget']);
  $vars['description'] = $vars['node']->content['body']['#value'];

  //prepare taxonomy
  $vocabulary_include = array(
    variable_get("country_vid", NULL),
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
}

/**
 * Override or insert variables into the "node-image.tpl.php" templates.
 *
 * @param array $vars
 *   An array of variables to pass to the theme template.
 *
 * @see _joinup_preprocess_node
 */
function _joinup_preprocess_image(&$vars) {
  $node = $vars['node'];
  $vocabulary_include = array(
    variable_get("domains_vid", NULL),
    variable_get("keywords_vid", NULL),
  );
  isa_toolbox_create_taxonomy_list($node, $vocabulary_include, TRUE);
  foreach ($vocabulary_include as $key => $value) {
    $vocab = taxonomy_vocabulary_load($value);
    if (isset($node->taxonomy_terms[$vocab->name])) {
      $vars['taxonomy_terms'][$vocab->name] = $node->taxonomy_terms[$vocab->name];
    }
  }
}


function _joinup_preprocess_project_release (&$vars){
  $node = $vars['node'];
  //Prepare document attachement
  if (!empty($node->field_release_document[0]['fid'])) {
    $vars ['documents'] = joinup_prepare_document_attachement ($node->field_release_document);
  }
  
  
}
