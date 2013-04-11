<?php
// $Id: project.inc,v 1.147 2009/11/25 16:38:15 dww Exp $

/**
 * Implementation of hook_form().
 */
function project_project_form($node, $form_state) {
  global $user;
  project_project_set_breadcrumb($node);

  /* Project taxonomy */
  if (project_use_taxonomy()) {
    // Since this form is used relatively infrequently, don't allow the js to be aggregated.
    drupal_add_js(drupal_get_path('module', 'project') .'/project.js', 'module', 'header', FALSE, TRUE, FALSE);
    $project_type_vid = _project_get_vid();
    $tree = taxonomy_get_tree($project_type_vid);
    $top_level = array();
    $options = array();
    foreach ($tree as $i => $term) {
      if ($term->parents[0] == 0) {
        $last_top = $term->tid;
        $top_level[$term->tid] = check_plain($term->name);
      }
      else {
        $options[$last_top][$term->tid] = $term->name;
      }
    }
    // See if there are any project specific taxonomy terms already
    // saved in this node (i.e. we're editing an existing project) and
    // if so, extract the right default values for our custom form
    // elements...
    $current_top = NULL;
    $current_options = array();

    if (!empty($node->taxonomy)) {
      // Depending on whether we're previewing the node or not,
      // $node->taxonomy will be provided in one of two ways.
      if (isset($form_state['node_preview'])) {
        // In node previews, $node->taxonomy is an array of vocabularies,
        // each of which is an array of selected tids.
        if (isset($node->taxonomy[$project_type_vid])) {
          foreach ($node->taxonomy[$project_type_vid] as $key => $value) {
            if (isset($top_level[$key])) {
              $current_top = $key;
            }
            else {
              $current_options[$key] = $key;
            }
          }
        }
      }
      else {
        // $node->taxonomy is an array of term objects
        // when we're not previewing the node.
        foreach ($node->taxonomy as $tid => $obj) {
          if (isset($top_level[$tid])) {
            $current_top = $tid;
          }
          else {
            $current_options[$tid] = $tid;
          }
        }
      }
    }

    $form['project_taxonomy'] = array(
      '#type' => 'fieldset',
      '#weight' => '-30',
      '#title' => t('Project categories'),
      '#collapsible' => TRUE,
      '#theme' => 'project_project_node_form_taxonomy',
    );

    $form['project_taxonomy']['project_type'] = array(
      '#title' => t('Project type'),
      '#type' => 'radios',
      '#options' => $top_level,
      '#default_value' => $current_top,
      '#required' => TRUE,
    );
    $select_size = max(5, 2*count($top_level));
    foreach ($options as $tid => $values) {
      $form['project_taxonomy']["tid_$tid"] = array(
        '#title' => t('!type categories', array('!type' => $top_level[$tid])),
        '#type' => 'select',
        '#multiple' => TRUE,
        '#options' => $values,
        '#default_value' => $current_options,
        '#attributes' => array('size' => min($select_size, count($values))),
      );
    }
  }

  /* Project properties */
  // We can't put the title and body inside $node->project or core gets
  // confused (e.g node_body_field() and friends).  So, we put the core node
  // fields in their own fieldset (for which is #tree is FALSE).
  $form['project_node'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project information'),
    '#collapsible' => TRUE,
  );
  $form['project_node']['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Full project name'),
    '#default_value' => isset($node->title) ? $node->title : NULL,
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  // This is sort of a hack: We want the 'uri' to be in the $node->project
  // array during validation and submission of this form, to protect the $node
  // namespace, even though from a usability standpoint, this field belongs
  // right up next to the title.  So, we add a 'project' subarray in here
  // for which #tree is TRUE, and put the 'uri' field in there.  That way, it
  // still lives inside the "Project information" fieldset as far as the UI is
  // concerned, but the value shows up in the $node->project array for
  // validation and submission as far as FAPI is concerned.
  $form['project_node']['project'] = array('#tree' => TRUE);
  $form['project_node']['project']['uri'] = array(
    '#type' => 'textfield',
    '#title' => t('Short project name'),
    '#default_value' => isset($node->project['uri']) ? $node->project['uri'] : NULL,
    '#size' => 40,
    '#maxlength' => 50,
    '#description' => (variable_get('project_enable_alias', TRUE)) ? t('This will be used to generate a /project/&lt;shortname&gt;/ URL for your project.') : '',
    '#required' => TRUE,
  );
  $form['project_node']['body_field'] = node_body_field($node, t('Full description'), 1);

  $form['project'] = array(
    '#type' => 'fieldset',
    '#title' => t('Project resources'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => TRUE,
  );
  $form['project']['homepage'] = array(
    '#type' => 'textfield',
    '#title' => t('Homepage'),
    '#default_value' => isset($node->project['homepage']) ? $node->project['homepage'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to project homepage.'),
  );
  $form['project']['documentation'] = array(
    '#type' => 'textfield',
    '#title' => t('Documentation'),
    '#default_value' => isset($node->project['documentation']) ? $node->project['documentation'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to project documentation.'),
  );
  $form['project']['license'] = array(
    '#type' => 'textfield',
    '#title' => t('License'),
    '#default_value' => isset($node->project['license']) ? $node->project['license'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to project license.'),
  );
  $form['project']['screenshots'] = array(
    '#type' => 'textfield',
    '#title' => t('Screenshots'),
    '#default_value' => isset($node->project['screenshots']) ? $node->project['screenshots'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to project screenshots.'),
  );
  $form['project']['changelog'] = array(
    '#type' => 'textfield',
    '#title' => t('Changelog'),
    '#default_value' => isset($node->project['changelog']) ? $node->project['changelog'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to changelog.'),
  );
  $form['project']['cvs'] = array(
    '#type' => 'textfield',
    '#title' => t('CVS tree'),
    '#default_value' => isset($node->project['cvs']) ? $node->project['cvs'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to webcvs/viewcvs.'),
  );
  $form['project']['demo'] = array(
    '#type' => 'textfield',
    '#title' => t('Demo site'),
    '#default_value' => isset($node->project['demo']) ? $node->project['demo'] : NULL,
    '#size' => 40,
    '#maxlength' => 255,
    '#description' => t('Link to a live demo.'),
  );

  return $form;
}

/**
 * Implementation of hook_validate().
 *
 * @param $node
 *   An object containing values from the project node form.  Note that since
 *   this isn't a fully-loaded $node object, not all values will necessarily
 *   be in the same location as they would after a node_load().
 */
function project_project_validate(&$node) {
  // Bail if user hasn't done a preview yet.
  if (!isset($node->title)) {
    return $node;
  }

  // Make sure title isn't already in use
  if (db_result(db_query("SELECT COUNT(*) FROM {node} WHERE type = '%s' AND status = %d AND title = '%s' AND nid <> %d", $node->type, 1, $node->title, $node->nid))) {
    form_set_error('title', t('This project name is already in use.'));
  }

  // Validate uri.
  if (empty($node->project['uri'])) {
    form_set_error('project][uri', t('A short project name is required.'));
  }
  else {
    // Make sure uri only includes valid characters
    if (!preg_match('/^[a-zA-Z0-9_-]+$/', $node->project['uri'])) {
      form_set_error('project][uri', t('Please only use alphanumerical characters for the project name.'));
    }

    // Make sure uri isn't already in use, or reserved.  Includes all X from
    // project/issues/X paths used in project_issues module
    $reserved_names = array('user', 'issues', 'releases', 'rss', 'subscribe-mail', 'search', 'add', 'update_project', 'statistics', 'comments', 'autocomplete', 'cvs', 'developers', 'usage');
    if (project_use_taxonomy()) {
      $terms = taxonomy_get_tree(_project_get_vid());
      foreach ($terms as $i => $term) {
        if ($term->depth == 0) {
          $reserved_names[] = strtolower($term->name);
        }
      }
    }
    if (in_array(strtolower($node->project['uri']), $reserved_names)) {
      form_set_error('project][uri', t('This project name is reserved.'));
    }
    $existing_nid = project_get_nid_from_uri($node->project['uri']);
    if (!empty($existing_nid) && $existing_nid != $node->nid) {
      form_set_error('project][uri', t('This project name is already in use.'));
    }
  }

  // Make sure all URL fields actually contain URLs.
  $fields = array(
    'homepage' => t('Homepage'),
    'changelog' => t('Changelog'),
    'cvs' => t('CVS tree'),
    'demo' => t('Demo site'),
  );
  foreach ($fields as $uri => $name) {
    if ($node->project[$uri] && !preg_match('/^(http|https|ftp):\/\//i', $node->project[$uri])) {
      form_set_error("project][$uri", t('The %field field is not a valid URL.', array('%field' => $name)));
    }
  }

  // Validate the project-specific sub-categories, if any...
  if (project_use_taxonomy() && isset($node->project_type)) {
    $tree = taxonomy_get_tree(_project_get_vid());
    $top_level = array();
    foreach ($tree as $i => $term) {
      if ($term->parents[0] == 0) {
        $top_level[$term->tid] = $term->name;
      }
    }
    foreach ($top_level as $tid => $name) {
      if ($node->project_type != $tid) {
        $tid_field = 'tid_' . $tid;
        if (!empty($node->$tid_field)) {
          form_set_error($tid, t('Project type %project_type was selected, you can not use values from %invalid_type categories', array('%project_type' => $top_level[$node->project_type], '%invalid_type' => $top_level[$tid])));
        }
      }
    }
  }
}

function project_project_set_breadcrumb($node = NULL, $extra = NULL) {
  $breadcrumb = array();
  $breadcrumb[] = l(t('Home'), NULL);

  /*
    @TODO: This is not an optimal way to do this, because it makes the
    assumption that there is only one menu item that links to /project (or
    whatever it happens to be called).  Also, it makes the assumption that the
    URL alias is intact for the project node (in other words, it's path is
    aliased to 'project/<project short name>' However, since in D6 we no
    longer have $_menu I'm not sure of a better way to do this.
  */
  if (!empty($node->path)) {
    // Get the path of the project node and remove the name of the project.
    $path = array();
    $path = explode('/', $node->path);
    $path = array_slice($path, 0, count($path) - 1);
    $path = implode('/', $path);
    $menu_link = db_fetch_object(db_query("SELECT * FROM {menu_links} ml INNER JOIN {menu_router} m ON m.path = ml.router_path WHERE ml.hidden = %d AND ml.link_path = '%s' ORDER BY ml.weight", 0, $path));
    if (!empty($menu_link)) {
      $breadcrumb[] = l($menu_link->link_title, 'project', array('title' => t('Browse projects')));
    }
  }

  if (!empty($node->nid) && project_use_taxonomy()) {
    $result = db_query(db_rewrite_sql('SELECT t.tid, t.* FROM {term_data} t INNER JOIN {term_hierarchy} h ON t.tid = h.tid INNER JOIN {term_node} r ON t.tid = r.tid WHERE h.parent = %d AND t.vid = %d AND r.vid = %d', 't', 'tid'), 0, _project_get_vid(), $node->vid);
    if ($term = db_fetch_object($result)) {
      $breadcrumb[] = l($term->name, 'project/'. $term->name);
    }
  }

  if (is_array($extra)) {
    $breadcrumb = array_merge($breadcrumb, $extra);
  }
  elseif ($extra && !empty($node)) {
    $breadcrumb[] = l($node->title, 'node/'. $node->nid);
  }

  drupal_set_breadcrumb($breadcrumb);
}

function project_project_view($node, $teaser = false, $page = false) {
  $node = node_prepare($node, $teaser);

  if ($page) {
    // Breadcrumb navigation
    project_project_set_breadcrumb($node);

    // If theme_project_release_project_download_table is implemented, format
    // the download table.  If this function is not implemented (eg. if the
    // project_release module is not enabled), there will not be an error
    // but of course there will be no release table.
    $project_table_output = theme('project_release_project_download_table', $node);
    if (!empty($project_table_output)) {
      $node->content['download_table'] = array(
        '#value' => $project_table_output,
        '#weight' => 1,
      );
    }

    // Retrieve nested array of sections of links to display on node page.
    $all_links = project_get_project_link_info($node);

    // Format links in $all_links for display in the project_project node.
    // Keep track of the section with the heaviest weight (which will be last)
    // so we can add a final clear after it to make sure the floating link
    // sections do not interfere with other formatting in the node's content.
    $max_weight = -10000;
    $last_section = '';
    foreach($all_links as $section => $values) {
      // Only add the section if there are links, and section type is "inline".
      if (!empty($values['links']) && (empty($values['type']) || $values['type'] == 'inline')) {
        $weight = !empty($values['weight']) ? $values['weight'] : 0;
        $node->content[$section] = array(
          '#value' => theme('item_list', $values['links'], isset($values['name']) ? $values['name'] : NULL),
          '#weight' => !empty($values['weight']) ? $values['weight'] : 0,
          '#prefix' => '<div class="project-links-section" id="project-links-section-'. $section .'">',
          '#suffix' => '</div>',
        );
        if (!empty($values['clear'])) {
          $node->content[$section]['#suffix'] .= '<br style="clear:both;" />';
        }
        if ($weight >= $max_weight) {
          $last_section = $section;
          $max_weight = $weight;
        }
      }
    }
    // We only want to add a clearing <br> after the final section if that
    // section didn't already add a clear for itself (e.g. the heaviest
    // section might already clear from hook_project_page_link_alter()).
    if (empty($all_links[$last_section]['clear']) && !empty($last_section)) {
      $node->content[$last_section]['#suffix'] .= '<br style="clear:both;" />';
    }
  }
  return $node;
}

/**
 * hook_nodeapi() implementation specific for project nodes.
 * @see project_nodeapi().
 */
function project_project_nodeapi(&$node, $op, $arg) {
  $language = isset($node->language) ? $node->language : '';
  switch ($op) {
    case 'insert':
      _project_save_taxonomy($node);
      if (module_exists('path') && variable_get('project_enable_alias', TRUE)) {
        path_set_alias("node/$node->nid", 'project/'. $node->project['uri'], NULL, $language);
      }
      break;

    case 'update':
      _project_save_taxonomy($node);
      if (module_exists('path') && variable_get('project_enable_alias', TRUE)) {
        path_set_alias("node/$node->nid");  // Clear existing alias.
        path_set_alias("node/$node->nid", 'project/'. $node->project['uri'], NULL, $language);
      }
      break;
  }
}

function project_project_insert($node) {
  db_query("INSERT INTO {project_projects} (nid, uri, homepage, changelog, cvs, demo, screenshots, documentation, license) VALUES (%d, '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')", $node->nid, $node->project['uri'], $node->project['homepage'], $node->project['changelog'], $node->project['cvs'], $node->project['demo'], $node->project['screenshots'], $node->project['documentation'], $node->project['license']);
//  project_release_scan_directory($node->project['uri']);
}

function project_project_update($node) {
  db_query("UPDATE {project_projects} SET uri = '%s', homepage = '%s', changelog = '%s', cvs = '%s', demo = '%s', screenshots = '%s', documentation = '%s', license = '%s' WHERE nid = %d", $node->project['uri'], $node->project['homepage'], $node->project['changelog'], $node->project['cvs'], $node->project['demo'], $node->project['screenshots'], $node->project['documentation'], $node->project['license'], $node->nid);
//  project_release_scan_directory($node->project['uri']);
}

function project_project_delete($node) {
  db_query('DELETE FROM {project_projects} WHERE nid = %d', $node->nid);
}

function project_project_retrieve($key = 0) {
  if (!is_numeric($key)) {
    $nid = project_get_nid_from_uri($key);
  }
  $node = node_load($nid);
  return ($node->type == 'project_project') ? $node : NULL;
}

function project_cvs($nid = 0) {
  if ($project = node_load($nid)) {
    if (node_access('view', $project)) {
      $_REQUEST['nid'] = $nid;
      $output = module_invoke('cvs', 'show_messages');
      drupal_set_title(t('CVS messages for %name', array('%name' => $project->title)));
      project_project_set_breadcrumb($project, TRUE);
      return $output;
    }
    else {
      drupal_access_denied();
    }
  }
  else {
    drupal_not_found();
  }
}

function _project_save_taxonomy(&$node) {
  if (project_use_taxonomy() && $node->project_type) {
    // First, clear out all terms from the project-specific taxonomy
    // in this node. We'll re-add the right ones based on what's saved.
    // This way, we're sure to clear out things that have been changed.
    $vid = _project_get_vid();
    $result = db_query('SELECT tid FROM {term_data} WHERE vid = %d', $vid);
    $args = array($node->nid, $node->vid);
    $terms = array();
    while ($item = db_fetch_object($result)) {
      $terms[] = $item->tid;
    }
    if (count($terms) > 1) {
      $sql = 'DELETE FROM {term_node} WHERE nid = %d AND vid = %d AND tid IN ('. db_placeholders($terms) .')';
      $args = array_merge($args, $terms);
      db_query($sql, $args);
    }
    $tid = $node->project_type;
    _project_db_save_taxonomy($node->nid, $tid, $node->vid);
    $tid_field = 'tid_' . $tid;
    if (isset($node->$tid_field)) {
      foreach ($node->$tid_field as $tid) {
        _project_db_save_taxonomy($node->nid, $tid, $node->vid);
      }
    }
  }
}

function _project_db_save_taxonomy($nid, $tid, $vid) {
  db_query('INSERT INTO {term_node} (nid, tid, vid) VALUES (%d, %d, %d)', $nid, $tid, $vid);
}

/**
 * Adds the 'project-taxonomy-element' div to the project_type
 * and term select box on the project_project node form.
 */
function theme_project_project_node_form_taxonomy($form) {
  $output = '';
  foreach (element_children($form) as $child) {
    $output .= '<div class="project-taxonomy-element">';
    $output .= drupal_render($form[$child]);
    $output .= '</div>';
  }
  return $output;
}

/**
 * Build a nested array of sections of links to display on project_project node pages.
 */
function project_get_project_link_info($node = NULL) {
  static $all_links;

  // We only need to build the links array once per page.
  if (is_array($all_links)) {
    return $all_links;
  }

  // Resources section
  $all_links['resources'] = array(
    'name' => t('Resources'),
    'weight' => 4,
    'type' => 'inline',
  );
  foreach (array('homepage' => t('Home page'), 'documentation' => t('Read documentation'), 'license' => t('Read license'), 'changelog' => t('Read complete log of changes'), 'demo' => t('Try out a demonstration'), 'screenshots' => t('Look at screenshots')) as $uri => $name) {
    if (!empty($node->project[$uri])) {
      $all_links['resources']['links'][$uri] = l($name, $node->project[$uri]);
    }
  }

  // Developer section
  $all_links['development'] = array(
    'name' => t('Development'),
    'weight' => 8,
    'type' => 'inline',
  );
  $links = array();
  if (!empty($node->project['cvs'])) {
    $links['browse_repository'] = l(t('Browse the CVS repository'), $node->project['cvs']);
  }

  if (project_use_cvs($node)) {
    $links['view_cvs_messages'] = l(t('View CVS messages'), 'project/cvs/'. $node->nid);
  }
  $all_links['development']['links'] = $links;

  // Allow other modules to add sections of links and/or links to the sections defined above.
  drupal_alter('project_page_link', $all_links, $node);

  return $all_links;
}

