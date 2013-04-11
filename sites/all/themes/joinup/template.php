<?php
// $Id: template.php,v 0.23 2011/01/19 09:52:54 sebastien.millart Exp $

define('MENU_ELIBRARY', 'menu-505');
define('MENU_NEWS', 'menu-498');
define('MENU_EVENT', 'menu-506');
define('MENU_COMMUNITY', 'menu-9693');
define('MENU_SOFTWARE', 'menu-9755');
define('MENU_ASSET', 'menu-496');

/**
 * Override or insert variables into the "page.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 * @see page.tpl.php
 * @ingroup phptemplate
 */
function joinup_preprocess_page(&$vars) {
  $path = $_GET['q'];
  $args = explode('/', $path);
  $last_arg = $args[count($args) - 1];
  if ((isset($_GET['recommended']) || $last_arg == 'recommended') && user_is_anonymous()) {
    $vars['content'] = variable_get('anonymous_connect_text', 'You should be logged in to display this page');
  }

  //set the page title and navigator title on users pages
  if ($args[0] == 'user' && is_numeric($args[1])) {
    $username = strip_tags(theme('username', $args[1]));
    $vars['title'] = $username;
    $vars['head_title'] = $username . ' | ' . variable_get('site_name', 'Joinup');
  }

  $vars['tabs2'] = menu_secondary_local_tasks();
  // Prepare Site Identity ($site_identity variable) for the Header region.
  $alt_text = t('Go to the home page');
  $site_html = '<span class="accessibility-info">' . variable_get('site_name', 'Joinup') . '.</span>';

  // Replace the first occurrence of the base path with an empty string in the logo image path.
  // For example, the base path could be '/' or '/isa/'.
  $logo = substr_replace($vars['logo'], '', 0, strlen($vars['base_path']));

  // Test if the logo or the site HTML exists.
  if ($logo || $site_html) {
    // If the logo exists.
    if ($vars['logo']) {
      // Create a themed image for the logo.
      $logo_image = theme_image($logo, $alt_text, '', array('id' => 'logo'), FALSE);
      // Create a themed link arround the logo image which point to the front page.
      $image_link = l($logo_image, '<front>', array('attributes' => array('title' => $alt_text), 'html' => TRUE));
      // Set the $site_identity variable with the $site_html and $image_link values.
      $vars['site_identity'] = $site_html . $image_link;
    }
    // If not
    else {
      // Set the $site_identity variable with a hypertext link to the front page arround the $site_html value only.
      $vars['site_identity'] = l($site_html, '<front>', array('attributes' => array('title' => $alt_text), 'html' => TRUE));
    }
  }

  // Define the colspan values depending of the visibility of different regions
  if ($vars['left'] && $vars['content']) {
    $vars['right_colspan'] = '5';
  } else {
    if ($vars['left']) {
      $vars['right_colspan'] = '13';
    } elseif ($vars['content']) {
      $vars['right_colspan'] = '5';
    } else {
      $vars['right_colspan'] = '16';
    }
  }

  if ($vars['right'] && $vars['content']) {
    $vars['left_colspan'] = '3';
  } else {
    if ($vars['right']) {
      $vars['left_colspan'] = '11';
    } elseif ($vars['content']) {
      $vars['left_colspan'] = '3';
    } else {
      $vars['left_colspan'] = '16';
    }
  }

  if ($vars['right'] && $vars['left']) {
    $vars['content_colspan'] = '8';
  } else {
    if ($vars['right']) {
      $vars['content_colspan'] = '11';
    } elseif ($vars['left']) {
      $vars['content_colspan'] = '13';
    } else {
      $vars['content_colspan'] = '16';
    }
  }

  // Get the URL path to hide the heading 2 for several content type homepages.
  if (module_exists('path')) {
    $path_alias = drupal_get_path_alias($_GET['q']);
    $alias_parts = explode('/', $path_alias);
    $node = menu_get_object();
    if ($node) {
      $node_type = $node->type;
      $node_type_group = '';

      if (isset($alias_parts[2])) {
        $node_page = $alias_parts[2];
      }
      if (!$node) {
        $node_type_group = $alias_parts[0];
      }

      if ((($node_type == ISA_COMMUNITY_TYPE || $node_type == ISA_PROJECT_TYPE || $node_type_group == ISA_ASSET_TYPE || $node_type_group == ISA_SOFTWARE_TYPE || $node_type_group == ISA_COMMUNITY_TYPE) && ($node_page == 'description' || $node_page == 'home'))
              || $node_type == 'dashboard' || $node_type == 'federatedproject'
              || $node_type == 'federatedforge' || $node_type == ISA_DOCUMENT_TYPE
              || $node_type == ISA_CASE_TYPE || $node_type == ISA_EVENT_TYPE
              || $node_type == ISA_FACTSHEET_TYPE || $node_type == ISA_VIDEO_TYPE
              || $node_type == ISA_NEWS_TYPE || $node_type == ISA_BLOG_TYPE
              || $node_type == ISA_WIKI_TYPE || $node_type == ISA_FEDERATED_PROJECT_TYPE
              || $node_type == ISA_ISSUE_TYPE || $node_type == ISA_PROJECT_RELEASE_TYPE
              || $node_type == ISA_IMAGE_TYPE || $node_type == ISA_FEDERATED_FORGE_TYPE
              || $node_type == ISA_NEWSLETTER_TYPE || ($node_type == 'people' && is_numeric($alias_parts[1]) && empty($alias_parts[2]))) {
        $vars['title'] = '';
      }
    }
  }

  //set collapsed to fieldset (for filters)
  $vars['filter_collapsed'] = 'collapsed';
  if (arg(2) == 'members' && isset($_GET['field_lastname_value'])) {
    $vars['filter_collapsed'] = '';
  }

  // SGS : enable primary links in isa-icp theme settings
  //precare solution to get the active menu, before we find why the $vars['primary_links'] is empty
  //$vars['primary_links'] = menu_primary_links();
  //context_get_plugin('reaction', 'menu')->execute($vars);
  joinup_set_active_primary_links($vars);
}

function joinup_set_active_primary_links(&$vars) {
  $path = $_GET['q'];
  $path = explode('/', $path);
  if ($path[0] == 'node' /* && is_numeric($path[1]) */) {
    $nid = variable_get('current_group', isa_toolbox_get_community_nid());
    if ($nid || $_GET['q'] == 'node/add/project-project') {
      if ($nid) {
        $group = node_load($nid);
        $type = strtoupper($group->group_type);
      } else {
        $type = ($_GET['type'] == 'OSS') ? 'SOFTWARE' : 'ASSET';
      }
      $vars['primary_links'][constant('MENU_' . $type)]['attributes']['class'] .= ' active';
    } else {
      $node = menu_get_object();
      if ($path[1] == 'add') {
        $type = $path[2];
      } else {
        $type = $node->type;
      }
      switch ($type) {
        case ISA_NEWS_TYPE:
        case ISA_BLOG_TYPE:
          if (!isset($vars['primary_links'][MENU_NEWS . ' active'])) {
            $vars['primary_links'][MENU_NEWS]['attributes']['class'] .= ' active';
          }
          break;
        case ISA_DOCUMENT_TYPE:
        case ISA_CASE_TYPE:
        case ISA_VIDEO_TYPE:
        case ISA_FACTSHEET_TYPE:
          if (!isset($vars['primary_links'][MENU_ELIBRARY . ' active'])) {
            $vars['primary_links'][MENU_ELIBRARY]['attributes']['class'] .= ' active';
          }
          break;
        case ISA_EVENT_TYPE:
          if (!isset($vars['primary_links'][MENU_EVENT . ' active'])) {
            $vars['primary_links'][MENU_EVENT]['attributes']['class'] .= ' active';
          }
          break;
        //wiki not in a group is a license wizard
        case ISA_WIKI_TYPE:
          if (!isset($vars['primary_links'][MENU_SOFTWARE . ' active'])) {
            $vars['primary_links'][MENU_SOFTWARE]['attributes']['class'] .= ' active';
          }
          break;
      }
    }
  }
}

/**
 * Theme function for rendering the relevant nodes into a block.
 *
 * This is provided so that an item list is the default, however a themer can
 * easily override this to make a teaser list or table.
 *
 * @param $nodes
 *   Associative array where the key is the node id and the value is the node title
 * @param $header
 *   Optional string to display at the top of the block
 */
function joinup_relevant_content_block($nodes, $header=FALSE, $delta = NULL) {
  $output = "";
  $output .= "<div class='right-sidebar-colspans-5'>";
  $output .= "<div class='view-content'>";
  $i = 0;
  foreach ($nodes as $node) {
    $node = node_load($node['nid']);
    if ($i++ % 2 == 0) {
      $output .= "<div class='odd clearfix'>";
    } else {
      $output .= "<div class='even clearfix'>";
    }

    // print a div around "created | country"
    isa_toolbox_get_user_country($node);
    $output .= "<div class='colspan-5'>";
    $output .= "<span class='field field-created'>" . date('d F Y', $node->created) . "</span>";
    if (isset($node->country) && $node->country != '') {
      $output .= " | ";
      $output .= "<span class='field field-country'>" . $node->country . "</span>";
    }
    $output .= "</div>";

    // print node title with a link to the node
    $output .= "<div class='colspan-5 field field-title'>";
    $output .= "<strong>" . l($node->title, 'node/' . $node->nid) . "</strong>";
    $output .= "</div>";

    $output .= "</div>";
  }
  $output .= "</div>";
  $output .= "</div>";
  return $output;
}

/**
 * Override or insert variables into the "block.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 * @see block.tpl.php
 * @ingroup phptemplate
 */
function joinup_preprocess_block(&$vars) {
  // Create the block object.
  $block = $vars['block'];

  // change the menu for virtual forges
  if ($vars['block']->delta == 'primary-links') {
    if (isa_toolbox_is_in_virtual_forge()) {
      $vars['block']->content = menu_tree('menu-vf-primary-links');
    }
  }

  // Changes title for Multistep block to the node help field
  if ($block->module == 'multistep') {
    $type = $block->delta;
    $type = node_get_types('type', $type);
    $block->subject = strip_tags($type->help);
  }

  $vars['block_class'] = '';
  $vars['accessibility_class'] = '';
  // Set the block class depending on the content type.
  if ($block->subject) {
    $vars['block_class'] = str_replace(array(" ", "'"), array('-', ''), strtolower($block->subject));
  }
  // Set the accessibility class depending on the block type.
  if ($block->module == 'search' || $block->module == 'user' || $block->module == 'menu') {
    $vars['accessibility_class'] = ' class="accessibility-info"';
  }
  // Change block menu news content
  if ($block->module == 'menu' && $block->delta == 'menu-news-menu') {
    $block->content = drupal_get_form('joinup_menu_news_form');
  }
  // Change block menu news content
  if ($block->module == 'menu' && $block->delta == 'menu-elibrary') {
    $block->content = drupal_get_form('joinup_menu_elibrary_form');
  }

  //Change title of block related content
  if ($block->subject == 'Relevant Content') {
    $block->subject = strip_tags('Related Content');
  }

  if ($block->module == 'apachesolr_search') {
    $vars['block_class'] = 'block_apachesolr_search';
  }

  // Set the block heading depending on the homepage and several regions.
  ($block->region == 'highlight' || $block->region == 'header') ? $vars['heading_type'] = 'h2' : $vars['heading_type'] = 'h3';
}

/**
 * Override or insert variables into the "node.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 * @see node.tpl.php
 * @see template.node.inc
 * @ingroup phptemplate
 */
function joinup_preprocess_node(&$vars) {
  $theme = variable_get('theme_default', NULL);
  $theme_path = drupal_get_path('theme', $theme);
  include_once $theme_path . '/includes/template.node.inc';
  _joinup_preprocess_node($vars);
}

/**
 * Return a themed breadcrumb trail.
 *
 * @param
 *   An array containing the breadcrumb links.
 *
 * @return
 *   A string containing the breadcrumb output.
 *
 * @see page.tpl.php
 */

/**
 * Override theme_breadcrumb().
 */
function joinup_breadcrumb($breadcrumb) {
  global $user;
  // if you cannot find a path in the menu router, then try to add some suffixes
  $suffixes = array('all', 'home');
  $primary_links = menu_primary_links();
  $breadcrumb = array();
  $normal_path = trim($_GET['q'], '/');
  $normal_path_parts = explode('/', $normal_path);
  $normal_path_begin = implode('/', array($normal_path_parts[0],
      $normal_path_parts[1]));
  $path_alias = drupal_get_path_alias($normal_path);
  if (strpos($path_alias, 'node/') === 0
          || strpos($path_alias, 'user/') === 0) {
    $path_alias_begin = drupal_get_path_alias($normal_path_begin);
    $path_alias_end = implode('/', array_slice($normal_path_parts, 2));
    if ($path_alias_end == '') {
      $path_alias = $path_alias_begin;
    } else {
      $path_alias = implode('/', array($path_alias_begin, $path_alias_end));
    }
  } else {
    $path_alias = drupal_get_path_alias($normal_path);
  }
  $path_alias_parts = explode('/', $path_alias);

  //Cases of My page / people
  if ($path_alias_parts[0] == 'people') {
    if ($path_alias_parts[1] == 'dashboard' ||
            $path_alias_parts[1] == $user->uid) {
      $path_alias_parts[0] = 'people/mypage';
    }
  }
  // $i is the number of path parts we use for the current breadcrumb path
  $i = 0;
  $last = count($path_alias_parts) - 1;
  // foreach breadcrumb path, generated the corresponding title and link
  foreach ($path_alias_parts as $key => $value) {
    ++$i;
    $part_title = '';
    $part_path = implode('/', array_slice($path_alias_parts, 0, $i));
    if ($value == 'page') {
      --$last;
      continue;
    }
    //$part_path = 'people/mypage';
    // try to find a item in the menu router
    $menu_item = menu_get_item($part_path);

    // try to find a item with a suffix in the menu router
    if (!$menu_item) {
      $modified = false;
      foreach ($suffixes as $key2 => $value2) {
        $menu_item = menu_get_item($part_path . '/' . $value2);
        //specific case for News & blogs in groups
        $menu_item2 = menu_get_item($part_path . 'andblog/' . $value2);
        if ($menu_item) {
          $part_path .= '/' . $value2;
          $modified = true;
          break;
        } elseif ($menu_item2) {
          $part_path .= 'andblog/' . $value2;
          $part_title = 'News & Blogs';
          $modified = true;
          break;
        }
      }
      // case for federated project
      // federated forge >> federated forge title >> federated project title
      if ($path_alias_parts[0] == 'federated_project') {

        $node = menu_get_object();
        if ($value == 'federated_project') {
          $federated_forge = node_load($node->field_fed_project_forge[0]['nid']);
          $breadcrumb[] = l('Federated forges', 'software/federated_forge');
          $breadcrumb[] = l($federated_forge->title, $federated_forge->path);
          ++$last;
          continue;
        } elseif ($value == 'description') {
          $part_path = $node->path;
        }
      }
      // case for document and wiki in a group:
      // group >> group name >> Elibrary >> item title
      if (($value == "document" || $value == "wiki") && isset($path_alias_parts[2]) && ($path_alias_parts[2] == 'wiki' || $path_alias_parts[2] == 'document')) {
        $group_types = array(ISA_COMMUNITY_TYPE, ISA_SOFTWARE_TYPE, ISA_ASSET_TYPE);
        if (in_array($path_alias_parts[0], $group_types)) {
          $node = menu_get_object();
          $group = node_load(array_shift($node->og_groups));
          $short_name = isa_links_get_group_short_name($group);
          $breadcrumb[] = l('e-Library', "{$group->group_type}/{$short_name}/elibray/all");
          continue;
        }
      }
      // case for blogs :
      // home >> blog >> blog_title >> edit/delete
      if ($value == 'blog') {
        $node = menu_get_object();
        $breadcrumb[] = l('Blogs', "news/blog");
        continue;
      }
      // case for topic :
      // forums >> forum name >> topic item
      if ($value == 'topic') {
        $node = menu_get_object();
        $group = node_load(array_shift($node->og_groups));
        $short_name = isa_links_get_group_short_name($group);
        $breadcrumb[] = l('Forums', "{$group->group_type}/{$short_name}/forum/all");
        foreach ($node->taxonomy as $key => $taxo) {
          if ($taxo->vid == variable_get('forum_vid', -1)) {
            $breadcrumb[] = l("{$taxo->name}", "{$group->group_type}/{$short_name}/forum/{$taxo->name}");
            $last++;
          }
        }
        continue;
      }
      // case when view/delete/edit group :
      // group >> group title >> description (>> edit/delete)
      // if no define part title here the breadcrumb display group >> group title >> group title
      $part_path_exp = explode('/', $part_path);
      if ($part_path_exp[2] == 'description' && !isset($part_path_exp[3]) && $part_path_exp[0] != 'federated_project') {
        $part_title = ucfirst($value);
      }
      // check the path is not in the next part of breadcrumb
      if ($modified && $i > 1 && $part_path == implode('/', array_slice($path_alias_parts, 0, $i + 1))) {
        --$last;
        continue;
      }
      // check the path is valid (user/% is valid
      // even if there is no people/% entry in the menu router)
      if (!$menu_item && $i != $last && !(menu_get_item(drupal_get_normal_path($part_path)))) {
        if (//($normal_path_parts[2] == 'delete' || $normal_path_parts[2] == 'edit')&&
                ($value == 'edit' || $value == 'delete' || $value == 'workflow')) {
          //$part_title = ucfirst($normal_path_parts[2]);
          //$part_path = $_GET['q'];
        } else {
          --$last;
          continue;
        }
      }
    }
    // use the primary links titles for the first part of breadcrumb
    if ($i == 1) {
      foreach ($primary_links as $key3 => $value3) {
        if ($value3['href'] == $part_path) {
          $part_title = $value3['title'];
          break;
        }
      }
      // manage pages beginning with 'admin' and similar
      if ($part_title == '')
        $part_title = ucfirst($path_alias_parts[0]);
    }
    else {
      if ($value == 'home') {
        //groups homepage
        $breadcrumb[] = l(ucfirst($path_alias_parts[1]), $path_alias);
        $last++;
      } else {
        // if a menu item exists, try to get a view title
        if ($menu_item) {
          $part_title = isa_toolbox_get_view_title($menu_item);
        }
        $menu_item = menu_get_item(drupal_lookup_path('source', $part_path));
// otherwise, get the page title from the path and add a capital letter
        if ($menu_item && $part_title == '') {
          if ($menu_item['title'] == 'European Union Public Licence (EUPL)') {
            // only display 'eupl', else it's too long for breadcrumb
            $part_title = 'EUPL';
          } elseif ($part_title == '') {
            $part_title = $menu_item['title'];
          }
        } elseif ($part_title == '') {
          $part_title = ucfirst($value);
        }
      }
    }
    $breadcrumb[] = l($part_title, $part_path);
  }
  //'node add' cases to not display Node >> Add in the breadcrumb
  if ($path_alias_parts[0] == 'node' && $path_alias_parts[1] == 'add') {
    $breadcrumb = array();
    $nid = $_GET['gids'][0];
    if (!$nid) {
      $nid = variable_get('current_group', isa_toolbox_get_community_nid());
    }
    if ($nid) {
      $node = node_load($nid);
      foreach ($primary_links as $key3 => $value3) {
        if ($value3['href'] == "{$node->group_type}/all") {
          $part_title = $value3['title'];
          break;
        }
      }
      $breadcrumb[] = l(ucfirst($part_title), "{$node->group_type}/all");
      $short_name = isa_links_get_group_short_name($node);
      $breadcrumb[] = l(ucfirst($short_name), "{$node->group_type}/{$short_name}/home");
    } else {
      $breadcrumb[$last] = $part_title;
    }
  } elseif ($path_alias_parts[0] == 'toboggan') {
    array_pop($breadcrumb);
    $last--;
  }

  if (count($breadcrumb) > 1) {
    //unlink all admin paths if user is not administrator
    if ($normal_path_parts[0] == 'admin' && !isa_toolbox_is_omnipotent()) {
      foreach ($breadcrumb as $key => $item) {
        $breadcrumb[$key] = strip_tags($item);
      }
    } elseif ($normal_path_parts[0] == 'user') {
      //if on a user/* page, set the last breadcrumb part to firstname and lastname
      if (is_numeric($normal_path_parts[1])) {
        if (!isset($normal_path_parts[2])) {
          global $user;
          //throw out the uid from the breadcrumb
          if ($user->uid != $normal_path_parts[1]) {
            unset($breadcrumb[$last]);
          }
        } elseif ($normal_path_parts[2] == 'contact') {
          $breadcrumb[$last] = t('Contact');
        } elseif ($normal_path_parts[2] == 'edit') {
          if (isset($normal_path_parts[3])) {
            unset($breadcrumb[$last]);
            --$last;
            if ($normal_path_parts[3] == 'profile') {
              $breadcrumb[$last] = t('Edit profile');
            } elseif ($normal_path_parts[3] == 'newsletter') {
              $breadcrumb[$last] = t('My newsletters');
            }
          } else {
            $breadcrumb[$last] = t('Edit account');
          }
        }
        $breadcrumb[1] = theme('username', $normal_path_parts[1]);
      }
    } elseif ($normal_path_parts[0] == 'people' && is_numeric($normal_path_parts[1])) {
      //replace uid by user firstname & lastname
      $breadcrumb[1] = theme('username', $normal_path_parts[1]);
    } else {
      // if drupal_get_title tells us about a more precise title, then use it
      $drupal_title = drupal_get_title();
      if ($drupal_title != '') {
        if (isset($path_alias_parts[2]) && $path_alias_parts[2] != 'description'
                && $normal_path_parts[2] != 'edit' && $normal_path_parts[2] != 'delete' && $normal_path_parts[2] != 'workflow') {
          $breadcrumb[$last] = $drupal_title;
        }
      }
    }

    array_unshift($breadcrumb, l('Home', 'homepage'));

    $breadcrumb[count($breadcrumb) - 1] = strip_tags($breadcrumb[count($breadcrumb) - 1]);
  } else {
    $breadcrumb = array();
    $breadcrumb[0] = l('Home', 'homepage');
    $drupal_title = drupal_get_title();
    if ($drupal_title != '') {
      $breadcrumb[1] = $drupal_title;
    }
  }
  return implode(' &raquo; ', $breadcrumb);
}

/**
 * Theme preprocess function for field.tpl.php.
 *
 * The $variables array contains the following arguments:
 * - $node
 * - $field
 * - $items
 * - $teaser
 * - $page
 *
 * @see field.tpl.php
 *
 * TODO : this should live in theme/theme.inc, but then the preprocessor
 * doesn't get called when the theme overrides the template. Bug in theme layer ?
 */
function joinup_preprocess_content_field(&$variables) {
  $element = $variables['element'];
  $field = content_fields($element['#field_name'], $element['#node']->type);

  $variables['node'] = $element['#node'];
  $variables['field'] = $field;
  $variables['items'] = array();

  if ($element['#single']) {
    // Single value formatter.
    foreach (element_children($element['items']) as $delta) {
      $variables['items'][$delta] = $element['items'][$delta]['#item'];
      // Use isset() to avoid undefined index message on #children when field values are empty.
      $variables['items'][$delta]['view'] = isset($element['items'][$delta]['#children']) ? $element['items'][$delta]['#children'] : '';
    }
  } else {
    // Multiple values formatter.
    // We display the 'all items' output as $items[0], as if it was the
    // output of a single valued field.
    // Raw values are still exposed for all items.
    foreach (element_children($element['items']) as $delta) {
      $variables['items'][$delta] = $element['items'][$delta]['#item'];
    }
    $variables['items'][0]['view'] = $element['items']['#children'];
  }

  $variables['teaser'] = $element['#teaser'];
  $variables['page'] = $element['#page'];

  $field_empty = TRUE;

  foreach ($variables['items'] as $delta => $item) {
    if (!isset($item['view']) || (empty($item['view']) && (string) $item['view'] !== '0')) {
      $variables['items'][$delta]['empty'] = TRUE;
    } else {
      $field_empty = FALSE;
      $variables['items'][$delta]['empty'] = FALSE;
    }
  }

  $ok = TRUE;

//  if ($element['#field_name']){
//      if (in_array($element['#field_name'], $element['#node']->show_email['fields'])) {
//        $ok = $element['#node']->show_email['check'];
//      }
//
//      if (in_array($element['#field_name'], $element['#node']->show_organization['fields'])) {
//        $ok = $element['#node']->show_organization['check'];
//      }
//
//      if (in_array($element['#field_name'], $element['#node']->show_address['fields'])) {
//        $ok = $element['#node']->show_organization['check'];
//        if ($ok)
//          $ok = $element['#node']->show_address['check'];
//      }
//
//      if (in_array($element['#field_name'], $element['#node']->show_profile['fields'])) {
//        $ok = $element['#node']->show_profile['check'];
//      }
//
//      $preferences = array('field_email_visibility', 'field_organization_visibility', 'field_address_visibility', 'field_profile_visibility');
//      if (in_array($element['#field_name'], $preferences)) {
//        $ok = FALSE;
//      }
//  }


  if ($ok) {
    $additions = array(
        'field_type' => $field['type'],
        'field_name' => $field['field_name'],
        'field_type_css' => strtr($field['type'], '_', '-'),
        'field_name_css' => strtr($field['field_name'], '_', '-'),
        'label' => check_plain(t($field['widget']['label'])),
        'label_display' => $element['#label_display'],
        'field_empty' => $field_empty,
        'template_files' => array(
            'content-field',
            'content-field-' . $element['#field_name'],
            'content-field-' . $element['#node']->type,
            'content-field-' . $element['#field_name'] . '-' . $element['#node']->type,
        ),
    );
    $variables = array_merge($variables, $additions);
  }
  else
    $variables = array();
}

/**
 * Format a fieldgroup using a 'fieldset'.
 *
 * Derived from core's theme_fieldset, with no output if the content is empty.
 */
function joinup_fieldgroup_fieldset($element) {
  $children = $element['#children'];
  $children = strip_tags($children);
  $children = trim($children);

  if (empty($children))
    $element['#children'] = $children;

  if (empty($element['#children']) && empty($element['#value'])) {
    return '';
  }

  if ($element['#collapsible']) {
    drupal_add_js('misc/collapse.js');

    if (!isset($element['#attributes']['class'])) {
      $element['#attributes']['class'] = '';
    }

    $element['#attributes']['class'] .= ' collapsible';
    if ($element['#collapsed']) {
      $element['#attributes']['class'] .= ' collapsed';
    }
  }
  return '<fieldset' . drupal_attributes($element['#attributes']) . '>' . ($element['#title'] ? '<legend>' . $element['#title'] . '</legend>' : '') . (isset($element['#description']) && $element['#description'] ? '<div class="description">' . $element['#description'] . '</div>' : '') . (!empty($element['#children']) ? $element['#children'] : '') . (isset($element['#value']) ? $element['#value'] : '') . "</fieldset>\n";
}

function joinup_help() {
  $trial = menu_get_active_trail();
  $multiforms = array('news');
  $menu = $trial[1];
  $ok = TRUE;
  //SGS : Warning: Invalid argument supplied for foreach()
  // check if $menu['map'] is set
  if (isset($menu['map'])) {
    foreach ($menu['map'] as $value) {
      if (in_array($value, $multiforms)) {
        $ok = FALSE;
      }
    }
  }

  if ($ok) {
    $help = menu_get_active_help();
    return '<div class="help">' . $help . '</div>';
  }
}

// Function to update appearance of user name (First name + Last name)
function joinup_username($objects, $options = array()) {
  $uid = is_numeric($objects) ? $objects : $objects->uid;

  $myprofile = content_profile_load('profile', $uid);
  if (!$myprofile) {
    // Could it be the anonymous user?
    if ($uid === 0) {
      return variable_get('anonymous', t('Anonymous'));
    }
    // we now need the Drupal username:
    // we avoid user_load because we just need the name and it is rather hard
    // to predict whether or not it will end up loading half the database...
    if (is_numeric($objects)) {
      $name = db_result(db_query('SELECT name FROM {users} WHERE uid = %d', $uid));
    } else {
      $name = $objects->name;
    }
    if (!drupal_strlen(trim($name))) {
      return variable_get('anonymous', t('Anonymous'));
    }
    return $name;
  } else {
    $username = ucfirst("{$myprofile->field_firstname[0]['value']} ")
            . strtoupper($myprofile->field_lastname[0]['value']);
    //$maxlen = 16;
    //if (strlen($username) > $maxlen)
    //  $username = substr($username, 0, $maxlen - 3) . "...";
    return l($username, "user/{$uid}");
  }
}

// Definition of the select form for news menu
function joinup_menu_elibrary_form($form_state) {
  $menu_items_news = menu_tree_all_data('menu-elibrary');
  foreach ($menu_items_news as $key => $value) {
    $menu_news[$value['link']['link_path']] = $value['link']['link_title'];
  }

  $form['list_menu'] = array(
      '#type' => 'select',
      '#title' => t('Content to display'),
      '#options' => $menu_news,
  );
  $form['list_menu']['#default_value'] = arg(0) . '/' . arg(1);

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Apply')
  );

  return $form;
}

// Manage submit button for news menu
function joinup_menu_elibrary_form_submit($form, &$form_state) {
  $res = $form_state['values']['list_menu'];
  drupal_goto($res);
}

// Definition of the select form for news menu
function joinup_menu_news_form($form_state) {
  $menu_items_news = menu_tree_all_data('menu-news-menu');
  foreach ($menu_items_news as $key => $value) {
    $menu_news[$value['link']['link_path']] = $value['link']['link_title'];
  }

  $form['list_menu'] = array(
      '#type' => 'select',
      '#title' => t('Content to display'),
      '#options' => $menu_news,
  );
  $form['list_menu']['#default_value'] = arg(0) . '/' . arg(1);

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Apply')
  );

  return $form;
}

// Manage submit button for news menu
function joinup_menu_news_form_submit($form, &$form_state) {
  $res = $form_state['values']['list_menu'];
  drupal_goto($res);
}

/**
 * Implementation of theme_preprocess_comment
 */
function joinup_preprocess_comment(&$vars) {
  $uid = $vars['comment']->uid;
  $profile = content_profile_load('profile', $uid);
  $picture = isa_toolbox_picture_fix($profile, 'profile_photo_small');
  $poster = user_load($uid);
  $name = joinup_username($poster);

  $created = date('F d, Y \a\t G:i', $vars['comment']->timestamp);
  $vars['comment']->created = $created;
  $vars['comment']->picture = $picture;
  $vars['comment']->name = $name;
  $vars['user_company_name'] = $profile->field_company_name['0']['value'];


// set the taxonomy term for users
  ///joinup_set_terms_in_vars($vars, $profile);
  isa_toolbox_create_taxonomy_list($profile, array(variable_get('country_vid', NULL)));
  $country = taxonomy_vocabulary_load(variable_get('country_vid', NULL));
  $vars['user_countries'] = $profile->taxonomy_terms[$country->name];


  $groups = $vars['node']->og_groups;
  $group_id = array_shift(array_keys($groups));
  if (og_is_group_member($group_id, FALSE, $uid)) {
    $user = user_load($uid);
    $group = node_load($group_id);
    if ($group->uid == $uid) {
      $joined_group = $group->created;
    } else {
      $joined_group = $user->og_groups[$group_id]['created'];
    }
    $vars['joined_group'] = date("d/m/Y", $joined_group);
  }
}

/**
 * Override or insert variables into the "box.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 * @see box.tpl.php
 * @ingroup phptemplate
 */
function joinup_preprocess_box(&$vars) {
  // Get the node type using the page URL to create a new template file called "box-topic.tpl.php".
  if (module_exists('path')) {
    $path_alias = drupal_get_path_alias($_GET['q']);
    $alias_parts = explode('/', $path_alias);
    $alias_parts_reverse = array_reverse($alias_parts);
    if ($alias_parts_reverse[1]) {
      $node_type = $alias_parts_reverse[1];
    }
    if ($node_type == 'topic') {
      $vars['template_file'] = 'box-' . $node_type;
      $uid = $vars['user']->uid;
      $profile = content_profile_load('profile', $uid);
      $picture = isa_toolbox_picture_fix($profile, 'profile_photo_small');
      $poster = user_load($uid);
      //joinup_set_terms_in_vars($vars, $profile);
      isa_toolbox_create_taxonomy_list($profile, array(variable_get('country_vid', NULL)));
      $country = taxonomy_vocabulary_load(variable_get('country_vid', NULL));
      $vars['user_countries'] = $profile->taxonomy_terms[$country->name];
      $vars['user_picture'] = $picture;
      $vars['user_name'] = joinup_username($poster);
      $vars['user_company_name'] = $profile->field_company_name['0']['value'];
      $vars['title'] = t('Post new reply');
      $group_id = isa_toolbox_get_community_nid();
      if (og_is_group_member($group_id, FALSE, $uid)) {
        $user = user_load($uid);
        $group = node_load($group_id);
        if ($group->uid == $uid) {
          $joined_group = $group->created;
        } else {
          $joined_group = $user->og_groups[$group_id]['created'];
        }
        $vars['joined_group'] = date("d/m/Y", $joined_group);
      }
    } elseif ($node_type == 'issue') {
      $vars['template_file'] = 'box-project_' . $node_type;
      $uid = $vars['user']->uid;
      $profile = content_profile_load('profile', $uid);
      $picture = isa_toolbox_picture_fix($profile, 'profile_photo_small');
      $poster = user_load($uid);
      $vars['user_picture'] = $picture;
      $vars['user_name'] = joinup_username($poster);
    } else {
      $uid = $vars['user']->uid;
      $profile = content_profile_load('profile', $uid);
      $picture = isa_toolbox_picture_fix($profile, 'profile_photo_small');
      $vars['user_picture'] = $picture;
    }
  }
}

/**
 * Override theme_apachesolr_breadcrumb_uid to return the user firstname + lastname instead of username
 * @param <numeric> $uid
 * @return <string> formated user firstname & lastname
 */
function joinup_apachesolr_breadcrumb_uid($uid) {
  return strip_tags(theme('username', $uid));
}

/**
 * Override or insert variables into the "simplenews-newsletter-body.tpl.php" templates.
 *
 * @param $vars
 *   An array of variables to pass to the theme template.
 *
 */
function joinup_preprocess_simplenews_newsletter_body(&$variables) {
  $content = variable_get('newsletters_mail_body', '...');
  $content = token_replace($content, 'node', $variables['node']);
  $variables['content'] = $content;
}

function joinup_preprocess_simplenews_newsletter_footer(&$variables) {
  $content = variable_get('newsletters_mail_footer', '...');
  if ($variables['key'] == 'test') {
    $content .= '- - -' . t('This is a test version of the newsletter.', array(), $variables['language']->language) . '- - -';
  }
  $content = token_replace($content, 'node', $variables['node']);
  $variables['content'] = $content;
}

/*
 * Implementation of theme_form_element
 * Deleted the colon ':' after elements title
 */

function joinup_form_element($element, $value) {
  // This is also used in the installer, pre-database setup.
  $t = get_t();

  $output = '<div class="form-item"';
  if (!empty($element['#id'])) {
    $output .= ' id="' . $element['#id'] . '-wrapper"';
  }
  $output .= ">\n";
  $required = !empty($element['#required']) ? '<span class="form-required" title="' . $t('This field is required.') . '">*</span>' : '';

  if (!empty($element['#title'])) {
    $title = $element['#title'];
    if (!empty($element['#id'])) {
      $output .= ' <label for="' . $element['#id'] . '">' . $t('!title !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    } else {
      $output .= ' <label>' . $t('!title !required', array('!title' => filter_xss_admin($title), '!required' => $required)) . "</label>\n";
    }
  }

  $output .= " $value\n";

  if (!empty($element['#description'])) {
    $output .= ' <div class="description">' . $element['#description'] . "</div>\n";
  }

  $output .= "</div>\n";

  return $output;
}

function joinup_comment_submitted($comment) {
  return t('Posted by !username on @datetime', array(
      '!username' => theme('username', $comment),
      '@datetime' => format_date($comment->timestamp, 'custom', 'F d, Y \a\t G:i'),
  ));
}

function joinup_node_submitted($node) {
  if ($node->type == ISA_TOPIC_TYPE) {
    return t('Posted by !username on @datetime', array(
        '!username' => theme('username', $node),
        '@datetime' => format_date($node->created, 'custom', 'F d, Y \a\t G:i'),
    ));
  } elseif ($node->type == ISA_FEDERATED_PROJECT_TYPE) {
    return t('Created on @datetime', array(
        '@datetime' => format_date($node->created, 'custom', 'F d, Y'),
    ));
  } else {
    return t('Submitted by !username on @datetime', array(
        '!username' => theme('username', $node),
        '@datetime' => format_date($node->created, 'custom', 'F d, Y'),
    ));
  }
}

/**
 * Theme a "you can't post comments" notice.
 *
 * @param $node
 *   The comment node.
 * @ingroup themeable
 */
function joinup_comment_post_forbidden($node) {
  global $user;
  static $authenticated_post_comments;

  if (!$user->uid) {
    if (!isset($authenticated_post_comments)) {
      // We only output any link if we are certain, that users get permission
      // to post comments by logging in. We also locally cache this information.
      $authenticated_post_comments = array_key_exists(DRUPAL_AUTHENTICATED_RID, user_roles(TRUE, 'post comments') + user_roles(TRUE, 'post comments without approval'));
    }

    if ($authenticated_post_comments) {
      // We cannot use drupal_get_destination() because these links
      // sometimes appear on /node and taxonomy listing pages.
      if (variable_get('comment_form_location_' . $node->type, COMMENT_FORM_SEPARATE_PAGE) == COMMENT_FORM_SEPARATE_PAGE) {
        $destination = 'destination=' . rawurlencode("comment/reply/$node->nid#comment-form-title");
      } else {
        $destination = 'destination=' . rawurlencode("node/$node->nid#comment-form-title");
      }

      if (variable_get('user_register', 1)) {
        // Users can register themselves.
        return t('<a id="login-required" href="@login">Login</a> or <a id="register-required" href="@register">register</a> to post comments', array('@login' => url('user/login', array('query' => $destination)), '@register' => url('user/register', array('query' => $destination))));
      } else {
        // Only admins can add new users, no public registration.
        return t('<a href="@login">Login</a> to post comments', array('@login' => url('user/login', array('query' => $destination))));
      }
    }
  }
}

function joinup_prepare_vote_rating($node) {
  $vote = fivestar_get_votes('node', $node->nid);
  $rate = $vote['average']['value'] / 20;
  $tot_vote = $vote['count']['value'];
  $tot_vote = isset($tot_vote) ? $tot_vote : 0;
  return t('Rating:<strong> ') . $rate . '/5 </strong> (' . t('based on ') . $tot_vote . t(' votes') . ')';
}

function joinup_prepare_document_attachement($field) {
  global $base_url;
  foreach ($field as $key => $value) {
    if (!empty($value['view'])) {
      $class = substr($value['filename'], (strrpos($value['filename'], '.') + 1));
      $href = file_create_url($value['filepath']);
      $text = $value['filename'];
      $weight = isa_toolbox_get_size_formatted($value['filesize']);
      $title = $value['data']['description'];

      $documents[$key] = l($text, $href, array('html' => TRUE, 'attributes' => array('class' => 'document ' . $class)));
      if ($text) {
        $documents[$key].= '<span>(' . $weight . ')</span><br />';
      }
      if ($title) {
        $documents[$key].= '<span>' . $title . '</span>';
      }
    }
  }
  return $documents;
}




/**
 * if the user has not permission Administer User override theme user administration overview.
 *
 */
function joinup_user_admin_account($form) {
  if (!user_access('administer users') && user_access('manage users')) {

    // remove Operations column
    unset($form['operations']);

    // Overview table:
    $header = array(
        theme('table_select_header_cell'),
        array('data' => t('Username'), 'field' => 'u.name'),
        array('data' => t('Status'), 'field' => 'u.status'),
        t('Roles'),
        array('data' => t('Member for'), 'field' => 'u.created', 'sort' => 'desc'),
        array('data' => t('Last access'), 'field' => 'u.access'),
            //t('Operations')
    );

    $output = drupal_render($form['options']);
    if (isset($form['name']) && is_array($form['name'])) {

      foreach (element_children($form['name']) as $key) {
        $rows[] = array(
            drupal_render($form['accounts'][$key]),
            drupal_render($form['name'][$key]),
            drupal_render($form['status'][$key]),
            drupal_render($form['roles'][$key]),
            drupal_render($form['member_for'][$key]),
            drupal_render($form['last_access'][$key]),
            drupal_render($form['operations'][$key]),
        );
      }
    } else {
      $rows[] = array(array('data' => t('No users available.'), 'colspan' => '7'));
    }

    $output .= theme('table', $header, $rows);
    if ($form['pager']['#value']) {
      $output .= drupal_render($form['pager']);
    }

    $output .= drupal_render($form);

    return $output;
  } elseif (user_access('administer users')) {
    return theme_user_admin_account($form);
  }
}



