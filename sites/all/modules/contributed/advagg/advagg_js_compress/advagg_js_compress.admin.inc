<?php

/**
 * @file
 * Admin page callbacks for the advagg JS compression module.
 */

/**
 * Page generation function for admin/settings/js-compress
 */
function advagg_js_compress_admin_page() {
  $output = '';
  return $output . drupal_get_form('advagg_js_compress_admin_settings_form');
}

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_js_compress_admin_settings_form() {
  $form = array();

  $form['advagg_js_compress_agg_files'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Compress JS Files'),
    '#default_value'  => variable_get('advagg_js_compress_agg_files', ADVAGG_JS_COMPRESS_AGG_FILES),
  );
  $form['advagg_js_compress_inline'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Compress Inline JS'),
    '#default_value'  => variable_get('advagg_js_compress_inline', ADVAGG_JS_COMPRESS_INLINE),
  );

  $description = '';
  $options = array(0 => t('JSMin+'));
  if (function_exists('jsmin')) {
    $options[1] = t('JSMin');
    $description .= t('JSMin is the C complied version and is about 25 times faster. Recommend using it.');
  }
  else {
    $description .= t('You can use the much faster C version of JSMin by installing the <a href="@php_jsmin">JSMin PHP Extension</a> on this server.', array('@php_jsmin' => 'http://www.ypass.net/software/php_jsmin/'));
  }
  $form['advagg_js_compressor'] = array(
    '#type'           => 'radios',
    '#title'          => t('Select the compression program to use'),
    '#default_value'  => variable_get('advagg_js_compressor', ADVAGG_JS_COMPRESSOR),
    '#options'        => $options,
    '#description'    => filter_xss($description),
  );

  $form['advagg_js_compress_packer_enable'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Enable Packer'),
    '#default_value'  => variable_get('advagg_js_compress_packer_enable', ADVAGG_JS_COMPRESS_PACKER_ENABLE),
    '#description'    => t('If enabled the non gzip version of JS files will be compressed using the JS Packer. WARNING: This has a high chance of breaking your JS. Only Enable on production after testing the non gzipped version locally.'),
  );

  return system_settings_form($form);
}
