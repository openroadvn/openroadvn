<?php
// $Id: contexthelp.views_default.inc,v 1.2.2.16 2010/01/26 14:32:31 darrenferguson Exp $
/**
 * @file
 * Contains default views on behalf of the contexthelp module.
 */

/**
 * Implementation of hook_default_view_views().
 */
function contexthelp_views_default_views() {
$view = new view;
$view->name = 'contexthelp';
$view->description = 'Context Help View displaying the context help for the portion of the site';
$view->tag = 'context_help';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'link_to_node' => 0,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'field_url_value' => array(
    'label' => '',
    'link_to_node' => 0,
    'label_type' => 'widget',
    'format' => 'plain',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 1,
    'id' => 'field_url_value',
    'table' => 'node_data_field_url',
    'field' => 'field_url_value',
    'relationship' => 'none',
  ),
  'body' => array(
    'label' => 'Body',
    'exclude' => 0,
    'id' => 'body',
    'table' => 'node_revisions',
    'field' => 'body',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'nid' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'Context Help',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'node',
    'validate_fail' => 'empty',
    'break_phrase' => 0,
    'not' => 0,
    'id' => 'nid',
    'table' => 'node',
    'field' => 'nid',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'contexthelp' => 'contexthelp',
      'blog' => 0,
      'contact_manager' => 0,
      'event' => 0,
      'faq' => 0,
      'og' => 0,
      'page' => 0,
      'profile' => 0,
      'story' => 0,
      'xmppchat' => 0,
    ),
    'validate_argument_node_access' => 1,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_is_member' => 0,
    'validate_argument_vocabulary' => array(
      '1' => 0,
      '41' => 0,
      '42' => 0,
      '43' => 0,
      '39' => 0,
      '38' => 0,
      '40' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
    'default_argument_user' => 0,
  ),
  'language' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'Node Language',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'empty',
    'id' => 'language',
    'table' => 'node',
    'field' => 'language',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'blog' => 0,
      'contact_manager' => 0,
      'contexthelp' => 0,
      'event' => 0,
      'faq' => 0,
      'og' => 0,
      'page' => 0,
      'profile' => 0,
      'story' => 0,
      'xmppchat' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_is_member' => 0,
    'validate_argument_vocabulary' => array(
      '1' => 0,
      '41' => 0,
      '42' => 0,
      '43' => 0,
      '39' => 0,
      '38' => 0,
      '40' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'contexthelp' => 'contexthelp',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'perm' => 'context help view',
));
$handler->override_option('title', 'Context Help');
$handler->override_option('empty', 'Context help is not available / does not exist for this section.');
$handler->override_option('empty_format', '1');
$handler->override_option('items_per_page', 0);
$handler->override_option('use_pager', '0');
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler->override_option('row_options', array(
  'inline' => array(),
  'separator' => '',
));

$views[$view->name] = $view;

$view = new view;
$view->name = 'faq_topics';
$view->description = 'List of FAQ topics in the system.';
$view->tag = 'faq';
$view->view_php = '';
$view->base_table = 'node';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'title' => array(
    'label' => 'Title',
    'link_to_node' => 1,
    'exclude' => 0,
    'id' => 'title',
    'table' => 'node',
    'field' => 'title',
    'relationship' => 'none',
  ),
  'field_url_value' => array(
    'label' => 'Page',
    'link_to_node' => 0,
    'label_type' => 'custom',
    'format' => 'plain',
    'multiple' => array(
      'group' => TRUE,
      'multiple_number' => '',
      'multiple_from' => '',
      'multiple_reversed' => FALSE,
    ),
    'exclude' => 1,
    'id' => 'field_url_value',
    'table' => 'node_data_field_url',
    'field' => 'field_url_value',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
  'body' => array(
    'label' => 'Body',
    'exclude' => 0,
    'id' => 'body',
    'table' => 'node_revisions',
    'field' => 'body',
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'language' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => 'all',
    'wildcard_substitution' => 'All',
    'title' => 'Content Language',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'empty',
    'id' => 'language',
    'table' => 'node',
    'field' => 'language',
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'default_argument_php' => '',
    'validate_argument_node_type' => array(
      'blog' => 0,
      'contact_manager' => 0,
      'contexthelp' => 0,
      'event' => 0,
      'faq' => 0,
      'og' => 0,
      'page' => 0,
      'profile' => 0,
      'story' => 0,
      'xmppchat' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_is_member' => 0,
    'validate_argument_vocabulary' => array(
      '1' => 0,
      '41' => 0,
      '42' => 0,
      '43' => 0,
      '39' => 0,
      '38' => 0,
      '40' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'type' => array(
    'operator' => 'in',
    'value' => array(
      'contexthelp_faq' => 'contexthelp_faq',
    ),
    'group' => '0',
    'exposed' => FALSE,
    'expose' => array(
      'operator' => FALSE,
      'label' => '',
    ),
    'id' => 'type',
    'table' => 'node',
    'field' => 'type',
    'relationship' => 'none',
  ),
));
$handler->override_option('access', array(
  'type' => 'role',
  'role' => array(
    '2' => 2,
    '3' => 3,
  ),
));
$handler->override_option('title', 'FAQ');
$handler->override_option('style_plugin', 'list');
$handler->override_option('style_options', array(
  'grouping' => '',
  'type' => 'ul',
));
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->override_option('path', 'help_topics');
$handler->override_option('menu', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
  'name' => 'navigation',
));
$handler->override_option('tab_options', array(
  'type' => 'none',
  'title' => '',
  'weight' => 0,
));
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->override_option('block_description', '');
$handler->override_option('block_caching', -1);

$views[$view->name] = $view;

return $views;
}
