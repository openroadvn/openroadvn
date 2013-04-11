<?php
// $Id: project_edit_releases.inc,v 1.3 2010/01/17 01:03:58 dww Exp $

/**
 * @file
 * Code for the "releases" subtab to the edit tab on project nodes.
 */

/**
 * Releases subtab for the project edit tab.
 */
function project_release_project_edit_releases($node) {
  project_project_set_breadcrumb($node);
  drupal_set_title(check_plain($node->title));
  return drupal_get_form('project_release_project_edit_form', $node);
}

function project_release_project_edit_form($form_state, $node) {
  $active_tids = project_release_compatibility_list();
  if (count($active_tids) > 0) {
    // Get all the data about major versions for this project.
    $data = _project_release_get_version_major_data($node);

    // Build the form elements for supported and recommended major versions.
    $form = _project_release_edit_version_major_form($data);

    // Now, add a header and some help text for those elements.
    $form['header'] = array(
      '#type' => 'markup',
      '#value' => t('Supported versions'),
    );

    $vocab = taxonomy_vocabulary_load(_project_release_get_api_vid());
    $form['help'] = array(
      '#type' => 'markup',
      '#value' => t('For each term in the %api_vocabulary_name vocabulary, the following tables allow you to define which major versions are supported. You can also control if the development snapshot releases should be displayed on the front page of the project. Finally, for each term in the %api_vocabulary_name vocabulary, you can select which major version (if any) should be recommended for new users to download.', array('%api_vocabulary_name' => $vocab->name)),
    );
  }

  // Finally, add everything else (currently, the "Advanced options").
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced options'),
    '#collapsible' => TRUE,
    '#collapsed' => $node->project_release['releases'],
    '#weight' => 5,
  );

  $form['advanced']['releases'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable releases'),
    '#return_value' => 1,
    '#weight' => -10,
    '#default_value' => isset($node->project_release['releases']) ? $node->project_release['releases'] : 1,
    '#description' => t('Allow releases of this project with specific versions.'),
  );

  if (user_access('administer projects')) {
    $form['advanced']['version_format'] = array(
      '#type' => 'textfield',
      '#title' => t('Version format string'),
      '#default_value' => $node->project_release['version_format'],
      '#size' => 50,
      '#maxlength' => 255,
      '#description' => t('Customize the format of the version strings for releases of this project.') .' '. PROJECT_RELEASE_VERSION_FORMAT_HELP .' '. t('If blank, this project will use the site-wide default (currently set to: %default)', array('%default' => variable_get('project_release_default_version_format', PROJECT_RELEASE_DEFAULT_VERSION_FORMAT))),
    );
  }

  $form['nid'] = array('#type' => 'value', '#value' => $node->nid);
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 45,
  );

  return $form;
}

function _project_release_get_version_major_data($node) {
  $data = array();
  $data['node'] = $node;
  $params = array();
  $params[] = $node->nid;
  $active_tids = project_release_compatibility_list();
  $tid_where = '';
  if (!empty($active_tids)) {
    $tid_where = 'prsv.tid IN ('. db_placeholders($active_tids) .')';
    $params = array_merge($params, array_keys($active_tids));
  }
  $result = db_query("SELECT prsv.*, td.name AS term_name FROM {project_release_supported_versions} prsv INNER JOIN {term_data} td ON prsv.tid = td.tid WHERE prsv.nid = %d AND $tid_where ORDER BY td.weight, td.name", $params);
  while ($obj = db_fetch_object($result)) {
    $tid = $obj->tid;
    if (empty($data[$tid])) {
      $data[$tid] = array(
        'name' => $obj->term_name,
        'majors' => array(),
      );
    }
    $data[$tid]['majors'][$obj->major] = array(
      '#snapshot' => $obj->snapshot ? TRUE : FALSE,
      '#supported' => $obj->supported ? TRUE : FALSE,
      '#recommended' => $obj->recommended ? TRUE : FALSE,
    );
  }
  return $data;
}

function _project_release_edit_version_major_form($data) {
// Since this form is used relatively infrequently, don't allow the js to be aggregated.
  drupal_add_js(drupal_get_path('module', 'project_release') .'/project_release.js', 'module', 'header', FALSE, TRUE, FALSE);
  $form = array();

  $node = $data['node'];
  unset($data['node']);

  $form['api'] = array(
    '#tree' => TRUE,
  );
  foreach ($data as $api_tid => $api_data) {
    $form['api'][$api_tid] = array(
      '#api_term_name' => $api_data['name'],
      'major' => array(),
    );
    $recommended_version = -1;
    $recommended_options = array(-1 => t('None'));
    foreach ($api_data['majors'] as $major_version => $major_data) {
      if ($major_data['#recommended'] == TRUE) {
        $recommended_version = $major_version;
      }

      $recommended_options[$major_version] = $major_version;
      $form['api'][$api_tid]['major'][$major_version] = array(
        'current' => array(
          '#type' => 'markup',
          '#value' => project_release_get_current_recommended($node->nid, $api_tid, $major_version)->version,
        ),
        'supported' => array(
          '#type' => 'checkbox',
          '#title' => t('Supported'),
          '#default_value' => $major_data['#supported'],
          '#attributes' => array('class' => 'supported'),
        ),
        'snapshot' => array(
          '#type' => 'checkbox',
          '#title' => t('Show snapshot release'),
          '#default_value' => $major_data['#snapshot'],
          '#attributes' => array('class' => 'snapshot'),
        ),
        'version_name' => array(
          // Normally, form elements of #type 'hidden' are evil, since the
          // values can be easily tampered with. However, in this case, we
          // totally don't care: the values are all completely ignored by the
          // submit handler. We need these values sent on to the browser so
          // they can be hidden/show by project_release.js, so we need to use
          // 'hidden' here or the values are never sent to the browser.
          // @TODO: We could replace these with drupal JS settings.
          '#type' => 'hidden',
          '#value' => $version_name,
          '#attributes' => array('class' => 'version-name'),
        ),
      );
    }
    $form['api'][$api_tid]['recommended'] = array(
      '#title' => t('Recommended major version'),
      '#type' => 'select',
      '#options' => $recommended_options,
      '#default_value' => $recommended_version,
      '#prefix' => '<div class="container-inline">',
      '#suffix' => '</div>',
      '#attributes' => array('class' => 'recommended'),
    );
  }
  return $form;
}

function theme_project_release_form_value($element) {
  return check_plain($element['#value']);
}

function theme_project_release_project_edit_form($form) {
  $output = '';
  if (!empty($form['api'])) {
    $output .= '<h3>'. drupal_render($form['header']) ."</h3>\n";
    $output .= '<p>'. drupal_render($form['help']) ."</p>\n";

    $header = array(
      t('Major version'),
      t('Current Release'),
      t('Supported'),
      array(
        'data' => t('Show snapshot release'),
        'colspan' => 2,
      ),
    );
    foreach (element_children($form['api']) as $tid) {
      $output .= '<h3>'. $form['api'][$tid]['#api_term_name'] .'</h3>';
      $rows = array();
      krsort($form['api'][$tid]['major']);
      foreach (element_children($form['api'][$tid]['major']) as $major) {
        $row = array();
        $row[] = $major;
        $row[] = drupal_render($form['api'][$tid]['major'][$major]['current']);
        // We want to unset the titles for each element, since we already have
        // table headers to label each column.
        unset($form['api'][$tid]['major'][$major]['supported']['#title']);
        $row[] = drupal_render($form['api'][$tid]['major'][$major]['supported']);
        unset($form['api'][$tid]['major'][$major]['snapshot']['#title']);
        $row[] = drupal_render($form['api'][$tid]['major'][$major]['snapshot']);
        $rows[] = $row;
      }
      // Finally, add a row for the currently recommended version.
      $row = array();
      $row[] = array(
        'data' => drupal_render($form['api'][$tid]['recommended']),
        'colspan' => 5,
      );
      $rows[] = $row;
      $output .= theme('table', $header, $rows);
    }
    unset($form['api']);
  }

  $output .= drupal_render($form['advanced']);
  $output .= drupal_render($form);
  return $output;
}

/**
 * Validates the project form regarding release-specific fields.
 * Ensures that the version format string doesn't contain bad characters.
 * @ingroup project_release_fapi
 * @see project_release_project_edit_releases
 */
function project_release_project_edit_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['version_format'])) {
    _project_release_validate_format_string($form_state['values'], 'version_format');
  }
  if (isset($form_state['values']['api'])) {
    foreach ($form_state['values']['api'] as $tid => $api_info) {
      $supported = FALSE;
      // First, we just iterate through to see if *any* majors are supported.
      foreach ($api_info['major'] as $major => $flags) {
        // At least 1 major is supported, so validate the settings.
        if ($flags['supported'] == FALSE && $flags['snapshot'] == TRUE) {
          $element = 'api]['. $tid .'][major]['. $major .'][snapshot';
          form_set_error($element, t('You can not show a snapshot release for a major version that is not supported for %api_term_name.', array('%api_term_name' => $form_state['values']['api'][$tid]['#api_term_name'])));
        }
        if ($flags['supported'] == FALSE && $api_info['recommended'] == $major) {
          form_set_error("api][$tid][recommended", t('You can not recommend a major version that is not supported for %api_term_name.', array('%api_term_name' => $form_state['values']['api'][$tid]['#api_term_name'])));
        }
      }
    }
  }
}

/**
 * Submit handler when project admins use the releases subtab.
 * @ingroup project_release_fapi
 * @see project_release_project_edit_releases
 */
function project_release_project_edit_form_submit($form, &$form_state) {
  $nid = $form_state['values']['nid'];
  db_query("UPDATE {project_release_projects} SET releases = %d, version_format = '%s' WHERE nid = %d", $form_state['values']['releases'], $form_state['values']['version_format'], $nid);

  if (!empty($form_state['values']['api'])) {
    foreach ($form_state['values']['api'] as $tid => $values) {
      if (!empty($values['major'])) {
        foreach ($values['major'] as $major => $major_values) {
          $major_values['recommended'] = ($values['recommended'] == $major) ? 1 : 0;
          if ($obj = db_fetch_object(db_query("SELECT * FROM {project_release_supported_versions} WHERE nid = %d AND tid = %d AND major = %d", $nid, $tid, $major))) {
            if ($obj->supported != $major_values['supported']
                || $obj->recommended != $major_values['recommended']
                || $obj->snapshot != $major_values['snapshot']) {
              db_query("UPDATE {project_release_supported_versions} SET supported = %d, recommended = %d, snapshot = %d WHERE nid = %d AND tid = %d AND major = %d", $major_values['supported'], $major_values['recommended'], $major_values['snapshot'], $nid, $tid, $major);
            }
          }
          else {
            db_query("INSERT INTO {project_release_supported_versions} (nid, tid, major, supported, recommended, snapshot) VALUES (%d, %d, %d, %d, %d, %d)", $nid, $tid, $major, $major_values['supported'], $major_values['recommended'], $major_values['snapshot']);
          }
        }
      }
    }
  }
  db_query("UPDATE {node} SET changed = %d WHERE nid = %d", time(), $form_state['values']['nid']);

  $cid = 'table:'. $form_state['values']['nid'] .':';
  cache_clear_all($cid, 'cache_project_release', TRUE);

  drupal_set_message(t('Release settings have been saved.'));
}

