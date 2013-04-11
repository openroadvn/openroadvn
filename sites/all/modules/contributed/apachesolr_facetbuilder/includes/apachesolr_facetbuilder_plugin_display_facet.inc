<?php
/**
 * The plugin that handles a facet builder facet.
 */
class apachesolr_facetbuilder_plugin_display_facet extends views_plugin_display {
  
  /**
   * Display validation.
   */
  function validate() {
  }

  function get_style_type() { return 'facet'; }
  
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('style_plugin', 'row_options', 'row_plugin', 'items_per_page'))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

  /**
   * Provide the default summary for options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    $categories = array();
    $categories['basic'] = array(
      'title' => t('Basic settings'),
    );

    $options['display_title'] = array(
      'category' => 'basic',
      'title' => t('Name'),
      'value' => check_plain($this->display->display_title),
      'desc' => t('Change the name of this display. This will be the filter title if no title is specified.'),
    );
    $title = strip_tags($this->get_option('title'));
    if (!$title) {
      $title = t('None');
    }

    $options['title'] = array(
      'category' => 'basic',
      'title' => t('Title'),
      'value' => $title,
      'desc' => t('The title will show in the filter block as the clickable link.'),
    );
  }

  /**
   * Inject anything into the query that the display handler needs.
   */
  function query() {
    if (!empty($this->view->facetbuilder_node->nid)) {
      $this->view->query->add_where(0, 'node.nid = %d', $this->view->facetbuilder_node->nid);
    }
  }

  /**
   * Set up any variables on the view prior to execution. These are separated
   * from execute because they are extremely common and unlikely to be
   * overridden on an individual display.
   */
  function pre_execute() {
    $this->view->set_items_per_page(1);
  }

}
