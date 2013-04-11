<?php

/**
 * @file
 * Views integration code for Apachesolr Facet Builder.
 */


/**
 *  Implementation of hook_views_query()
 *  
 *  Handle the date_popup calendar goto date.
 */
function apachesolr_facetbuilder_views_query_alter(&$view, &$query) {
}

function apachesolr_facetbuilder_views_pre_view(&$view, &$display_id, &$args) {
  //
}

/**
 * Implementation of hook_views_plugins
 */
function apachesolr_facetbuilder_views_plugins() {
  $path = drupal_get_path('module', 'apachesolr_facetbuilder');
  $views_path = drupal_get_path('module', 'views');

  $data = array(
    'module' => 'apachesolr_facetbuilder', // This just tells our themes are elsewhere.
    'display' => array(
      // Parents are not really displays, just needed so the files can
      // be included.
      'parent' => array(
        'no ui' => TRUE,
        'handler' => 'views_plugin_display',
        'path' => "$views_path/plugins",
        'parent' => '',
      ),
      // Facet display type plugin.
      'facet' => array(
        'title' => t('Facet'),
        'help' => t('A facet to index and filter apachesolr results by.'),
        'handler' => 'apachesolr_facetbuilder_plugin_display_facet',
        'path' => "$path/includes",
        'theme' => 'views_view',
        'uses hook menu' => FALSE,
        'uses hook block' => FALSE,
        'use ajax' => FALSE,
        'use pager' => FALSE,
        'accept attachments' => TRUE,
        'admin' => t('Facetbuilder Facet'),
      ),
    ),
  );
  return $data;
}