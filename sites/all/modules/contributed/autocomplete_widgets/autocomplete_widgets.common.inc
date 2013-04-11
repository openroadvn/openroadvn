<?php
// $Id: autocomplete_widgets.common.inc,v 1.1.2.2 2009/12/24 09:28:59 markuspetrux Exp $

/**
 * @file
 * Common functions for Autocomplete Widgets module.
 */

/**
 * Fetch an array of options for the given widget.
 *
 * @param $field
 *   The field description.
 * @param $string
 *   Optional string to filter values on (used by autocomplete).
 * @param $match
 *   Operator to match filtered name against, can be any of:
 *   'contains', 'equals', 'starts_with'
 * @param $keys
 *   Optional keys to lookup (the $string and $match arguments will be
 *   ignored).
 * @param $limit
 *   If non-zero, limit the size of the result set.
 *
 * @return
 *   An array of valid values in the form:
 *   array(
 *     key => value,
 *     ...
 *   )
 */
function _autocomplete_widgets_get_options($field, $string = '', $match = 'contains', $keys = NULL, $limit = NULL) {
  static $results = array();

  // Create unique id for static cache.
  if (!isset($keys) || !is_array($keys)) {
    $keys = array();
  }
  $cid = $field['field_name'] .':'. $match .':'. ($string !== '' ? $string : implode('-', $keys)) .':'. $limit;

  if (!isset($results[$cid])) {
    if ($field['widget']['type'] == 'autocomplete_widgets_allowvals') {
      $results[$cid] = _autocomplete_widgets_get_options_allowvals($field, $string, $match, $keys, $limit);
    }
    else if ($field['widget']['type'] == 'autocomplete_widgets_flddata') {
      $results[$cid] = _autocomplete_widgets_get_options_flddata($field, $string, $match, $keys, $limit);
    }
    else {
      $results[$cid] = array();
    }
  }

  return $results[$cid];
}

/**
 * Fetch an array of options for the given widget (allowed values).
 *
 * Options are retrieved from the allowed values defined for the field.
 */
function _autocomplete_widgets_get_options_allowvals($field, $string = '', $match = 'contains', $keys = NULL, $limit = NULL) {
  $function = $field['module'] .'_allowed_values';
  $allowed_values = (array)(function_exists($function) ? $function($field) : content_allowed_values($field));
  if (!isset($limit) || !is_numeric($limit)) {
    $limit = count($allowed_values);
  }
  $case_insensitive = (!empty($field['widget']['autocomplete_case']) ? FALSE : TRUE);
  if ($case_insensitive && $match != 'equals') {
    $string = drupal_strtolower($string);
  }
  $count = 0;
  $options = array();
  foreach ($allowed_values as $key => $value) {
    if ($string === '') {
      if (isset($keys) && is_array($keys)) {
        if (in_array($key, $keys)) {
          $options[$key] = $value;
          $count++;
        }
      }
      else {
        $options[$key] = $value;
        $count++;
      }
    }
    else if ($match == 'equals') {
      if ($value == $string) {
        $options[$key] = $value;
        $count++;
      }
    }
    else {
      $pos = strpos(($case_insensitive ? drupal_strtolower($value) : $value), $string);
      if (($match == 'starts_with' && $pos === 0) || ($match == 'contains' && $pos !== FALSE)) {
        $options[$key] = $value;
        $count++;
      }
    }
    if ($count >= $limit) {
      break;
    }
  }
  return $options;
}

/**
 * Fetch an array of options for the given widget (field data).
 *
 * Options are retrieved from existing values for the field.
 */
function _autocomplete_widgets_get_options_flddata($field, $string = '', $match = 'contains', $keys = NULL, $limit = NULL) {
  $db_info = content_database_info($field);
  $table = $db_info['table'];
  $column = $field['field_name'] .'_'. key($db_info['columns']);
  $where = array();
  $args = array();

  if ($string !== '') {
    $lower = (!empty($field['widget']['autocomplete_case']) || $match == 'equals' ? '' : 'LOWER');
    $match_operators = array(
      'contains' => "LIKE $lower('%%%s%%')",
      'equals' => "= '%s'",
      'starts_with' => "LIKE $lower('%s%%')",
    );
    $where[] = "$lower(f.". $column .') '. (isset($match_operators[$match]) ? $match_operators[$match] : $match_operators['contains']);
    $args[] = $string;
  }
  else if (isset($keys) && is_array($keys)) {
    $where[] = 'f.'. $column .' IN ('. db_placeholders($keys) .')';
    $args = array_merge($args, $keys);
  }

  $sql = 'SELECT f.'. $column .' FROM {'. $table .'} f WHERE '. implode(' AND ', $where) .' ORDER BY f.'. $column;
  if (!empty($field['widget']['i18n_flddata'])) {
    // Adding a join with the node table allows the i18n rewrite the query
    // to filter values from node for the proper language.
    $sql = db_rewrite_sql(str_replace(' WHERE ', ' INNER JOIN {node} n ON f.vid = n.vid WHERE ', $sql));
  }
  $result = $limit ? db_query_range($sql, $args, 0, $limit) : db_query($sql, $args);
  $options = array();
  while ($row = db_fetch_object($result)) {
    $options[$row->$column] = $row->$column;
  }

  return $options;
}
