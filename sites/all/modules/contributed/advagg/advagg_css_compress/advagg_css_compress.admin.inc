<?php

/**
 * @file
 * Admin page callbacks for the advagg CSS compression module.
 */

/**
 * Page generation function for admin/settings/css-compress
 */
function advagg_css_compress_admin_page() {
  $output = '';
  return $output . drupal_get_form('advagg_css_compress_admin_settings_form');
}

/**
 * Form builder; Configure advagg settings.
 *
 * @ingroup forms
 * @see system_settings_form()
 */
function advagg_css_compress_admin_settings_form() {
  $form = array();

  $form['advagg_css_compress_agg_files'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Compress CSS Files'),
    '#default_value'  => variable_get('advagg_css_compress_agg_files', ADVAGG_CSS_COMPRESS_AGG_FILES),
  );
  $form['advagg_css_compress_inline'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('Compress Inline CSS'),
    '#default_value'  => variable_get('advagg_css_compress_inline', ADVAGG_CSS_COMPRESS_INLINE),
  );
  $form['advagg_css_compressor'] = array(
    '#type'           => 'radios',
    '#title'          => t('Select the compression library to use'),
    '#default_value'  => variable_get('advagg_css_compressor', ADVAGG_CSS_COMPRESSOR),
    '#options'        => array(
      0 => 'CSSTidy',
      1 => 'CSS Compressor',
    ),
    '#description'    => t('<a href="@csscompressor">CSS Compressor</a> is a faster CSS compression alternative. <a href="@csstidy">CSSTidy</a> is a well known CSS compression library, but it has some known issues/limitations.',
      array(
        '@csscompressor'  => 'http://www.codenothing.com/css-compressor/',
        '@csstidy'        => 'https://github.com/Cerdic/CSSTidy',
      )
    ),
  );
  $form['advagg_css_compress_preserve_css'] = array(
    '#type'           => 'checkbox',
    '#title'          => t('CSSTidy: Preserve CSS'),
    '#default_value'  => variable_get('advagg_css_compress_preserve_css', ADVAGG_CSS_COMPRESS_PRESERVE_CSS),
    '#description'    => t('If disabled CSS selectors will try to be merged together, significantly reducing the css file size. May result in broken layouts is disabled. This only applies to compression through CSSTidy.'),
  );

  return system_settings_form($form);
}
