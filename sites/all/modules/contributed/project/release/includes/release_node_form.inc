<?php
// $Id: release_node_form.inc,v 1.11 2010/01/30 02:33:40 dww Exp $

/**
 * @file
 * Code required for the release node form.
 */

/**
 * Private helper to implement hook_form().
 *
 * Create the project release node form.
 */
function _project_release_form(&$release, &$form_state) {
  if (arg(1) == 'add') {
    // Initialize variables and $release object properties to prevent notices.
    $is_edit = FALSE;
    $admin = NULL;
    $pid = (integer) arg(3);
    $project = node_load($pid);
    if (!isset($project) || $project->type != 'project_project') {
      drupal_set_message(t('Node %nid is not a valid project.', array('%nid' => $pid)), 'error');
      drupal_goto('node/add/project-release');
    }
    // Make sure this user should have permissions to add releases for
    // the requested project
    if (!project_check_admin_access($project)) {
      drupal_access_denied();
      module_invoke_all('exit');
      exit;
    }
    project_project_set_breadcrumb($project, TRUE);
    $format = project_release_get_version_format($project);
    $release->project_release['pid'] = $pid;
  }
  else {
    global $user;
    $admin = user_access('administer projects');
    $is_edit = TRUE;
    $project = node_load($release->project_release['pid']);
    $breadcrumb[] = l($project->title, 'node/'. $project->nid);
    $breadcrumb[] = l(t('Releases'), 'node/'. $project->nid . '/release');
    project_project_set_breadcrumb($project, $breadcrumb);
    $format = project_release_get_version_format($project);
  }
  $form['project'] = array(
    '#type' => 'value',
    '#value' => $project,
  );

  $modify = $admin || !$is_edit;

  $form['#attributes'] = array("enctype" => "multipart/form-data");

  if ($is_edit) {
    $form['rel_id'] = array(
      '#type' => 'fieldset',
      '#title' => t('Release identification'),
      '#weight' => -4,
      '#collapsible' => TRUE,
      '#theme' => 'project_release_node_form_version_elements',
    );
  }
  _project_release_form_add_text_element($form['rel_id']['title'], t('Title'), $release->title, $is_edit, $admin, TRUE, 40, 128);
  if (!empty($release->project_release['version'])) {
    _project_release_form_add_text_element($form['rel_id']['version'], t('Version string'), $release->project_release['version'], $is_edit, $admin, TRUE, 20, 255);
  }
  // The version string belongs in $node->project_release[], so we set
  // #parents to ensure the form value is placed there during validate,
  // preview, and submit.
  $form['rel_id']['version']['#parents'] = array('project_release', 'version');

  $form['project_release'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('Version number elements'),
    '#collapsible' => TRUE,
    '#theme' => 'project_release_node_form_version_elements',
  );

  if (!empty($release->project_release['pid'])) {
    $pid = $release->project_release['pid'];
  }
  elseif (!empty($form_state['values']['project_release']['pid'])) {
    $pid = $form_state['values']['project_release']['pid'];
  }
  // else?
  $form['project_release']['pid'] = array(
    '#type' => 'value',
    '#value' => $pid,
  );

  $form['validate_version'] = array('#type' => 'value', '#value' => 1);
  _project_release_form_add_version_element($form, $release, $modify, $format, 'major', t('Major'));
  _project_release_form_add_version_element($form, $release, $modify, $format, 'minor', t('Minor'));
  _project_release_form_add_version_element($form, $release, $modify, $format, 'patch', t('Patch-level'));
  _project_release_form_add_version_element($form, $release, $modify, $format, 'extra', t('Extra identifier'), t('Optionally specify other identifying information for this version, for example "beta-1", "rc-1" or "dev". In most cases, this should be left blank.'), 40);

  // A boolean to indicate if this release should be regularly rebuilt
  // (e.g. from a revision control branch) or not (e.g. an official release
  // from a tag).  There's no UI for this field, since it's only used if a
  // revision-control integration module is enabled.
  $form['project_release']['rebuild'] = array(
    '#type' => 'value',
    '#value' => !empty($release->project_release['rebuild']),
  );

  $form['project_release_files'] = array(
    '#type' => 'fieldset',
    '#tree' => TRUE,
    '#title' => t('File information'),
    '#collapsible' => TRUE,
  );

  if (!empty($release->project_release['files'])) {
    $files = $release->project_release['files'];
    foreach ($files as $fid => $file) {
      // In the case of previews, we only have the fid of the file,
      // so check for that and load the file object if necessary.
      if (is_numeric($file)) {
        $file = project_release_load_file($file);
      }
      $form['project_release_files'][$fid]['file_info'] = array(
        '#value' => theme('project_release_download_file', $file, FALSE),
      );
      if ($admin) {
        $form['project_release_files'][$fid]['delete'] = array(
          '#type' => 'checkbox',
          '#title' => t('Delete @filename', array('@filename' => $file->filename)),
          '#default_value' => isset($release->project_release_files[$fid]['delete']) ? $release->project_release_files[$fid]['delete'] : 0,
          // TODO: we'll need to rip this out when multiple files lands.
          //'#description' => (isset($files) ? t('In order to add a new file, you must first delete %filename.', array('%filename' => $file->filename)) : ''),
        );
      }
      // Store the fid here in the proper $node->project_release[] array
      // so it can be used to generate file previews.
      $form['project_release']['files'][$fid] = array(
        '#type' => 'value',
        '#value' => $fid,
      );
    }
  }

  // TODO: we'll need to rip out the conditional when multiple files lands.
  //if (!isset($files)) {
    // In case of previews, get any uploaded file info.
    if (isset($form_state['project_release']['new_file'])) {
      $new_file = $form_state['project_release']['new_file'];
    }
    elseif (!empty($release->project_release_files['temp'])) {
      $new_file = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $release->project_release_files['temp']));
    }
    $form['project_release_files']['file'] = array(
      '#title' => t('File'),
      '#type' => 'file',
      '#description' => isset($new_file) ? t('A file named %filename has already been uploaded. If you upload another file %filename will be replaced.', array('%filename' => $new_file->filename)) : t('Choose a file that will be associated with this release.'),
    );
    // Account for already uploaded files being previewed.
    if (isset($new_file)) {
      $form['project_release_files']['temp'] = array(
        '#type' => 'value',
        '#value' => $new_file->fid,
      );
    }
  //}

  $form['body_field'] = node_body_field($release, t('Release notes'), !$admin);
  // Add a description to the body field.
  $form['body_field']['body']['#description'] = t('Enter a description of this release, such as a list of the major changes or updates.');

  $tag = isset($release->project_release['tag']) ? $release->project_release['tag'] : '';
  _project_release_form_add_text_element($form['tag']['tag'], t('Tag'), $tag, $is_edit, $admin, TRUE, 40, 255);
  $form['tag']['tag']['#parents'] = array('project_release', 'tag');

  // Add a custom validation function.
  $form['#validate'][] = 'project_release_node_form_validate';
  return $form;
}

/**
 * Modifies the given $form array to add the appropriate form element
 * for the requested version field. Since the 20+ lines of code in
 * here have to be duplicated 6 times in project_release_form(), this
 * function exists so we can reuse the code.
 * @see project_release_form
 * @ingroup project_release_internal
 *
 * @param $form Form array to modify
 * @param $release Relase node the form is for
 * @param $modify Boolean indicating if we should allow modifications
 * @param $format Version format string for this project
 * @param $name Name of this version element
 * @param $title Translatable title of the form element
 * @param $description Translatable description of the form element.
 * @param $size Size of the form element
 * @param $required Boolean for if the form element should be required
 */
function _project_release_form_add_version_element(&$form, $release, $modify, $format, $name, $title, $description = '', $size = 10, $required = FALSE) {
  $var_name = 'version_'. $name;
  $regexp = "@.*[!#%]$name.*@";
  if (preg_match($regexp, $format)) {
    $form['project_release'][$var_name] = array(
      '#type' => 'textfield',
      '#title' => $title,
      '#default_value' => isset($release->project_release[$var_name]) ? $release->project_release[$var_name] : '',
      '#size' => $size,
      '#maxlength' => $size + 10,
      '#attributes' => array('style' => 'width:auto'),
    );
    if ($required) {
      // TODO: handle this more flexibly for sites not using CVS
      // perhaps if the format variable is UPPERCASE it's required,
      // and lowercase is optional or something crazy?
      $form[$var_name]['#required'] = TRUE;
    }
    if ($description) {
      $form['project_release'][$var_name]['#description'] = $description;
    }
    if (!$modify) {
      $form['project_release'][$var_name]['#attributes'] = array('disabled' => 'disabled');
      $form['project_release'][$var_name]['#value'] = $release->project_release[$var_name];
    }
  }
  else {
    $form['project_release'][$var_name] = array(
      '#type' => 'value',
      '#value' => isset($release->project_release[$var_name]) ? $release->project_release[$var_name] : '',
    );
  }
}

/**
 * Modifies the given $form array to add the appropriate form element
 * for the desired text field. Since the 16+ lines of code in here
 * have to be duplicated 5 times in project_release_form(), this
 * function exists so we can reuse the code.
 * @see project_release_form
 * @ingroup project_release_internal
 *
 * @param $form
 *   Reference to form element to add.
 * @param $title
 *   Translatable title of the form element.
 * @param $value
 *   The value to use in the form.
 * @param $is_edit
 *   Boolean indicating if we're editing or creating.
 * @param $admin
 *   Boolean for if the edit is by a project administrator.
 * @param $required
 *   Boolean for if the field should be required.
 * @param $size
 *   Value to use for the '#size' property.
 * @param $maxlength
 *   Value to use for the '#maxlength' property.
 */
function _project_release_form_add_text_element(&$form, $title, $value, $is_edit, $admin, $required = TRUE, $size = 40, $maxlength = 50) {
  if ($is_edit && !empty($value)) {
    $form = array(
      '#type' => 'textfield',
      '#title' => $title,
      '#default_value' => $value,
      '#required' => $required,
      '#size' => $size,
      '#maxlength' => $maxlength,
    );
    if(!$admin) {
      $form['#attributes']['disabled'] = 'disabled';
      $form['#value'] = $value;
    }
  }
  else {
    $form = array(
      '#type' => 'value',
      '#value' => $value,
    );
  }
}

/**
 * Private callback to validate a release node form.
 *
 * @see project_release_node_form_validate()
 */
function _project_release_node_form_validate(&$form, &$form_state) {
  global $user;
  $project_release = $form_state['values']['project_release'];
  if (!empty($form_state['values']['validate_version'])) {
    if (!isset($project_release['version_major']) && !isset($project_release['version_minor']) &&
        !isset($project_release['version_patch']) &&
        (!($project_release['version_extra']) || $project_release['version_extra'] === '')) {
      form_set_error('project_release][version_major', t('You must fill in some version information.'));
      // TODO: find a better form value to mark as the error?
    }
    foreach (array('version_major' => t('Major version number'), 'version_minor' => t('Minor version number')) as $field => $name) {
      $val = $project_release[$field];
      if (isset($val) && $val !== '' && !is_numeric($val)) {
        form_set_error("project_release][$field", t('%name must be a number.', array('%name' => $name)));
      }
    }
    $val = $project_release['version_patch'];
    if (isset($val) && $val !== '' && !is_numeric($val) && $val != 'x') {
      form_set_error('project_release][version_patch', t("Patch-level version number must be numeric or the letter 'x'."));
    }
  }

  $validators = array(
    'project_release_validate_file_extension' => array(),
  );

  // For some reason, with the $form['project_release_files'] element
  // #tree'd, the uploaded file shows up in 'project_release_files'
  // and not it's sub-element, so we use that here.
  if ($file = file_save_upload('project_release_files', $validators, file_directory_path() . '/project')) {
    // We need the file object, so pass that into $form_state here.
    $form_state['project_release']['new_file'] = $file;
  }

  if (project_release_get_api_taxonomy()) {
    $vid = _project_release_get_api_vid();
    if (isset($form_state['values']['taxonomy'])) {
      $tid = $form_state['values']['taxonomy'][$vid];
    }
    elseif (isset($form_state['values'][$vid])) {
      $tid = $form_state['values'][$vid];
    }
    if (isset($tid) && is_numeric($tid)) {
      $form_state['values']['project_release']['version_api_tid'] = $tid;
      $project_release['version_api_tid'] = $tid;
    }
  }

  // With cvs.module installed, this validation is already handled.
  // We only want to do it here if we're *not* doing the N-page form...
  if (!empty($form_state['values']['validate_version']) && !isset($form_state['values']['nid'])) {
    $version = (object) $project_release;
    if (project_release_exists($version)) {
      // TODO: is there a better form element to mark with this error?
      form_set_error('project_release][version_patch', t('This version already exists for this project.'));
    }
  }

  // TODO: it'd be nice to automagically reset the version string and
  // title based on changes to the version elements on an edit, but we
  // have to be careful not to break the fancy N-page form when
  // cvs_form_alter() is involved...
  $project = isset($form_state['values']['project']) ? $form_state['values']['project'] : new stdClass;

  if (isset($project->project['uri'])) {
    $project_name = $project->project['uri'];
  }
  elseif (isset($project_release['pid'])) {
    $project_name = project_get_uri_from_nid($form_state['values']['project_release']['pid']);
  }

  if (isset($form_state['values']['title'])) {
    // TODO: Magic re-setting to "%project_name %version" ??
  }
  elseif (isset($project_release['version']) && $project_release['version'] !== '') {
    form_set_value($form['title'], $project_name .' '. $project_release['version'], $form_state);
  }
  elseif (!empty($project)) {
    $version = project_release_get_version((object) $project_release, $project);
    form_set_value(array('#parents' => array('project_release', 'version')), $version, $form_state);
    $title = !empty($project_name) ? $project_name : $project->title;
    form_set_value($form['title'], "$title $version", $form_state);
  }
}

function project_release_validate_file_extension($file) {
  // Make sure that the extension on the file is one of the allowed
  // extensions for release files. Most of this validation code was
  // modified from the code in file_check_upload().
  $extensions = variable_get('project_release_file_extensions', PROJECT_RELEASE_FILE_EXTENSIONS);
  $regex = '/\.('. ereg_replace(' +', '|', preg_quote($extensions)) .')$/i';
  if (!preg_match($regex, $file->filename)) {
    return array(t('It is only possible to attach files with the following extensions: %files-allowed.', array('%files-allowed' => $extensions)));
  }
  return array();
}

function project_release_node_submit(&$form, $form_state) {
  // Get rid of the file upload item, not needed.
  unset($form_state['values']['project_release_files']['file']);
  // Look for newly uploaded files.
  if (isset($form_state['project_release']['new_file'])) {
    $new_file = $form_state['project_release']['new_file'];
  }
  elseif (!empty($form_state['values']['project_release_files']['temp'])) {
    $temp = $form_state['values']['project_release_files']['temp'];
    // Have to ensure the temp file hasn't been wiped from the files table.
    if ($temp_file = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $temp))) {
      $new_file = $temp_file;
    }
    unset($form_state['values']['project_release_files']['temp']);
  }
  $existing_files = !empty($form_state['values']['project_release_files']) ? $form_state['values']['project_release_files'] : NULL;

  if (isset($existing_files)) {
    foreach ($existing_files as $fid => $values) {
      if ($values['delete']) {
        $file = db_fetch_object(db_query("SELECT * FROM {files} WHERE fid = %d", $fid));
        project_release_file_delete($file);
      }
    }
  }
  // Add new files.
  if (isset($new_file)) {    
    $status_updated = file_set_status($new_file, FILE_STATUS_PERMANENT);
    if ($status_updated) {
      $new_file->nid = $form_state['nid'];
      $filepath = file_create_path($new_file->filepath);
      $new_file->filehash = md5_file($filepath);
      drupal_write_record('project_release_file', $new_file);
    }
  }
}

/**
 * Helper method to take data out of a $node object and store it into
 * the DB as necessary. Sadly, db_query() doesn't let us store NULL in
 * the DB, since those get cast to 0. Therefore, we have to do some
 * manual effort to dynamically create the appropriate SQL depending
 * on which version fields are set in the release node.
 * @see project_release_insert
 * @see project_release_update
 * @see db_query
 * @ingroup project_release_internal
 *
 * @param $node
 *   Object containing form values from the project_release node form.  Even
 *   though this is NOT a fully loaded $node object, the release-related
 *   values are in the $node->project_release array due to manual #tree and
 *   #parents hacking in project_release_form().
 * @param $is_new Is this a new release node, or are we updating?
 */
function project_release_db_save($node, $is_new) {
  // If the patch field is set to a non-numeric value, we just want to
  // keep it as a NULL in the DB, instead of casting it to a 0.
  if (isset($node->project_release['version_patch']) && !is_numeric($node->project_release['version_patch'])) {
    unset($node->project_release['version_patch']);
  }

  $types = array('pid' => "%d", 'version' => "'%s'", 'tag' => "'%s'",
    'rebuild' => "%d",
  );
  $values = array(
    'pid' => $node->project_release['pid'],
    'version' => $node->project_release['version'],
    'tag' => $node->project_release['tag'],
    'rebuild' => $node->project_release['rebuild'],
  );
  $fields = array('version_major', 'version_minor', 'version_patch', 'version_api_tid');
  foreach ($fields as $field) {
    if (isset($node->project_release[$field]) && is_numeric($node->project_release[$field])) {
      $types[$field] = "%d";
      $values[$field] = $node->project_release[$field];
    }
  }
  if (module_exists('taxonomy')) {
    // version_api_tid might not be where we think it is, so if we don't have
    // a real value by now, look in $node->taxonomy
    if (empty($types['version_api_tid'])) {
      $vid = _project_release_get_api_vid();
      if (isset($node->taxonomy[$vid])) {
        if (is_array($node->taxonomy[$vid])) {
          $api_tid = reset($node->taxonomy[$vid]);
        }
        else {
          $api_tid = (int)$node->taxonomy[$vid];
        }
        $types['version_api_tid'] = '%d';
        $values['version_api_tid'] = $api_tid;
      }
    }

    if (($type_vid = _project_release_get_release_type_vid()) && ($security_tid = variable_get('project_release_security_update_tid', 0))) {
      $types['security_update'] = '%d';
      $values['security_update'] = !empty($node->taxonomy[$type_vid][$security_tid]);
    }
  }

  $version_extra_weight_map = project_release_get_version_extra_weight_map();
  if (!empty($node->project_release['version_extra'])) {
    $types['version_extra'] = "'%s'";
    $values['version_extra'] = $node->project_release['version_extra'];
    // Since we have a version_extra defined, see what the weight should be,
    // based on our current mapping of version_extra prefixes to weights.
    foreach ($version_extra_weight_map as $prefix => $weight) {
      // If the $prefix exists inside version_extra, we have a match. We use
      // === 0 to tell the difference between the prefix being at position 0
      // (start of the string) vs. strpos() returning FALSE (not found).
      if (strpos($node->project_release['version_extra'], $prefix) === 0) {
        $types['version_extra_weight'] = "%d";
        $values['version_extra_weight'] = $weight;
        break;
      }
    }
    // If version_extra contains any digits, save them as version_extra_delta.
    // This is used to ensure that alpha10 is considered "newer" than alpha9.
    $match = array();
    if (preg_match('/(\d+)/', $node->project_release['version_extra'], $match)) {
      $types['version_extra_delta'] = "%d";
      $values['version_extra_delta'] = $match[1];
    }
  }
  // If there's no version_extra, but our mapping defines a weight for 'NULL',
  // specify save that weight into the version_extra_weight column.
  elseif (!empty($version_extra_weight_map['NULL'])) {
    $types['version_extra_weight'] = "%d";
    $values['version_extra_weight'] = $version_extra_weight_map['NULL'];
  }

  if ($is_new) {
    $types['nid'] = "%d";
    $sql = 'INSERT INTO {project_release_nodes} ('. implode(', ', array_keys($types)) .') VALUES ('. implode(', ', $types) .')';
  }
  else {
   $arr = array();
   foreach ($types as $key => $value) {
     $arr[] = $key .' = '. $value;
   }
   $sql = 'UPDATE {project_release_nodes} SET '. implode(',', $arr) .' WHERE nid = %d';
  }
  $values['nid'] = $node->nid;
  db_query($sql, $values);
}

/**
 * Redirect node/add/project_release/* to node/add/project-release/*.
 */
function project_release_add_redirect_page() {
  $arg = arg(3);
  drupal_goto('node/add/project-release/' . (empty($arg) ? '' : $arg));
}

/**
 * Form builder for a simple form to select a project when creating a new
 * release (as the first "page", but this is not really a multi-page form).
 */
function project_release_pick_project_form() {
  $form = array();

  drupal_set_title(t('Submit @name', array('@name' => node_get_types('name', 'project_release'))));

  // Fetch a list of all projects.
  $uris = NULL;
  $projects = array(0 => t('- Select a project -')) + project_projects_select_options($uris);
  if (count($projects) == 1) {
    drupal_set_message(t('You do not have access to any projects.'), 'error');
  }

  $form['pid'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#options' => $projects,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  return $form;
}

function project_release_pick_project_form_validate($form, &$form_state) {
  if (empty($form_state['values']['pid'])) {
    form_set_error('pid', t('You must select a project.'));
  }
  $node = node_load($form_state['values']['pid']);
  if (empty($node) || $node->type != 'project_project') {
    form_set_error('pid', t('Invalid project selected.'));
  }
}

function project_release_pick_project_form_submit($form, &$form_state) {
  $form_state['redirect'] = 'node/add/project-release/'. $form_state['values']['pid'];
}
