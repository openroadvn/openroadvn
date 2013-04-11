<?php

/**
 * Change Log
 * 20120520: SWS  Set share-this to FALSE until it is configured for Openray
 * Defect in this file : 
 * 
 * -----------------------------------------
 * Button 'subscribe-rss' use different access callback 
 *      - 'isa_links_not_add_or_edit_or_workflow_page'
 *      - 'isa_links_is_list'
 *      - 'isa_links_project_is_list_page'
 *      - 'NULL'
 * It's not coherent
 * -----------------------------------------
 * 
 * 
 * 
 * 
 */


/**
 * List of all buttons depending on the path
 * The 'propose_your' button must be beforethe 'action' buttons
 * Button parameters :
  'title'       => Title displayed
  'href'        => Link to the new page/popup
  'class'       => CSS class
  'permission'  => button is displayed if the user has this permission
  'access_callback'    => function (defined in this file) which return TRUE if the button must be displayed
  'url_callback'       => return the replacement string for the href
 * If you need a share this button, use : 'share-this' => TRUE,
 * If you need a rating vote, use : 'rate' => array('label' => 'Rate this * item:', 'node' => $node),
 */
function isa_links_buttons_datas() {
  $datas = array(
      'people/*'                  => 'isa_links_get_my_page_buttons()',
      'user/*'                    => 'isa_links_get_my_page_buttons()',
      'people/*/blog'             => 'isa_links_get_my_page_buttons()',
      'homepage'                   => 'isa_links_get_front_page_buttons()',
      'vf-homepage'                   => 'isa_links_get_front_page_buttons()',
      'page/*'                    => 'isa_links_get_front_page_buttons()',
      'community/all'             => 'isa_links_get_communities_list_buttons()',
      'community/recommended'     => 'isa_links_get_communities_list_buttons()',
      'community/mine'            => 'isa_links_get_communities_list_buttons()',
      'community/editor'          => 'isa_links_get_communities_list_buttons()',
      'community/*/members'       => 'isa_links_get_members_pages_buttons("community")',
      'community/*/members/*'     => 'isa_links_get_members_pages_buttons("community")',
      'community/*/news/all'      => 'isa_links_get_communities_list_buttons()',
      'community/*/news/*'        => 'isa_links_get_node_display_buttons("news")',
      'community/*/wiki/*'        => 'isa_links_get_node_display_buttons("wiki")',
      'community/*/document/*'    => 'isa_links_get_node_display_buttons("document")',
      'community/*/topic/*'       => 'isa_links_get_node_display_buttons("topic")',
      'community/*/image/*'       => 'isa_links_get_node_display_buttons("image")',
      'community/*/*'             => 'isa_links_get_community_buttons()',
      'asset/all'                 => 'isa_links_get_assets_list_buttons()',
      'vf-asset/all'                 => 'isa_links_get_assets_list_buttons()',
      'asset/recommended'         => 'isa_links_get_assets_list_buttons()',
      'asset/i_use'               => 'isa_links_get_assets_list_buttons()',
      'asset/call_for_comment'    => 'isa_links_get_assets_list_buttons()',
      'asset/editor'              => 'isa_links_get_assets_list_buttons()',
      'asset/page/*'              => 'isa_links_get_assets_static_pages_buttons()',
      'asset/experts'             => 'isa_links_get_assets_static_pages_buttons()',
      'asset/experts/*'           => 'isa_links_get_assets_static_pages_buttons()',
      'asset/*/members'           => 'isa_links_get_members_pages_buttons("project")',
      'asset/*/members/*'         => 'isa_links_get_members_pages_buttons("project")',
      'asset/*/news/all'          => 'isa_links_get_project_buttons()',
      'asset/*/news/*'            => 'isa_links_get_node_display_buttons("news")',
      'asset/*/issue/all'         => 'isa_links_get_project_buttons()',
      'asset/*/issue/*'           => 'isa_links_get_issue_buttons()',
      'asset/*/asset_assistant'   => 'isa_links_get_project_buttons()',
      'asset/*/release/all'       => 'isa_links_get_project_buttons()',
      'asset/*/release/*/survey'  => 'isa_links_get_project_buttons()',
      'asset/*/release/*'         => 'isa_links_get_release_buttons()',
      'asset/*/wiki/*'            => 'isa_links_get_node_display_buttons("wiki")',
      'asset/*/topic/*'           => 'isa_links_get_node_display_buttons("topic")',
      'asset/*/document/*'        => 'isa_links_get_node_display_buttons("document")',
      'asset/*/image/*'           => 'isa_links_get_node_display_buttons("image")',
      'asset/*/*'                 => 'isa_links_get_project_buttons()',
      'software/all'              => 'isa_links_get_software_list_buttons()',
      'vf-software/all'              => 'isa_links_get_software_list_buttons()',
      'software/recommended'      => 'isa_links_get_software_list_buttons()',
      'software/i_use'            => 'isa_links_get_software_list_buttons()',
      'software/editor'           => 'isa_links_get_software_list_buttons()',
      'software/my_software'           => 'isa_links_get_software_list_buttons()',
      'software/federated_forge'  => 'isa_links_get_software_list_buttons("federated_forge")',
      'software/license-wizard/*' => 'isa_links_get_software_list_buttons()',
      'software/page/*'           => 'isa_links_get_software_static_pages_buttons()',
      'software/guidelines'       => 'isa_links_get_software_static_pages_buttons()',
      'software/guidelines/*'     => 'isa_links_get_software_static_pages_buttons()',
      'software/studies'          => 'isa_links_get_software_static_pages_buttons()',
      'software/studies/*'        => 'isa_links_get_software_static_pages_buttons()',
      'software/*/members'        => 'isa_links_get_members_pages_buttons("project")',
      'software/*/members/*'      => 'isa_links_get_members_pages_buttons("project")',
      'software/*/news/all'       => 'isa_links_get_news_item_buttons()',
      'software/*/news/*'         => 'isa_links_get_node_display_buttons("news")',
      'software/*/issue/all'      => 'isa_links_get_project_buttons()',
      'software/*/issue/*'        => 'isa_links_get_issue_buttons()',
      'software/*/release/all'    => 'isa_links_get_project_buttons()',
      'software/*/release/*/survey'      => 'isa_links_get_project_buttons()',
      'software/*/release/*'      => 'isa_links_get_release_buttons()',
      'software/*/wiki/*'         => 'isa_links_get_node_display_buttons("wiki")',
      'software/*/topic/*'        => 'isa_links_get_node_display_buttons("topic")',
      'software/*/document/*'     => 'isa_links_get_node_display_buttons("document")',
      'software/*/image/*'        => 'isa_links_get_node_display_buttons("image")',
      'software/*/*'              => 'isa_links_get_project_buttons()',
      'federated_forge/*'         => 'isa_links_get_federated_forges_buttons()',
      'federated_project/*/*'     => 'isa_links_get_federated_projects_buttons()',
      'news/newsletter'           => 'isa_links_get_newsletter_list_button()',
      'newsletter/*'              => 'isa_links_get_node_display_buttons("newsletter")',
      'news/blog'                 => 'isa_links_get_blog_post_list_buttons()',
      'news/all'                  => 'isa_links_get_news_all_list_buttons()',
      'news/news'                 => 'isa_links_get_news_list_buttons()',
      'news/recommended'          => 'isa_links_get_news_list_buttons()',
      'news/editor'               => 'isa_links_get_news_list_buttons()',
      'news/*'                    => 'isa_links_get_node_display_buttons("news")',
      'blog/*'                    => 'isa_links_get_node_display_buttons("blog")',
      'node/*/edit'               => 'isa_links_get_edit_buttons()',
      'node/*/edit/releases' => 'isa_links_get_edit_buttons ()',
      'node/*/revisions'          => 'isa_links_get_edit_buttons()',
      'node/*/revisions/*'        => 'isa_links_get_edit_buttons()',
      'node/*/workflow'           => 'isa_links_get_edit_buttons()',
      'node/*/translate'          => 'isa_links_get_edit_buttons()',
      'node/add/*'                => 'isa_links_get_add_buttons()',
      'help'                      => 'isa_links_get_front_page_buttons()',
      'help_topics'               => 'isa_links_get_front_page_buttons()',
      'contexthelpfaq/*'          => 'isa_links_get_front_page_buttons()',
      'contact'                   => 'isa_links_get_front_page_buttons()',
      'sitemap'                   => 'isa_links_get_front_page_buttons()',
      'glossary'                  => 'isa_links_get_front_page_buttons()',
      'glossary/*'                => 'isa_links_get_front_page_buttons()',
      'partners'                  => 'isa_links_get_front_page_buttons()',
      'metrics'                   => 'isa_links_get_front_page_buttons()',
      'category/glossary/*'       => 'isa_links_get_front_page_buttons()',
      'document/*'                => 'isa_links_get_document_buttons()',
      'event/all'                 => 'isa_links_get_event_list_buttons()',
      'event/archives'            => 'isa_links_get_event_list_buttons()',
      'event/recommended'         => 'isa_links_get_event_list_buttons()',
      'event/editor'              => 'isa_links_get_event_list_buttons()',
      'event/*'                   => 'isa_links_get_node_display_buttons("event")',
      'elibrary/case/*'           => 'isa_links_get_node_display_buttons("case")',
      'elibrary/factsheet/*'      => 'isa_links_get_node_display_buttons("factsheet")',
      'elibrary/video/*'          => 'isa_links_get_node_display_buttons("video")',
      'elibrary/document/*'       => 'isa_links_get_node_display_buttons("document")',
      'elibrary/factsheet'        => 'isa_links_get_elibrary_list_buttons("factsheet")',
      'elibrary/*'                => 'isa_links_get_elibrary_list_buttons()',
  );
  return $datas;
}

function isa_links_get_add_buttons() {
  $nid = $_GET['gids'][0];
  $path = explode('/', drupal_get_path_alias($_GET['q']));
  if ($nid && is_numeric($nid)) {
    $node = node_load($nid);
    return _isa_links_get_appropriate_buttons($node, $path[2]);
  }
  else {
    $type = $_GET['type'];
    if ($type) {
      switch ($type) {
        case ISA_ASSET_TYPE:
          return isa_links_get_assets_list_buttons();
        case ISA_SOFTWARE_TYPE:
        case 'OSS':
          return isa_links_get_software_list_buttons();
      }
    }
    else {
      $nid = variable_get('current_group', isa_toolbox_get_community_nid());
      $node = node_load($nid);
      switch ($path[2]) {
        case ISA_COMMUNITY_TYPE:
          return isa_links_get_communities_list_buttons();
        default:
          return _isa_links_get_appropriate_buttons($node, $path[2]);
      }
    }
  }
}
/*
function isa_links_get_revisions_buttons() {
  $node = menu_get_object();
  return _isa_links_get_appropriate_buttons($node);
}*/

function isa_links_get_edit_buttons() {
  $node = node_load(arg(1));
  return _isa_links_get_appropriate_buttons($node);
}

function _isa_links_get_appropriate_buttons($node, $type = NULL) {
  if (!$type) {
    $type = $node->type;
  }
  switch ($type) {
    case ISA_WIKI_TYPE:
      if (isa_toolbox_is_license_wizard_wiki($node)){
        return isa_links_get_software_list_buttons ();
      }else{
        return isa_links_get_node_display_buttons($type,TRUE);
      }
      break;
    case ISA_NEWSLETTER_TYPE: 
    case ISA_NEWS_TYPE:
    case ISA_EVENT_TYPE:
    case ISA_BLOG_TYPE:
    case ISA_TOPIC_TYPE:
    case ISA_DOCUMENT_TYPE:
    case ISA_CASE_TYPE:
    case ISA_VIDEO_TYPE:
    case ISA_FACTSHEET_TYPE:
    case ISA_IMAGE_TYPE:
      return isa_links_get_node_display_buttons($type,TRUE);
    case ISA_ISSUE_TYPE:
    case 'project-issue':
    case 'issue':
      return isa_links_get_issue_buttons($node);
    case ISA_PROJECT_RELEASE_TYPE:
    case 'project-release':
    case 'release':
      return isa_links_get_release_buttons($node);
    case ISA_FEDERATED_FORGE_TYPE:
    case 'federated-forge':
      return isa_links_get_federated_forges_buttons($node);
    case ISA_FEDERATED_PROJECT_TYPE:
    case 'federated-project':
      return isa_links_get_federated_projects_buttons($node);
    case ISA_STUDY_TYPE:
      return isa_links_get_software_list_buttons();
    case 'page':
      return isa_links_get_front_page_buttons();
  }
  //$group = node_load(array_shift(array_keys($node->og_groups_both)));
  if ($node->group_type) {
    switch ($node->group_type) {
      case ISA_COMMUNITY_TYPE:
        return isa_links_get_community_buttons();
      case ISA_ASSET_TYPE:
      case ISA_SOFTWARE_TYPE:
    return isa_links_get_project_buttons();
    }
  }
}

function isa_links_get_issue_buttons($group = NULL) {
  if (!$group) {
    $group = node_load(variable_get('current_group', isa_toolbox_get_community_nid()));
  }else{
    // if the param $group is the project issue
    // the function '_isa_links_get_appropriate_buttons' get the project issue node in $group
    if ($group->type == ISA_ISSUE_TYPE){
      $node = $group;
      $group = node_load(variable_get('current_group', isa_toolbox_get_community_nid()));
    }
  }
  if (!$node){
    $node = menu_get_object();
  }
  return array(
      'push' => array(
          isa_links_get_general_button('submit-issue', 'isa_links_project_submit_issue_access', $group, NULL, "node/add/project-issue/{$group->project['uri']}", 'Create an Issue'),
          isa_links_get_general_button('submit-task', 'isa_links_project_submit_task_access', $group, NULL, "node/add/project-issue/{$group->project['uri']}", 'Create a Task'),
      ),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('asset-assistant', 'isa_links_asset_assistant_access', $node, NULL, "asset/{$node->project['uri']}/asset_assistant", 'Asset Assistant'),
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      ),
      'links' => array(
          isa_links_get_general_button('issue-categories', 'isa_project_issues_categories_access', NULL, NULL, "{$group->group_type}/{$group->project['uri']}/issue/manage_categories", 'Manage issue categories'),
          isa_links_get_general_button('comment-form', 'isa_links_issue_send_comment_access', $node, NULL, $node->path, 'Send your comment'),
          isa_links_get_general_button('subscribe popups-form', 'isa_links_subscribe_access', $node, NULL, "node/$node->nid/subscribe", 'Subscribe', NULL, 'subscribe to content'),
      ),
  );
}

function isa_links_issue_send_comment_access($node) {
  global $user;
  if ($node && isa_links_not_add_or_edit_or_workflow_page() && $user->uid != 0 ) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_get_release_buttons($group = NULL) {
  if (!$group || $group->type == ISA_PROJECT_RELEASE_TYPE) {
    $group = node_load(variable_get('current_group', isa_toolbox_get_community_nid()));
  }
  $node = menu_get_object();
  return array(
      'push' => array(
          isa_links_get_general_button('submit-issue', 'isa_links_project_submit_issue_access', $group, NULL, "node/add/project-issue/{$group->project['uri']}", 'Create an Issue'),
          isa_links_get_general_button('create-release', 'isa_links_project_create_release_access', $group, NULL, "node/add/project-release/{$group->nid}", 'Create a Release'),
      ),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      ),
      'links' => array(
          isa_links_get_general_button('rate', 'isa_links_rate_access', $node, NULL, NULL, 'Rate this Release:'),
          isa_links_get_general_button('subscribe popups-form', 'isa_links_subscribe_access', $node, NULL, "node/$node->nid/subscribe", 'Subscribe', NULL, 'subscribe to content'),
      ),
  );
}

/**
 *
 * @param <type> $node :current node
 * @return <boolean> TRUE if the subscribe link should be accessible
 */
function isa_links_subscribe_access($node) {
  global $user;
  
  subscriptions_suspended($user->uid, TRUE);
  if (is_numeric($node)) {
    $node = node_load($node);
  }
  // don't display subscribe link if the node isn't validated
  if (!isa_links_node_is_validated($node)){
    return FALSE;
  }
  //The user need to be a member of the group to subscribe
  $gid = variable_get('current_group', isa_toolbox_get_community_nid());
  if (isset($gid)) {
    $group = node_load($gid);
    return isa_links_is_group_member_access($group);
  }

  module_invoke_all('subscriptions', 'node_options', $user, $node);
  // don't display the "Subscribe" link for license wizard wiki pages
  $path_alias = explode('/', $node->path);
  if ($path_alias[1] == 'license-wizard') {
    return FALSE;
  }
  if (subscriptions_content_type_is_blocked($node->type) && !user_access('subscribe to all content types')) {
    return FALSE;
  }
  if (isa_links_not_add_or_edit_or_workflow_page() && $node && $user->uid != 0) {
    return TRUE;
  }
  else {
    return FALSE;
  }
  return FALSE;
}

/**
 * List of 'propose your' buttons
 */
function isa_links_get_propose_your_buttons() {
  // anonymous user should have access to buttons for create contents
  $buttons = array(
      array(
          'title' => t('Propose a News Item'),
          'href' => 'node/add/news',
          //'permission' => 'create news content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Propose a Community'),
          'href' => 'node/add/community',
          //'permission' => 'create community content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Propose your Software Project'),
          'href' => 'node/add/project-project',
          //'permission' => 'maintain projects',
          'query' => array('type' => 'OSS'),
      ),
      array(
          'title' => t('Propose your Shared Asset Project'),
          'href' => 'node/add/project-project',
          //'permission' => 'maintain projects',
          'query' => array('type' => 'asset'),
      ),
      array(
          'title' => t('Propose a Document'),
          'href' => 'node/add/document',
          //'permission' => 'create document content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Propose a Case'),
          'href' => 'node/add/case',
          //'permission' => 'create case content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Propose an Event'),
          'href' => 'node/add/event',
          //'permission' => 'create event content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Create a Federated Forge'),
          'href' => 'node/add/federated-forge',
          'permission' => 'create federated_forge content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Create a Blog post'),
          'href' => 'node/add/blog',
          //'permission' => 'create blog content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Create a Factsheet'),
          'href' => 'node/add/factsheet',
          'permission' => 'create factsheet content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Create a Video'),
          'href' => 'node/add/video',
          'permission' => 'create video content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
      array(
          'title' => t('Create a Study'),
          'href' => 'node/add/study',
          'permission' => 'create study content',
          'access_callback' => 'isa_links_is_not_in_virtual_forge',
      ),
  );
  
  return $buttons;
}

/**
 * Reusable buttons
 * @param string $class : class of the button
 * @param string $access_callback : access_callback function, if not present don't set
 * @param string $access_arguments : access_arguments function, if not present don't set
 * @param string $url_callback : url_callback function, if not present don't set
 * @param string $href : url to link to, if not present link to /node
 * @param atring $title : Title of the button
 * @param array  $query : query array, if not present don't set
 * @param string $permission : permission needed to display the button
 */
function isa_links_get_general_button($class, $access_callback = NULL, $access_arguments = NULL, $url_callback = NULL, $href = 'node', $title = 'TITLE', $query = NULL, $permission = NULL) {
  switch ($class) {
    case 'propose-your' :
      $button = array(
          'title' => t('Propose your...'),
          'html' => 'TRUE',
          'class' => 'quick-actions',
          //'access_callback' => 'isa_links_user_is_authenticated',
      );
      break;
    case 'comment-form' :
      $button = array(
          'title' => $title,
          'href' => $href,
          'class' => $class,
          'fragment' => 'comment-form-title',
          'permission' => 'post comments',
      );
      break;
    case 'newsletter-subscription' :
      /*global $user;
      $href = "people/mypage";
      if (isset($user->uid) && $user->uid != 0) {
        $href = "user/$user->uid/edit/newsletter";
      }
      $button = array(
          'title' => t('Subscribe to Newsletter'),
          'href' => $href,
          'class' => 'subscribe-newsletter',
          'query' => $query,
      );*/
      break;
    case 'rate':
      $button = array(
          'label' => t($title),
          'node' => $access_arguments,
          'permission' => 'rate content',
          'class' => $class,
      );
      break;
    default:
      $button = array(
          'title' => $title,
          'href' => $href,
          'class' => $class,
          'query' => $query,
      );
      break;
  }

  if ($access_callback) {
    $button['access_callback'] = $access_callback;
  }
  if ($access_arguments) {
    $button['access_arguments'] = $access_arguments;
  }
  if ($url_callback) {
    $button['url_callback'] = $url_callback;
  }
  if ($permission) {
    $button['permission'] = $permission;
  }

  return $button;
}

function isa_links_people_rss_button_access() {
  if (in_array(arg(2), array('recommended', 'blog'))) {
    return TRUE;
  }
  return FALSE;
}

/**
 *   Control access for create a content of '$node' type
 * 
 * @param object $node
 * @return boolean  
 * recall : 
 * 
 * to create content in group check :
 * - group workflow
 * - group field 'field_*_*_creation'
 * - group membership
 * 
 * to create content in global JoinUp:
 * - authenticated user
 * - moderator for some content, so check permission
 * 
 */
function isa_links_create_content_access($node) {
  $gid = variable_get('current_group', isa_toolbox_get_community_nid());
  if (!is_object($node)){
    $type = $node;
  }else{
    $type = $node->type;
  }
  $perm_access = (user_access("create {$type} content") || user_access ('administer nodes')) ;
  if ($gid) {
    $group = node_load($gid);
    if ($group->_workflow != ISA_SID_COMMUNITY_VALIDATED) {
      return FALSE;
    }
    $is_member = og_is_group_member($group->nid, TRUE);
    switch ($type) {
      case ISA_DOCUMENT_TYPE:
        $create_content = (($group->type == ISA_COMMUNITY_TYPE && $group->field_community_documents_creati[0]['value'] == 'Activated') ||
            ($group->type == ISA_PROJECT_TYPE && $group->field_project_documents_creation[0]['value'] == 'Activated'));
        break;
      case ISA_NEWS_TYPE:
        $create_content = (($group->type == ISA_COMMUNITY_TYPE && $group->field_community_news_creation[0]['value'] == 'Activated') ||
            ($group->type == 'project_project' && $group->field_project_news_creation[0]['value'] == 'Activated'));
        break;
      case ISA_WIKI_TYPE:
        $create_content = (($group->type == ISA_COMMUNITY_TYPE && $group->field_community_wiki_creation[0]['value'] == 'Activated') ||
            ($group->type == ISA_PROJECT_TYPE && $group->field_project_wiki_creation[0]['value'] == 'Activated'));

        break;
      case ISA_TOPIC_TYPE:
        $create_content = (($group->type == ISA_COMMUNITY_TYPE && $group->field_community_forum_creation[0]['value'] == 'Activated') ||
            ($group->type == ISA_PROJECT_TYPE && $group->field_project_forum_creation[0]['value'] == 'Activated'));
        break;
      default:
        $create_content = TRUE;
        break;
    }
    return ($is_member && $perm_access && $create_content);
  }
  else {
    global $user;
    // displays buttons to create content for anonymous user
    // displays only buttons than authenticated user has access
    $perms = permissions_get_permissions_for_role('authenticated user');
    $anonymous_access = ($user->uid === 0) && in_array("create {$type} content", $perms);

    return $perm_access || $anonymous_access;
  }
}

function isa_links_join_access($node) {
  global $user;
  if ($node->og_selective == 0 && isa_links_is_not_group_member_access($node) && $user->uid != $node->uid && isa_links_node_is_validated($node)) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_request_access($node) {
  if ($node->og_selective == 1 && isa_links_is_not_group_member_access($node) && $user->uid != $node->uid && isa_links_node_is_validated($node)) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_highlight_access($node) {
  global $user;
  $state_validated = array(ISA_SID_NEWS_CREATED,ISA_SID_NEWS_PUBLISHED,ISA_SID_NEWS_REQUEST_PUBLICATION,ISA_SID_COMMUNITY_VALIDATED, ISA_SID_NEWS_VALIDATED, ISA_SID_FEDPROJ_PUBLISHED, ISA_SID_RELEASE_CREATED , ISA_SID_RELEASE_REQUESTED, ISA_SID_RELEASE_APPROVED);
  //if (count($user->og_groups) > 0 && ($node->og_selective == 0 || $node->type == ISA_PROJECT_TYPE)) {
  if (count($user->highlight_groups) > 0) { // Checks to see if user is a member of any group
    if (!$node->_workflow || ($node->_workflow && in_array ($node->_workflow,$state_validated))) {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_get_uid_from_arg1() {
  $uri = explode('/', drupal_get_normal_path($_GET['q']));
  return ($uri[1]);
}

function isa_links_is_my_profile() {
  global $user;
  $uri = explode('/', drupal_get_normal_path($_GET['q'])); 
  $uid = $uri[1];
  if ($uri[0] == 'user' || $uri[0] == 'people'){
    if (is_numeric($uid) && $uid == $user->uid){
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_is_someone_else_profile() {
  $uri = explode('/', drupal_get_normal_path($_GET['q']));
  $nid = $uri[1];
  if (!is_numeric($nid)) {
    return FALSE;
  }
  global $user;
  if ($user->uid == 0) {
    return FALSE;
  }
  return ($nid != $user->uid);
}

function isa_links_user_allows_contact() {
  $uri = explode('/', drupal_get_normal_path($_GET['q']));
  $nid = $uri[1];
  $user = user_load($nid);
  return _contact_user_tab_access($user);
}

function isa_links_people_send_message_button_access() {
  return isa_links_is_someone_else_profile()
  && isa_links_user_allows_contact();
}

function isa_links_get_my_page_buttons($type = NULL) {
  $links = array();
  global $user;
  $path = explode('/', drupal_get_path_alias($_GET['q']));

  $buttons = array(
      'push' => array(
          isa_links_get_general_button('invite-people popups', '! isa_links_people_send_message_button_access', NULL, NULL, 'invite', 'Invite People'),
          isa_links_get_general_button('send-message', 'isa_links_people_send_message_button_access', NULL, 'isa_links_get_uid_from_arg1', 'people/%d/contact', 'Send a personal message to this user'),
      ),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
      ),
      'links' => $links,
  );
  return $buttons;
}

function isa_links_get_front_page_buttons() {
  return array(
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('our-services', NULL, NULL, NULL, 'page/our_services', 'Our services'),
          'share-this' => FALSE,
      )
  );
}

function isa_links_get_communities_list_buttons() {
  return array(
      'push' => array(isa_links_get_general_button('propose-community', NULL, NULL, NULL, 'node/add/community', 'Request to create your Community')),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
          'share-this' => FALSE,
      ),
  );
}

function isa_links_get_community_buttons() {
  $nid = variable_get('current_group', isa_toolbox_get_community_nid());
  $node = node_load($nid);
  $path = explode('/', $node->path);
  global $user;
  $editors_choice_flag_properties = _isa_links_get_link_properties($node->nid, 'editor_choice');
  return array(
      'push' => array(
          isa_links_get_general_button('join-community popups-form-reload', 'isa_links_join_access', $node, NULL, 'og/subscribe/' . $nid, 'Join this community'),
          isa_links_get_general_button('request-community-membership popups-form', 'isa_links_request_access', $node, NULL, 'og/subscribe/' . $nid, 'Request membership'),
      ),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('leave-community popups-form-reload', 'isa_links_leave_group', $node, NULL, "og/unsubscribe/{$nid}/{$user->uid}", 'Leave community'),
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('subscribe-rss', 'isa_links_is_list', NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
          'share-this' => FALSE,
      ),
      'links' => array(
          isa_links_get_general_button('create-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news', t('Create a News Item'), array('gids[]' => $nid)),
          isa_links_get_general_button('create-forum', 'isa_links_create_content_access', 'topic', NULL, 'node/add/topic', t('Create a Forum Topic'), array('gids[]' => $nid)),
          isa_links_get_general_button('create-wiki', 'isa_links_create_content_access', 'wiki', NULL, 'node/add/wiki', t('Create a Wiki'), array('gids[]' => $nid)),
          isa_links_get_general_button('create-document', 'isa_links_create_content_access', 'document', NULL, 'node/add/document', t('Create a Document'), array('gids[]' => $nid)),
          isa_links_get_general_button('create-image', 'isa_links_create_content_access', 'image', NULL, 'node/add/image', t('Create an Image'), array('gids[]' => $node->nid), 'create image content'),
          isa_links_get_general_button('add-editors-choice', 'isa_links_node_is_validated', $node, NULL, $editors_choice_flag_properties['href'], $editors_choice_flag_properties['title'], array('destination' => $editors_choice_flag_properties['destination'], 'token' => $editors_choice_flag_properties['token']), 'Display button add editor choice'),
          isa_links_get_general_button('highlight popups-form', 'isa_links_highlight_access', $node, NULL, "hl/{$nid}/edit", t('Highlight this Community'), NULL, 'admin highlight'),
          isa_ml_subscription_link($node, $user->uid, FALSE),
          isa_links_get_general_button('add-ml', 'isa_ml_access_add_ml', NULL, NULL, "community/{$path[1]}/add_ml", t('Add a Mailing List')),
          isa_links_get_general_button('subscribe popups-form', 'isa_links_subscribe_access', $node, NULL, "node/{$nid}/subscribe", t('Subscribe'), NULL, 'subscribe to content'),
      )
  );
}

function isa_links_get_assets_static_pages_buttons() {
  return array(
      'push' => array(isa_links_get_general_button('propose-asset', NULL, NULL, NULL, 'node/add/project-project', 'Request to create your Shared Asset', array('type' => 'asset'))),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      )
  );
}

function isa_links_get_assets_list_buttons() {
  return array(
      'push' => array(isa_links_get_general_button('propose-asset', NULL, NULL, NULL, 'node/add/project-project', 'Request to create your Shared Asset', array('type' => 'asset'))),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('subscribe-rss', 'isa_links_not_add_or_edit_or_workflow_page', NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
          'share-this' => FALSE,
      )
  );
}


function isa_links_not_add_or_edit_or_workflow_page() {
  if (preg_match('/node\/.*\/edit/', $_GET['q']) ||
          preg_match('/node\/add\/.*/', $_GET['q']) ||
          preg_match('/node\/.*\/workflow/', $_GET['q']) ||
          preg_match('/software\/license-wizard\/.*/', drupal_get_path_alias($_GET['q']))) {
    return FALSE;
  }
  return TRUE;
}

function isa_links_get_software_list_buttons($type =  NULL) {
  $buttons =  array(
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
       //   isa_links_get_general_button('legal-question', NULL, NULL, NULL, 'contact', 'Ask a legal question'),
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('subscribe-rss', 'isa_links_not_add_or_edit_or_workflow_page', NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
          'share-this' => FALSE,
      )
  );
  if ($type == 'federated_forge'){
    $buttons ['push'][] = isa_links_get_general_button('create-federated-forge', 'isa_links_create_federated_forge', NULL, NULL, 'node/add/federated-forge', 'Create a Federated Forge');
  }
  $buttons ['push'][] = isa_links_get_general_button('propose-oss', NULL, NULL, NULL, 'node/add/project-project', 'Request to create your OSS Project', array('type' => 'OSS'));
  return $buttons;
}

function isa_links_get_project_buttons() {
  global $user;
  $nid = variable_get('current_group', isa_toolbox_get_community_nid());
  $node = isa_toolbox_get_original_node(node_load($nid));
  $path = explode('/', drupal_get_normal_path($_GET['q']));
  $release_id = isa_toolbox_get_last_release($node->nid);
  $release = NULL;
  if ($release_id) {
    $release = node_load($release_id);
  }
  $uri = $path[1];

  $i_use_flag_properties = _isa_links_get_link_properties($node->nid, 'i_use_this_project', $node->group_type);
  $call_for_comment_properties = _isa_links_get_link_properties($node->nid, 'call_for_review');
  $editors_choice_flag_properties = _isa_links_get_link_properties($node->nid, 'editor_choice');
  return array(
      'push' => array(
          isa_links_get_general_button('request-' . $node->group_type . '-membership popups-form', 'isa_links_request_project_access', $node, NULL, "og/subscribe/{$node->nid}", 'Request to Join this project'),
          isa_links_get_general_button('download', 'isa_links_display_download_button', array($release, $node), NULL, $release->path, 'Download Now'),
          isa_links_get_general_button('submit-issue', 'isa_links_project_submit_issue_access', $node, NULL, "node/add/project-issue/{$uri}", 'Create an Issue'),
          isa_links_get_general_button('submit-task', 'isa_links_project_submit_task_access', $node, NULL, "node/add/project-issue/{$uri}", 'Create a Task'),
          isa_links_get_general_button('create-release', 'isa_links_project_create_release_access', $node, NULL, "node/add/project-release/{$node->nid}", 'Create a Release'),
      ),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('leave-' . $node->group_type . ' popups-form-reload', 'isa_links_leave_group', $node, NULL, "og/unsubscribe/{$node->nid}/{$user->uid}", 'Leave this project'),
          isa_links_get_general_button(($i_use_flag_properties['unflag'] ? 'no-' : '') . 'i-use-' . $node->group_type, 'isa_links_project_use_access', $node, NULL, $i_use_flag_properties['href'], $i_use_flag_properties['title'], array('destination' => $i_use_flag_properties['destination'], 'token' => $i_use_flag_properties['token'])),
          isa_links_get_general_button('asset-assistant', 'isa_links_asset_assistant_access', $node, NULL, "asset/{$node->project['uri']}/asset_assistant", 'Asset Assistant'),
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          isa_links_get_general_button('subscribe-rss', 'isa_links_project_is_list_page', NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
          'share-this' => FALSE,
      ),
      'links' => array(
          isa_links_get_general_button('create-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news', t('Create a News Item'), array('gids[]' => $node->nid)),
          isa_links_get_general_button('create-forum', 'isa_links_create_content_access', 'topic', NULL, 'node/add/topic', t('Create a Forum Topic'), array('gids[]' => $node->nid)),
          isa_links_get_general_button('create-wiki', 'isa_links_create_content_access', 'wiki', NULL, 'node/add/wiki', t('Create a Wiki'), array('gids[]' => $node->nid)),
          isa_links_get_general_button('create-document', 'isa_links_create_content_access', 'document', NULL, 'node/add/document', t('Create a Document'), array('gids[]' => $node->nid)),
          isa_links_get_general_button('create-image', 'isa_links_create_content_access', 'image', NULL, 'node/add/image', t('Create an Image'), array('gids[]' => $node->nid), 'create image content'),
          isa_links_get_general_button('add-editors-choice', 'isa_links_node_is_validated', $node, NULL, $editors_choice_flag_properties['href'], $editors_choice_flag_properties['title'], array('destination' => $editors_choice_flag_properties['destination'], 'token' => $editors_choice_flag_properties['token']), 'Display button add editor choice'),
          isa_links_get_general_button('call-for_comment', 'isa_links_call_for_comment_access', $node, NULL, $call_for_comment_properties['href'], $call_for_comment_properties['title'], array('destination' => $call_for_comment_properties['destination'], 'token' => $call_for_comment_properties['token'])),
          isa_links_get_general_button('highlight popups-form', 'isa_links_highlight_access', $node, NULL, "hl/{$node->nid}/edit", ($node->group_type == 'asset' ? t('Highlight this Shared Asset') : t('Highlight this Software')), NULL, 'admin highlight'),
          isa_ml_subscription_link($node, $user->uid, TRUE),
          isa_links_get_general_button('add-ml', 'isa_ml_access_add_ml', NULL, NULL, "{$node->group_type}/{$path[1]}/add_ml", t('Add a Mailing List')),
          isa_links_get_general_button('subscribe popups-form', 'isa_links_subscribe_access', $node, NULL, "node/{$node->nid}/subscribe", t('Subscribe'), NULL, 'subscribe to content'),
      ),
  );
}

function isa_links_display_download_button($args) {
  $release = array_shift($args);
  $project = array_shift($args);
  if ($release) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_create_federated_forge($node) {
  return user_access('administer nodes') || user_access('create federated_forge content');
}

function isa_links_get_federated_forges_buttons($node) {
  return array(
      'push' => array(isa_links_get_general_button('create-federated-forge', 'isa_links_create_federated_forge', NULL, NULL, 'node/add/federated-forge', 'Create a Federated Forge')),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      )
  );
}

function isa_links_get_federated_projects_buttons($node = NULL) {
  if (!$node){
  $path = explode('/', drupal_get_normal_path($_GET['q']));
  $nid = $path[1];
  $node = node_load($nid);  
  }else{
    $nid = $node->nid;
  }
  if ($node->tnid != 0){
    $i_use_flag_properties = _isa_links_get_link_properties($node->tnid, 'i_use_this_project', $node->group_type);
  }else{
    $i_use_flag_properties = _isa_links_get_link_properties($node->nid, 'i_use_this_project', $node->group_type);
  }
  
  
  $editors_choice_flag_properties = _isa_links_get_link_properties($nid, 'editor_choice');
  $return = array(
    'push' => array(
        isa_links_get_general_button('create-federated-forge', 'isa_links_create_federated_forge', NULL, NULL, 'node/add/federated-forge', 'Create a Federated Forge')),
    'quick_action' => isa_links_get_propose_your_buttons(),
    'action' => array(
        
      isa_links_get_general_button(($i_use_flag_properties['unflag'] ? 'no-' : '') . 'i-use-federated-project', 'isa_links_project_use_access', $node, NULL, $i_use_flag_properties['href'], $i_use_flag_properties['title'], array('destination' => $i_use_flag_properties['destination'], 'token' => $i_use_flag_properties['token'])),
      isa_links_get_general_button('propose-your'),
      isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
      'share-this' => FALSE,
    ),
  );
  if ($path[2] != 'edit'){
    $return['links'] = array(
      isa_links_get_general_button('add-editors-choice', 'isa_links_node_is_validated', $node, NULL, $editors_choice_flag_properties['href'], $editors_choice_flag_properties['title'], array('destination' => $editors_choice_flag_properties['destination'], 'token' => $editors_choice_flag_properties['token']), 'Display button add editor choice'),
      isa_links_get_general_button('highlight popus-form', 'isa_links_highlight_access', $node, NULL, "hl/{$nid}/edit", 'Highlight this Federated Project', NULL, 'admin highlight'),
  );
  }

  return $return;
}

function isa_links_call_for_comment_access($node) {
  if ($node->group_type == ISA_ASSET_TYPE &&
          (user_access('display Call for review quick action') || isa_toolbox_is_omnipotent())) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_project_is_list_page() {
  if ((preg_match('/(asset|software)\/.*\/all/', $_GET['q']) &&
          !preg_match('/(asset|software)\/[^\/]*\/forum\/all/', $_GET['q'])) ||
          preg_match('/(asset|software)\/[^\/]*\/forum\/[IiKkOoMmHh].*/', $_GET['q'])) {
    return TRUE;
  }
  else {
    return FALSE;
  }
}

function isa_links_user_is_authenticated() {
  global $user;
  return ($user->uid != 0);
}

function isa_links_project_use_access($project) {
  if (($project->_workflow == ISA_SID_COMMUNITY_VALIDATED 
          ||  $project->_workflow == ISA_SID_FEDPROJ_PUBLISHED
          )&&
          user_access('display I use this project quick action')) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_is_list() {
  $paths = array('community/*/elibrary/all', 'community/*/highlights/*/all', 'community/*/newsandblog/all');
  foreach ($paths as $path) {
    $regexp = '/^(' . preg_replace(array('/(\r\n?|\n)/', '/\\\\\*/', '/(^|\|)\\\\<front\\\\>($|\|)/'), array('|', '.*', '\1' . preg_quote(variable_get('site_frontpage', 'node'), '/') . '\2'), preg_quote($path, '/')) . ')$/';
    if (preg_match($regexp, $_GET['q'])) {
      return TRUE;
    }
  }
  return FALSE;
}

function _isa_links_get_link_properties($nid, $flag_name, $group_type = NULL) {
  $properties = array();
  $flag = flag_get_flag($flag_name);
  $action = $flag->is_flagged($nid) ? 'unflag' : 'flag';

  // Generate the link URL.
  $link_types = flag_get_link_types();
  $link_type_module = $link_types[$flag->link_type]['module'];
  $link = module_invoke($link_type_module, 'flag_link', $flag, $action, $nid);
  if (isset($link['title']) && empty($link['html'])) {
    $link['title'] = check_plain($link['title']);
  }
  
  $properties['title'] = isset($link['title']) ? $link['title'] : $flag->get_label($action . '_short', $nid);
  $properties['href'] = $link['href'];
  $properties['token'] = flag_get_token($nid);
  $properties['destination'] = $_GET['q'];
  $properties['unflag'] = ($action == 'unflag');

  return $properties;
}

function isa_links_asset_assistant_access($project) {
  if ($project->_workflow == ISA_SID_COMMUNITY_VALIDATED &&
          (preg_match('/(asset|software)\/.*\/issue\/all/', $_GET['q']) ||
          preg_match('/asset\/.*\/asset_assistant/', $_GET['q'])) &&
          isa_asset_assistant_access_edit_form($project->nid)) {
    return TRUE;
  }
  else {
    //project issue edition
    $node = menu_get_object();
    if ($node->type == 'project_issue') {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_project_submit_issue_access($project) {
  if ($project->_workflow == ISA_SID_COMMUNITY_VALIDATED &&
          (preg_match('/(asset|software)\/.*\/issue\/(all|manage|edit|delete)/', $_GET['q']) ||
          preg_match('/asset\/.*\/asset_assistant/', $_GET['q']) ||
          preg_match('/node\/add\/project-(issue|release)\/.*/', $_GET['q']) ||
          preg_match('/(asset|software)\/.*\/release\/all/', $_GET['q']))) {
    return isa_links_user_is_authenticated();
  }
  else {
    //project issue edition
    $node = menu_get_object();
    if ($node->type == 'project_issue' || $node->type == 'project_release') {
      return isa_links_user_is_authenticated();
    }
  }
  return FALSE;
}

function isa_links_project_submit_task_access($project) {
  if ($project->_workflow == ISA_SID_COMMUNITY_VALIDATED &&
          (preg_match('/(asset|software)\/.*\/issue\/(all|manage|edit|delete)/', $_GET['q']) ||
          preg_match('/asset\/.*\/asset_assistant/', $_GET['q']) ||
          preg_match('/node\/add\/project-issue\/.*/', $_GET['q']))) {
    return TRUE;
  }
  else {
    //project issue edition
    $node = menu_get_object();
    if ($node->type == 'project_issue') {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_project_create_release_access($project) {
  global $user;
  if (($user->uid == $project->uid ||
          isa_toolbox_is_omnipotent() ||
          array_key_exists(variable_get('moderator_rid', -1), $user->roles)) &&
          (preg_match('/(asset|software)\/.*\/release\/all/', $_GET['q']) ||
          preg_match('/node\/add\/project-release\/.*/', $_GET['q']))) {
    return TRUE;
  }
  else {
    //project release edition
    $node = menu_get_object();
    if ($node->type == 'project_release' &&
            ($user->uid == $project->uid ||
            isa_toolbox_is_omnipotent() ||
            array_key_exists(variable_get('moderator_rid', -1), $user->roles))) {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_is_group_member_access($node) {
  global $user;
  if ($node->_workflow == ISA_SID_COMMUNITY_VALIDATED) {
    if (og_is_group_member($node->nid, FALSE)) {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_leave_group($node) {
  global $user;
  if ($user->uid == $node->uid) {
    return FALSE;
  }

  if ($node->_workflow == ISA_SID_COMMUNITY_VALIDATED) {
    if (og_is_group_member($node->nid, FALSE)) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * SGS : WHY CHECK !isa_links_user_is_authenticated()  ?? 
 * if user is anonymous, og_is_group_member return FALSE !!!!!
 * THIS FUNCTION IS USELESS , USE !isa_links_is_group_member_acces !!!!
 * 
 * @param object $node
 * @return type 
 */
function isa_links_is_not_group_member_access($node) {

  if (!isa_links_user_is_authenticated()) {
    return FALSE;
  }
  else {
    return!isa_links_is_group_member_access($node);
  }
}

function isa_links_request_project_access($node) {
  global $user;
  if ($user->uid == $node->uid) {
    return FALSE;
  }
  //The user is not authenticated
  if ($user->uid == 0) {
    return FALSE;
  }
  if ($node->_workflow == ISA_SID_COMMUNITY_VALIDATED) {
    if (!og_is_group_member($node->nid, FALSE)) {
      return TRUE;
    }
  }
  return FALSE;
}

function isa_links_is_news_or_blog_list() {
  $path = $_GET['q'];
  if ($path == 'news/all' ||
          $path == 'news/news' ||
          $path == 'news/blog' ||
          $path == 'news/recommended' ||
          $path == 'news/editor') {
    return TRUE;
  }
  return FALSE;
}

function isa_links_get_software_static_pages_buttons() {
  return array(
      'push' => array(isa_links_get_general_button('propose-oss', NULL, NULL, NULL, 'node/add/project-project', 'Request to create your OSS Project', array('type' => 'OSS'))),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('legal-question', NULL, NULL, NULL, 'contact', 'Ask a legal question'),
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      )
  );
}

function isa_links_get_newsletter_list_button() {
  $push = array(isa_links_get_general_button('propose-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news', 'Propose a News item'));
  $buttons = array(
      'push' => $push,
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
          isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
      )
  );
  return $buttons;
}

function isa_links_get_news_list_buttons() {
  global $user;
  $buttons = array(); // create a variable to prevent the case where there is no node
  $gid = variable_get('current_group', isa_toolbox_get_community_nid());
  $node = menu_get_object();
  $node = isa_toolbox_get_original_node($node);
  $node_group = node_load($gid);
   
  $editors_choice_flag_properties = _isa_links_get_link_properties($node->nid, 'editor_choice');
  if ($gid) {
    $push = array(isa_links_get_general_button('create-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news', 'CREATE A NEWS ITEM', array('gids[]' => $gid)));
  }
  else {
    $push = array(isa_links_get_general_button('propose-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news', 'Propose a News item'));
  }
  //there is a no node
  $buttons = array(
      'push' => $push,
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
          isa_links_get_general_button('subscribe-rss', NULL , NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
      )
  );
  return $buttons;
}

function isa_links_get_news_all_list_buttons() {
  return array(
      'push' => array(isa_links_get_general_button('propose-news', 'isa_links_create_content_access', 'news', NULL, 'node/add/news'), isa_links_get_general_button('create-blog', 'isa_links_create_content_access', 'blog', NULL, 'node/add/blog', 'CREATE A BLOG POST')),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
          isa_links_get_general_button('subscribe-rss', 'isa_links_is_news_or_blog_list', NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
      ),
  );
}

function isa_links_get_blog_post_list_buttons() {
  global $user;
  $buttons = array(); //prevnet the case there is no node
  $node = menu_get_object();
  $node = isa_toolbox_get_original_node($node);
  $editors_choice_flag_properties = _isa_links_get_link_properties($node->nid, 'editor_choice');
  //there is no node
  $buttons = array(
      'push' => array(isa_links_get_general_button('create-blog', 'isa_links_create_content_access', 'blog', NULL, 'node/add/blog', 'CREATE A BLOG POST')),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
          isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
      )
  );
  return $buttons;
}


function isa_links_get_event_list_buttons() {
  global $user;
 // if (user_is_logged_in ()) {
    return array(
        'push' => array(isa_links_get_general_button('propose-event', 'isa_links_create_content_access', 'event', NULL, 'node/add/event', 'Propose an Event')),
        'quick_action' => isa_links_get_propose_your_buttons(),
        'action' => array(
            isa_links_get_general_button('propose-your'),
            isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
            isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
            'share-this' => FALSE,
        ),
    );
  //}
  return array('action' => array(isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
                                 'share-this' => FALSE));
}

function isa_links_get_elibrary_list_buttons($type = NULL) {
  global $user;
  //if (user_is_logged_in ()) {
   
    if ($type){
      // check if node type use workflow
      $workflow = workflow_get_workflow_for_type($type);
      if ($workflow) {
        $button_type = 'propose';
      }
      else {
        $button_type = 'create';
      }
      $all_type = node_get_types();
      $type_name = $all_type[$type]->name;
      $push[] = isa_links_get_general_button("$button_type-$type", 'isa_links_create_content_access', 'factsheet', NULL, "node/add/$type", "Propose a $type_name");;
    }
    $push[] = isa_links_get_general_button('propose-case', 'isa_links_create_content_access', 'case', NULL, 'node/add/case', 'Propose a Case');
    $push[] = isa_links_get_general_button('propose-document', 'isa_links_create_content_access', 'document', NULL, 'node/add/document', 'Propose a Document');
    return array(
        'push' => $push,
        'quick_action' => isa_links_get_propose_your_buttons(),
        'action' => array(
            isa_links_get_general_button('propose-your'),
            isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
            isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS'),
            'share-this' => FALSE,
        ),
    );
// // }
//  $buttons = isa_links_get_front_page_buttons();
//  $buttons['action'][] = isa_links_get_general_button('subscribe-rss', NULL, NULL, NULL, $_GET['q'] . '/feed', 'Subscribe to RSS');
//  return $buttons;
}

/**
 * Generate buttons list for contents type (display, add, edit):
 * news,topic,wiki,image,document,factsheet,video,newsletter,
 * 
 * @global object $user the current user
 * @param string $type machine name of content type
 * @param boolean $edit true if add or edit a node
 * @return array 
 */
function isa_links_get_node_display_buttons($type, $edit = FALSE) {
  global $user;
    $links = array();
    $push = array();
    $options = array();
    $node = menu_get_object();
    if ($node){
      $node = isa_toolbox_get_original_node($node);
    }
    else {
      $node = arg(2);
    }
    $gid = variable_get('current_group', isa_toolbox_get_community_nid());
    $all_type = node_get_types();
    $type_name = $all_type[$type]->name;

    // check if node type use workflow
    $workflow = workflow_get_workflow_for_type($type);
    if ($workflow && !$gid) {
      $button_type = 'propose';
    }
    else {
      $button_type = 'create';
    }

    if ($gid) {
      $options = array('gids[]' => $gid);
    }
    $access_param = $node;
    $access = 'isa_links_create_content_access';
    // --- Create/propose $type ---
    $push[] = isa_links_get_general_button("{$button_type}-{$type}", $access, $access_param, NULL, 'node/add/' . $type, ucfirst($button_type) . ' a ' . $type_name, $options);

    if (user_is_logged_in() && !$edit && isa_links_node_is_validated($node)) {
        // --- Rating ---
        if (variable_get('fivestar_title_' . $type, -1) != -1) {
          $links['rate'] = isa_links_get_general_button('rate', 'isa_links_rate_access', $node, NULL, NULL, t('Rate this ') . $type_name . ':');
        }
        // --- highlight ---
        $highlight = taxonomy_vocabulary_load(variable_get('hl_vid', NULL));
        if (in_array($node->type, $highlight->nodes)) {
          $links[] = isa_links_get_general_button('highlight popups-form', 'isa_links_highlight_access', $node, NULL, "hl/{$node->nid}/edit", t("Highlight this ") . $type_name);
        }
        // --- flags ---
        $flags = flag_get_flags('node', $type);
        foreach ($flags as $machine_name => $flag) {
          if ($machine_name != 'bookmarks') {
            $flag_properties = _isa_links_get_link_properties($node->nid, $machine_name, $node->group_type);
            $access = array_intersect(array_keys($user->roles), $flag->roles);

            if ($access) {
              $class = str_replace("_", '-', $machine_name);
              $links[] = isa_links_get_general_button($class, NULL, NULL, NULL, $flag_properties['href'], $flag_properties['title'], array('destination' => $flag_properties['destination'], 'token' => $flag_properties['token']));
            }
          }
        }
        // --- comment ---
        $comment_active = variable_get('comment_' . $type, 0);
        if ($comment_active != 0) {
          if ($type == ISA_TOPIC_TYPE) {
            $title = t('Reply');
          }
          else {
            $title = t('Send your comment');
          }
          $links[] = isa_links_get_general_button('comment-form', NULL, NULL, NULL, $node->path, $title, NULL , 'post comments');
        }

        // --- subscribe ----
        if (!subscriptions_content_type_is_blocked($type)) {
          $links[] = isa_links_get_general_button('subscribe popups-form', 'isa_links_subscribe_access', $node, NULL, "node/$node->nid/subscribe", 'Subscribe', 'subscribe to content');
        }
      
    }

    return array(
      'push' => $push,
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
        isa_links_get_general_button('propose-your'),
        isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
        'share-this' => FALSE,
      ),
      'links' => $links,
    );
}

function isa_links_get_members_pages_buttons($type) {
  
  $gid = variable_get('current_group', isa_toolbox_get_community_nid());
  $query = array('gid' => $gid);  
  
  $buttons = array(
      'push' => array(isa_links_get_general_button("invite-people-{$type} popups", 'isa_links_user_is_authenticated', NULL, NULL, 'invite', 'Invite People', $query)),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
      ),
  );
  return $buttons;
}




function isa_links_rate_access($node) {
  global $user;
  if ($user->uid == $node->uid)
    return FALSE;
  if (isa_links_not_add_or_edit_or_workflow_page() && $user->uid != 0)
    return TRUE;
  return FALSE;
}

function isa_links_get_image_list_buttons() {
  global $user;
  $node = menu_get_object();
  $node = isa_toolbox_get_original_node($node);
  $group = node_load(variable_get('current_group', isa_toolbox_get_community_nid()));
  return array(
      'push' => array(isa_links_get_general_button('create-image', 'isa_links_create_content_access', 'image', NULL, 'node/add/image', 'CREATE A IMAGE', array('gids[]' => $group->nid), 'create image content')),
      'quick_action' => isa_links_get_propose_your_buttons(),
      'action' => array(
          isa_links_get_general_button('propose-your'),
          isa_links_get_general_button('newsletter-subscription', NULL, NULL, NULL),
          'share-this' => FALSE,
      ),
  );
}

/**
 * Check if the node is in validated state (workflow)
 * 
 * @param object $node
 * @return boolean true if the node is validated, else return false
 */
function isa_links_node_is_validated($node) {
  $group_types = array(ISA_COMMUNITY_TYPE, ISA_ASSET_TYPE, ISA_PROJECT_TYPE);
  if (in_array($node->type, $group_types)) {
    return ($node->_workflow == ISA_SID_COMMUNITY_VALIDATED);
  }
  $state_validated = array(ISA_SID_NEWS_CREATED,ISA_SID_NEWS_PUBLISHED,ISA_SID_NEWS_REQUEST_PUBLICATION,ISA_SID_COMMUNITY_VALIDATED, ISA_SID_NEWS_VALIDATED, ISA_SID_FEDPROJ_PUBLISHED, ISA_SID_RELEASE_CREATED , ISA_SID_RELEASE_REQUESTED, ISA_SID_RELEASE_APPROVED);
  if ($node->_workflow){
    return in_array($node->_workflow, $state_validated);
  }
  return TRUE;
}


function isa_links_perm() {
  return (
  array(
      'Display button add editor choice', //permission granted to the administrator and moderator roles
  )
  );
}
