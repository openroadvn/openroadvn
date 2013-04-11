<?php

/**
 * List of all menus depending on the path
 */
function isa_links_menus_datas() {
  $datas = array(
      'page/*'                => 'isa_links_get_info_page_menus()',
      'people/*'              => 'isa_links_get_people_menus()',
      'user/*'                => 'isa_links_get_people_menus()',
      'admin/*'               => 'array(\'Admin\' => isa_links_get_admin_menus())',
      'community/all'         => 'isa_links_get_communities_list_menus()',
      'community/recommended' => 'isa_links_get_communities_list_menus()',
      'community/mine'        => 'isa_links_get_communities_list_menus()',
      'community/editor'      => 'isa_links_get_communities_list_menus()',
      'community/*/*'         => 'isa_links_get_group_menus(ISA_COMMUNITY_TYPE)',
      'asset/all'             => 'isa_links_get_assets_list_menus()',
      'asset/recommended'     => 'isa_links_get_assets_list_menus()',
      'asset/i_use'           => 'isa_links_get_assets_list_menus()',
      'asset/editor'          => 'isa_links_get_assets_list_menus()',
      'asset/call_for_comment'=> 'isa_links_get_assets_list_menus()',
      'asset/page/*'          => 'isa_links_get_assets_list_menus()',
      'asset/experts'         => 'isa_links_get_assets_list_menus()',
      'asset/experts/*'       => 'isa_links_get_assets_list_menus()',
      'asset/*/*'             => 'isa_links_get_group_menus(ISA_ASSET_TYPE)',
      'software/all'          => 'isa_links_get_software_list_menus()',
      'software/recommended'  => 'isa_links_get_software_list_menus()',
      'software/i_use'        => 'isa_links_get_software_list_menus()',
      'software/editor'       => 'isa_links_get_software_list_menus()',
      'software/my_software'       => 'isa_links_get_software_list_menus()',
      'software/federated_forge'  => 'isa_links_get_software_list_menus()',
      'software/license-wizard/*' => 'isa_links_get_software_list_menus()',
      'software/guidelines'   => 'isa_links_get_software_list_menus()',
      'software/guidelines/*' => 'isa_links_get_software_list_menus()',
      'software/studies'      => 'isa_links_get_software_list_menus()',
      'software/studies/*'    => 'isa_links_get_software_list_menus()',
      'software/page/*'       => 'isa_links_get_software_list_menus()',
      'software/*/*'          => 'isa_links_get_group_menus(ISA_SOFTWARE_TYPE)',
      'federated_forge/*'     => 'isa_links_get_software_list_menus()',
      'federated_project/*/*' => 'isa_links_get_software_list_menus()',
      'node/*/edit'           => 'isa_links_get_edit_menus()',
      'node/*/edit/releases'  => 'isa_links_get_edit_menus()',	
      'node/*/revisions'      => 'isa_links_get_edit_menus()',
      'node/*/revisions/*'    => 'isa_links_get_edit_menus()',
      'node/*/workflow'       => 'isa_links_get_edit_menus()',
      'node/*/translate'      => 'isa_links_get_edit_menus()',
      'node/add/*'            => 'isa_links_get_add_menus()',
      'help'                  => 'isa_links_get_info_page_menus()',
      'help_topics'           => 'isa_links_get_info_page_menus()',
      'contexthelpfaq/*'      => 'isa_links_get_info_page_menus()',
      'contact'               => 'isa_links_get_info_page_menus()',
      'sitemap'               => 'isa_links_get_info_page_menus()',
      'glossary'              => 'isa_links_get_info_page_menus()',
      'glossary/*'            => 'isa_links_get_info_page_menus()',
      'category/glossary/*'   => 'isa_links_get_info_page_menus()',
      'partners'              => 'isa_links_get_info_page_menus()',
      'metrics'               => 'isa_links_get_info_page_menus()',
      'page/osor.eu'          => 'isa_links_get_info_page_menus()',
      'page/semic.eu'         => 'isa_links_get_info_page_menus()',
      'news/*'                => 'isa_links_get_news_list_menus()',
      'newsletter/*'          => 'isa_links_get_news_list_menus()',
      'blog/*'                => 'isa_links_get_news_list_menus()',
      'event/*'               => 'isa_links_get_event_menus()',
      'elibrary/*'            => 'isa_links_get_elibrary_menus()',
  );
  return $datas;
}

function isa_links_get_add_menus() {
  $nid = $_GET['gids'][0];
  if ($nid) {
    //add content to a group
    $node = node_load($nid);
    if ($node->group_type) {
      return isa_links_get_group_menus($node->group_type);
    }
  }
  else {
    //add an asset or a software
    $type = $_GET['type'];
    if ($type) {
      switch ($type) {
        case ISA_ASSET_TYPE:
          return isa_links_get_assets_list_menus();
        case ISA_SOFTWARE_TYPE:
        case 'OSS':
          return isa_links_get_software_list_menus();
      }
    }
    else {
      $path = explode('/', $_GET['q']);
      $nid = variable_get('current_group', isa_toolbox_get_community_nid());
      $node = node_load($nid);
      switch ($path[2]) {
        case ISA_COMMUNITY_TYPE:
          return isa_links_get_communities_list_menus();
        case 'project-issue':
        case 'project-release':
          return isa_links_get_group_menus($node->group_type);
        case 'federated-forge':
        case 'federated-project':
          return isa_links_get_software_list_menus();
        case ISA_NEWSLETTER_TYPE:
          return array('Admin' => isa_links_get_admin_menus());
        case ISA_NEWS_TYPE:
        case ISA_BLOG_TYPE:
          return isa_links_get_news_list_menus();
        case ISA_EVENT_TYPE:
          return isa_links_get_event_menus();
        case ISA_CASE_TYPE:
        case ISA_VIDEO_TYPE:
        case ISA_FACTSHEET_TYPE:
        case ISA_DOCUMENT_TYPE:
          return isa_links_get_elibrary_menus();
        case ISA_STUDY_TYPE:
          return isa_links_get_software_list_menus();
      }
    }
  }
}

function isa_links_get_revisions_menus() {
  $node = menu_get_object();
  $group = node_load(array_shift(array_keys($node->og_groups_both)));
  if ($group->group_type) {
    return isa_links_get_group_menus($group->group_type);
  }
  else {
    return isa_links_get_group_menus($node->group_type);
  }
}
/**
 * return the menu for the node concerning oss or asset
 * @param type $node
 * @return type 
 */
function isa_links_treate_group_page($node)
{ 
  if(isa_toolbox_alias_from_oss_or_asset($node))
  { 
    $alias = isa_toolbox_alias_from_oss_or_asset($node);
    switch ($alias) {
      case 'sware_on_openray':        
        return isa_links_get_group_menus(ISA_SOFTWARE_TYPE);
      case 'assets_on_openray':     
        return isa_links_get_group_menus(ISA_ASSET_TYPE);
      default:
        return NULL;
    }
  }
}

function isa_links_get_edit_menus() {
  $node = node_load(arg(1));
  
  
  if (isset ($node->og_group) && !empty ($node->og_group)){
    $group = node_load(array_shift($node->og_groups));
    if ($group->group_type) {
      return isa_links_get_group_menus($group->group_type);
    }
  }
  else {
    switch ($node->type) {
      case ISA_ISSUE_TYPE:
        $node = node_load($node->project_issue['pid']);
        return isa_links_get_group_menus($node->group_type);
      case ISA_PROJECT_RELEASE_TYPE:
        $node = node_load($node->project_release['pid']);
        return isa_links_get_group_menus($node->group_type);
      case ISA_PROJECT_TYPE:
      case ISA_COMMUNITY_TYPE:
        return isa_links_get_group_menus($node->group_type);
      case ISA_FEDERATED_FORGE_TYPE:
      case ISA_FEDERATED_PROJECT_TYPE:
       // for license wizard (is a wiki content)
      case ISA_WIKI_TYPE:
        return isa_links_get_software_list_menus();
      case ISA_EVENT_TYPE:
        return isa_links_get_event_menus();
      case ISA_CASE_TYPE:
      case ISA_VIDEO_TYPE:
      case ISA_FACTSHEET_TYPE:
      case ISA_DOCUMENT_TYPE:
        return isa_links_get_elibrary_menus();
      case ISA_NEWS_TYPE:
      case ISA_BLOG_TYPE:
        return isa_links_get_news_list_menus();
      case ISA_STUDY_TYPE:
        return isa_links_get_software_list_menus();
      case 'page':
        if (NULL != isa_links_treate_group_page($node))
          return isa_links_treate_group_page($node);          
        else
          return isa_links_get_info_page_menus();
      default:
        $node = node_load(array_shift($node->og_groups_both));
        if ($node) {
          return isa_links_get_group_menus($node->group_type);
        }
    }
  }
}

function isa_links_people_list_menu_access() {
  if (in_array(arg(1), array('all', 'experts', 'moderators', 'recommended'))) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_people_mypage_menu_access() {
  global $user;
  if ((is_numeric(arg(1)) && arg(1) == $user->uid) || arg(1) == 'dashboard') {
    return TRUE;
  }
  return FALSE;
}

function isa_links_people_admin_menu_access() {
  global $user;
  if ((isa_toolbox_is_omnipotent() || user_access('administer nodes') || user_access('access Theme Editor for Default theme')) &&
          ((is_numeric(arg(1)) && arg(1) == $user->uid) ||
          arg(1) == 'dashboard' ||
          strstr($_GET['q'], 'admin/user/user') ||
          strstr($_GET['q'], 'admin/content/taxonomy') ||
          strstr($_GET['q'], 'admin/content/simplenews') ||
          strstr($_GET['q'], 'node/add/newsletter')
          )) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_people_blog_menu_access() {
  global $user;

  if (is_numeric(arg(1)) && arg(1) != $user->uid) {
    return TRUE;
  }
  return FALSE;
}

function isa_links_user_uid() {
  $nid = explode('/', drupal_get_normal_path($_GET['q']));
  if (!is_numeric($nid)) {
    global $user;
    return $user->uid;
  }
  return $nid[1];
}

function isa_links_user_uid_blog() {
  $nid = explode('/', drupal_get_normal_path($_GET['q']));
  return $nid[1];
}

function isa_links_get_assets_list_menus() {
  return array(
      t('Shared asset') => array(
          'menus' => array(
              array(
                  'title' => t('Assets on Openray'),
                  'href' => 'node/2553',
              ),
              array(
                  'title' => t('Find asset'),
                  'href' => 'asset/all',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'asset/recommended',
              ),
              array(
                  'title' => t('Assets I use'),
                  'href' => 'asset/i_use',
              ),
              array(
                  'title' => t('Editor\'s choice'),
                  'href' => 'asset/editor',
              ),
               array(
                  'title' => t('Legal Aids'),
                  'href' => 'node/2556',
              ),

          ),
      ),
      t('Clearing process') => array(
          'menus' => array(
              array(
                  'title' => t('What is it?'),
                  'href' => 'node/2554',
              ),
              array(
                  'title' => t('Call for comment'),
                  'href' => 'asset/call_for_comment',
              ),
              

          ),
      ),
    /*  'Methodologies' => array(
          'menus' => array(
              //array(
              //    'title' => t('Asset assistant'),
              //    'href' => 'node/2555',
             // ),
              array(
                  'title' => t('Legal Aids'),
                  'href' => 'node/2556',
              ),
          ),
      ),
      //'Partners' => array(
      //    'menus' => array(
      //        array(
      //            'title' => t('Partner projects'),
      //            'href' => 'node/2558',
      //        ),
       //   ),
     // ),*/
  );
}

function isa_links_get_software_list_menus() {
  return array(
      t('Software') => array(
          'menus' => array(
              array(
                  'title' => t('Software on Openray'),
                  'href' => 'node/2550',
              ),
              array(
                  'title' => t('Find Software'),
                  'href' => 'software/all',
              ),
              array(
                  'title' => t('My Software'),
                  'href' => 'software/my_software',
                 // 'href' => 'software/my_sware',
              ),
              array(
                  'title' => t('Software I use'),
                  'href' => 'software/i_use',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'software/recommended',
              ),
              array(
                  'title' => t('Editor\'s choice'),
                  'href' => 'software/editor',
              ),
               array(
                  'title' => t('Software FAQ'),
                  'href' => 'software/page/faq',
              ),
          ),
      ),
    /*  'Legal Corner' => array(
          'menus' => array(
              array(
                  'title' => t('License Wizard'),
                  'href' => 'node/2048',
              ),
              array(
                  'title' => t('EUPL'),
                  'href' => 'node/2567',
              ),
              array(
                  'title' => t('Licence & Comp. agreements'),
                  'href' => 'node/19778',
              ),
              array(
                  'title' => t('Licence compatibility'),
                  'href' => 'node/19779',
              ),
              array(
                  'title' => t('FAQ'),
                  'href' => 'software/page/faq',
              ),
          ),
      ),*/
      t('Methodologies') => array(
          'menus' => array(
              array(
                  'title' => t('Case studies'),
                  'href' => 'software/studies',
              ),
              array(
                  'title' => t('Guidelines'),
                  'href' => 'software/guidelines',
              ),
          ),
      ),
      t('Partners') => array(
          'menus' => array(
             /* array(
                  'title' => t('OSS Centres'),
                  'href' => 'node/2563',
              ),*/
              array(
                  'title' => t('Federated Forges'),
                  'href' => 'software/federated_forge',
                  
              ),
             /* array(
                  'title' => t('Virtual Forges'),
                  'href' => 'node/2568',
              ),*/
          ),
      ),
  );
}

function isa_links_get_info_page_menus() {
  return array(
      t('Information') => array(
          'menus' => array(
              array(
                  'title' => t('About us'),
                  'href' => 'node/2551',
              ),
              /*array(
                  'title' => t('Osor.eu'),
                  'href' => 'node/2848',
              ),
              array(
                  'title' => t('Semic.eu'),
                  'href' => 'node/2847',
              ),*/
              array(
                  'title' => t('Our services'),
                  'href' => 'node/2549',
              ),
              array(
                  'title' => t('Metrics'),
                  'href' => 'metrics',
              ),
              array(
                  'title' => t('Legal notice'),
                  'href' => drupal_lookup_path('source','page/legal-notice'),
              ),
              array(
                  'title' => t('Get involved'),
                  'href' => 'node/2825',
              ),
              array(
                  'title' => t('Help'),
                  'href' => 'help',
              ),
              array(
                  'title' => t('FAQ'),
                  'href' => 'help_topics',
              ),
              array(
                  'title' => t('Contact'),
                  'href' => 'contact',
              ),
              array(
                  'title' => t('Sitemap'),
                  'href' => 'sitemap',
              ),
              array(
                  'title' => t('Partners'),
                  'href' => 'node/2644',
              ),
              array(
                  'title' => t('Glossary'),
                  'href' => 'glossary',
              ),
              /*array(
                    'title' => t('Widget'),
                    'href' => drupal_lookup_path('source','page/widget') ,
                ),*/
          ),
      ),
  );
}

function isa_links_get_people_menus() {
  return array(
      t('People') => array(
          'menus' => array(
              array(
                  'title' => t('Browse all'),
                  'href' => 'people/all',
              ),
              array(
                  'title' => t('Moderators'),
                  'href' => 'people/moderators',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'people/recommended',
              ),
          ),
          'access_callback' => 'isa_links_people_list_menu_access',
      ),
      t('Profile') => array(//CAUTION : don't remove the beyond space
          'menus' => array(
              array(
                  'title' => t('My Page'),
                  'href' => 'user/%d',
                  'url_callback' => 'isa_links_user_uid',
              ),
              array(
                  'title' => t('My Dashboard'),
                  'href' => 'people/dashboard',
                  'url_callback' => 'isa_links_user_uid',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'people/%d/recommended',
                  'url_callback' => 'isa_links_user_uid',
              ),
          ),
          'access_callback' => 'isa_links_people_mypage_menu_access',
      ),
      t('Admin') => isa_links_get_admin_menus(),
      Profile => array(
          'menus' => array(
              array(
                  'title' => t('User\'s Page'),
                  'href' => 'user/%d',
                  'url_callback' => 'isa_links_user_uid_blog',
              ),
              array(
                  'title' => t('User\'s Blog'),
                  'href' => 'people/%d/blog',
                  'url_callback' => 'isa_links_user_uid_blog',
              ),
          ),
          'access_callback' => 'isa_links_people_blog_menu_access',
      ),
  );
}

function isa_links_get_admin_menus() {
  return array(
      'menus' => array(
          array(
              'title' => t('Manage users'),
              'href' => 'admin/user/user',
              'permission' => 'manage users'
          ),
          array(
              'title' => t('Manage categories'),
              'href' => 'admin/content/taxonomy',
              'permission' => 'administer taxonomy'
          ),
          array(
              'title' => t('Manage newsletters'),
              'href' => 'admin/content/simplenews/users',
              'permission' => 'administer nodes'
          ),
          array(
              'title' => t('Configure newsletters'),
              'href' => 'admin/content/simplenews/types',
              'permission' => 'administer nodes'
          ),
          array(
              'title' => t('Create a Newsletter'),
              'href' => 'node/add/newsletter',
              'permission' => 'administer nodes'
          ),
          array( 
            'title' => t('Edit theme'),
            'href' => 'admin/build/themes/editor/'.variable_get('theme_default',NULL),
            'permission' => 'access Theme Editor for Default theme',  
            'access_callback' => 'isa_toolbox_is_in_virtual_forge',
          ),
      ),
      'access_callback' => 'isa_links_people_admin_menu_access',
  );
}

function isa_links_get_communities_list_menus() {
  return array(
      t('Communities') => array(
          'menus' => array(
              array(
                  'title' => t('Find Community'),
                  'href' => 'community/all',
              ),
              array(
                  'title' => t('My Communities'),
                  'href' => 'community/mine',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'community/recommended',
              ),
              array(
                  'title' => t('Editor\'s Choice'),
                  'href' => 'community/editor',
              ),
          ),
      ),
  );
}

function isa_links_get_event_menus() {
  return array(
      t('Events') => array(
          'menus' => array(
              array(
                  'title' => t('Browse upcoming'),
                  'href' => 'event/all',
              ),
              array(
                  'title' => t('Browse archives'),
                  'href' => 'event/archives',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'event/recommended',
              ),
              array(
                  'title' => t('Editor\'s Choice'),
                  'href' => 'event/editor',
              ),
          ),
      ),
  );
}

function isa_links_get_elibrary_menus() {
  return array(
      t('e-Library') => array(
          'menus' => array(
              array(
                  'title' => t('Browse all'),
                  'href' => 'elibrary/all',
              ),
              array(
                  'title' => t('Cases'),
                  'href' => 'elibrary/case',
              ),
              array(
                  'title' => t('Factsheets'),
                  'href' => 'elibrary/factsheet',
              ),
              array(
                  'title' => t('Web TV'),
                  'href' => 'elibrary/video',
              ),
              array(
                  'title' => t('Documents'),
                  'href' => 'elibrary/document',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'elibrary/recommended',
              ),
              array(
                  'title' => t('Editor\'s Choice'),
                  'href' => 'elibrary/editor',
              ),
          ),
      ),
  );
}

function isa_links_get_news_list_menus() {
  return array(
      t('News') => array(
          'menus' => array(
              array(
                  'title' => t('Browse all'),
                  'href' => 'news/all',
              ),
              array(
                  'title' => t('News'),
                  'href' => 'news/news',
              ),
              array(
                  'title' => t('Blogs'),
                  'href' => 'news/blog',
              ),
              array(
                  'title' => t('Recommended'),
                  'href' => 'news/recommended',
              ),
              array(
                  'title' => t("Editor's choice"),
                  'href' => 'news/editor',
              ),
          ),
      ),
      t('Newsletter') => array(
          'menus' => array(
              array(
                  'title' => t('Archives'),
                  'href' => 'news/newsletter',
              ),
          ),
      ),
  );
}

function isa_links_get_group_menus($group_type) {
  $menu_title = ucfirst($group_type);
  if ($menu_title == 'Asset') {
    $menu_title = t('Shared Asset');
  }

  $group_id = variable_get('current_group', isa_toolbox_get_community_nid());
  $short_name = isa_links_get_group_short_name();

  $project_issue = NULL;
  $project_release = NULL;
  if ($group_type == 'software' || $group_type == 'asset') {
    $original_group = isa_toolbox_get_original_node(node_load($group_id));
    $short_name = isa_links_get_group_short_name($original_group);
    $project_issue = array('title' => t('Issues'),
        'href' => "{$group_type}/{$short_name}/issue/all");
    $project_release = array('title' => t('Releases'),
        'href' => "{$group_type}/{$short_name}/release/all");
  }

  return array(
      $menu_title => array(
          'menus' => array(
              array(
                  'title' => t('Welcome'),
                  'href' => "{$group_type}/{$short_name}/home",
              ),
              array(
                  'title' => t('Description'),
                  'href' => "node/{$group_id}",
              ),
              array(
                  'title' => t('Members list'),
                  'href' => "{$group_type}/{$short_name}/members",
              ),
              $project_issue,
              $project_release,
              array(
                  'title' => t('Metrics'),
                  'href' => "{$group_type}/{$short_name}/metrics",
              ),
          ),
      ),
      t('Highlights') => array(
          'menus' => array(
              array(
                  'title' => t('Shared Assets'),
                  'href' => "{$group_type}/{$short_name}/highlights/asset/all",
              ),
              array(
                  'title' => t('Software'),
                  'href' => "{$group_type}/{$short_name}/highlights/software/all",
              ),
              array(
                  'title' => t('Communities'),
                  'href' => "{$group_type}/{$short_name}/highlights/community/all",
              ),
          ),
          'permission' => 'access community',
      ),
      t('Communications') => array(
          'menus' => array(
              array(
                  'title' => t('Forums'),
                  'href' => "{$group_type}/{$short_name}/forum/all",
              ),
              array(
                  'title' => t('News & Blogs'),
                  'href' => "{$group_type}/{$short_name}/newsandblog/all",
              ),
              array(
                  'title' => t('Events'),
                  'href' => "{$group_type}/{$short_name}/highlights/event/all",
              ),
              array(
                  'title' => t('e-Library'),
                  'href' => "{$group_type}/{$short_name}/elibrary/all",
              ),
              array(
                  'title' => t('Images'),
                  'href' => "{$group_type}/{$short_name}/image/all",
                  'class' => 'popups',
              ),
          ),
          'permission' => 'access community',
      ),
  );
}
