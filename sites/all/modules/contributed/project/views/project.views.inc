<?php
// $Id: project.views.inc,v 1.5 2009/02/16 22:43:15 dww Exp $


/**
 * @file
 * Provide views data and handlers for project.module.
 */

/**
 * Implementation of hook_views_data().
 *
 * Exposes all fields to the views system.
 */
function project_views_data() {
  $data = array();
  // ----------------------------------------------------------------
  // project_projects table

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['project_projects']['table']['group']  = t('Project');

  $data['project_projects']['table']['join'] = array(
    'node' => array(
      'type' => 'INNER',
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );

  // uri
  $data['project_projects']['uri'] = array(
    'title' => t('Short name'),
    'help' => t('The short name (uri) of a project.'),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // homepage
  $data['project_projects']['homepage'] = array(
    'title' => t('Homepage'),
    'help' => t("The project's homepage."),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // changelog
  $data['project_projects']['changelog'] = array(
    'title' => t('Changelog'),
    'help' => t("The project's changelog."),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // cvs
  $data['project_projects']['cvs'] = array(
    'title' => t('CVS directory'),
    'help' => t("The project's cvs directory."),
    'field' => array(
      'group' => t('Project'),
      //'handler' => 'views_handler_field_node',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // demo
  $data['project_projects']['demo'] = array(
    'title' => t('Demonstration'),
    'help' => t('Link to a demonstration of the project.'),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // documentation
  $data['project_projects']['documentation'] = array(
    'title' => t('Documentation'),
    'help' => t("The project's documentation."),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // screenshots
  $data['project_projects']['screenshots'] = array(
    'title' => t('Screenshots'),
    'help' => t("The project's screenshots."),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  // license
  $data['project_projects']['license'] = array(
    'title' => t('License'),
    'help' => t('The license used for the project.'),
    'field' => array(
      'group' => t('Project'),
      'handler' => 'views_handler_field_url',
      'click sortable' => TRUE,
     ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
    'argument' => array(
      'handler' => 'views_handler_argument_string',
    ),
  );

  return $data;
}

/**
 * Implementation of hook_views_data_alter().
 */
function project_views_data_alter(&$data) {
  // @TODO:  Rewrite the help and title to be more descriptive?
  $data['term_node']['term_node_tid_depth_project'] = array(
    'group' => t('Taxonomy'),
    'title' => t('Term ID (summary)'),
    'help' => t('This argument produces a summary with the option to exclude top level terms.'),
    'real field' => 'tid',
    'argument' => array(
      'handler' => 'project_handler_argument_term_node_tid_depth_project',
      'name table' => 'term_data',
      'name field' => 'name',
      'empty name field' => t('Uncategorized'),
      'numeric' => TRUE,
    ),
  );

  // Add a Project type vocabulary filter that makes it possible to include
  // only the top level terms or child terms in the project types vocabulary.
  $data['term_data']['project_type'] = array(
    'title' => t('Project type depth'),
    'help' => t('Limit the results to project type terms that are either parents or children, or both.'),
    'filter' => array(
      'field' => 'vid',
      'handler' => 'project_handler_filter_project_type_vid',
    ),
  );

  // tid field (based on {term_node}.tid field in taxonomy.views.inc).
  $data['term_node']['project_type_tid'] = array(
    'group' => t('Project'),
    'title' => t('Project type term'),
    'real field' => 'tid',
    'field' => array(
      'title' => t('Project type terms'),
      'help' => t('The project type terms associated with a node.'),
      'handler' => 'project_handler_field_term_node_project_type_tid',
    ),
    'filter' => array(
      'help' => t('Filter by the project type term.'),
      'handler' => 'project_handler_filter_term_node_project_type_tid',
      'hierarchy table' => 'term_hierarchy',
      'numeric' => TRUE,
    ),
  );

  // Project sorting method filter.  This is a bit of a hack until Views
  // supports exposed sorting.
  $data['views']['project_sort_method'] = array(
    'group' => t('Project'),
    'title' => t('Sort method'),
    'help' => t('This is a filter that provides a sort method select box that can be exposed to the user.'),
    'filter' => array(
      'handler' => 'project_handler_filter_project_sort_method',
    ),
  );
}

/**
 * Implementation of hook_views_handlers().
 */
function project_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project') .'/views/handlers',
    ),
    'handlers' => array(
      'project_handler_argument_term_node_tid_depth_project' => array(
        'parent' => 'views_handler_argument_term_node_tid',
      ),
      'project_handler_field_term_node_project_type_tid' => array(
        'parent' => 'views_handler_field_term_node_tid',
      ),
      'project_handler_filter_project_sort_method' => array(
        'parent' => 'views_handler_filter',
      ),
      'project_handler_filter_project_type_vid' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'project_handler_filter_project_type_vid' => array(
        'parent' => 'views_handler_filter_in_operator',
      ),
      'project_handler_filter_term_node_project_type_tid' => array(
        'parent' => 'views_handler_filter_term_node_tid',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_plugins().
 */
function project_views_plugins() {
  $project_path = drupal_get_path('module', 'project');
  return array(
    'module' => 'project',
    'style' => array(
      'project_list' => array(
        'title' => t('Project list'),
        'help' => t('Displays a list of project rows with additional project related information.'),
        'handler' => 'project_plugin_style_project_list',
        'path' => "$project_path/views/plugins",
        'theme' => 'project_views_view_project_list',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'uses row plugin' => TRUE,
        'uses options' => TRUE,
        'uses fields' => TRUE,
        'uses grouping' => TRUE,
        'type' => 'normal',
      ),
      'collapsible_summary' => array(
        'title' => t('Collapsible'),
        'help' => t('Displays the summary in a collapsible fieldset.'),
        'handler' => 'views_plugin_style_summary',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
        'theme' => 'project_views_view_collapsible_summary',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'type' => 'summary', // only shows up as a summary style
        'uses options' => TRUE,
      ),
    ),
    'row' => array(
      'project_node' => array(
        'title' => t('Project node'),
        'help' => t('Display the project node with download table.'),
        'handler' => 'project_plugin_row_project_node_view',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
        'theme' => 'project_views_view_row_project_node',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'base' => array('node'), // only works with 'node' as base.
        'uses options' => TRUE,
        'type' => 'normal',
      ),
      'project_fields' => array(   // Project row style for fields.
        'title' => t('Project fields'),
        'help' => t('Displays project fields.'),
        'handler' => 'project_plugin_row_project_fields_view',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
        'theme' => 'project_views_view_row_project_fields',
        'theme file' => 'theme.inc',
        'theme path' => "$project_path/views/theme",
        'uses fields' => TRUE,
        'uses options' => TRUE,
        'type' => 'normal',
      ),
    ),
    'argument validator' => array(
      'project_term_top_level' => array(
        'title' => t('Project type: Top level term'),
        'handler' => 'project_plugin_argument_validate_project_term_top_level',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
      ),
      'project_type_term' => array(
        'title' => t('Project type term'),
        'handler' => 'project_plugin_argument_validate_project_type_term',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
      ),
      'project_nid' => array(
        'title' => t('Project node'),
        'handler' => 'project_plugin_argument_validate_project_nid',
        'path' => drupal_get_path('module', 'project') .'/views/plugins',
      ),
    ),
  );
}

/**
 * Provides the term object for the project type used
 * as an argument.
 *
 * @param $argument
 *   This should be the value of the argument used to indicate
 *   what project type is being browsed.  This will usually be
 *   the first argument.  The value can be either the tid or the
 *   name of the term.
 *
 * @return
 *   The term object or NULL if none could be found.
 */
function _project_views_get_project_type($argument = NULL) {
  if (module_exists('taxonomy')) {
    if (is_numeric($argument)) {
      $term = taxonomy_get_term($argument);
    }
    else {
      $project_vid = _project_get_vid();
      $term = db_fetch_object(db_query(db_rewrite_sql("SELECT t.tid, t.* FROM {term_data} t INNER JOIN {term_hierarchy} th ON t.tid = th.tid WHERE t.vid = %d AND th.parent = %d AND t.name = '%s'", 't', 'tid'), $project_vid, 0, $argument));
    }
    return $term;
  }
}

