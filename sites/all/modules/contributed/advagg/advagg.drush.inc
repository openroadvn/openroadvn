<?php

/**
 * @file
 * Drush commands for Advanced CSS/JS Aggregation.
 */

/**
 * Implement hook_drush_cache_clear.
 *
 * @param $types
 *   List of cache types that can be cleared.
 */
function advagg_drush_cache_clear(&$types) {
  // Add in Advanced CSS/JS Aggregation
  $types['advagg'] = 'drush_advagg_cache_scan';
}

/**
 * Rescan bundles and rebuild if needed.
 */
function drush_advagg_cache_scan() {
  global $_advagg;
  _drupal_flush_css_js();
  $cache_tables = advagg_flush_caches();
  foreach ($cache_tables as $table) {
    cache_clear_all('*', $table, TRUE);
  }

  if (empty($_advagg['files'])) {
    drush_log(dt('Advanced CSS/JS Aggregation cache scanned and no out of date bundles detected.'));
  }
  else {
    if (empty($_advagg['rebuilt'])) {
      drush_log(dt("Advanced CSS/JS Aggregation cache scanned and out of date bundles have marked.\nOld Files:\n!files\nMarked Bundles Count: !count", array(
        '!files' => implode("\n", $_advagg['files']),
        '!count' => count($_advagg['bundles']),
      )
      ));
    }
    else {
      drush_log(dt("Advanced CSS/JS Aggregation cache scanned and out of date bundles have been incremented and rebuilt.\nOld Files:\n%files\n%count done.", array(
        '%files' => implode("\n", $_advagg['files']),
        '%count' => count($_advagg['rebuilt']),
      )
      ));
    }
  }
}
