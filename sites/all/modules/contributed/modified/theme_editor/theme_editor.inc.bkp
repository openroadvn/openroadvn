<?php
// $Id: theme_editor.inc,v 1.1.2.2 2009/05/07 20:12:03 arcaneadam Exp $

/****************************
|							|
|	Menu Callbacks			|
|							|
****************************/


/**
 * Menu-callback for JavaScript-based new file creation.
 */
function theme_editor_js() {
	$form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  // Get the form from the cache.
	// Load the form from the Form API cache.
  if (!($form = form_get_cache($form_build_id, $form_state))) {
    form_set_error('form_token', t('Validation error, please try again. If this error persists, please contact the site administrator.'));
    $output = theme('status_messages');
  }
  else {
  // We will run some of the submit handlers so we need to disable redirecting.
  $form['#redirect'] = FALSE;
  // We need to process the form, prepare for that by setting a few internals
  // variables.
  $form['#post'] = $_POST;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  // Build, validate and if possible, submit the form.
  $form_state = array('values' => $_POST);
  $form_state['values']['dir']=$form['dir']['#value'];
  $form_state['values']['file_ext']=$form['file_ext']['#value'];
  $form_state['values']['current_file']=$form['current_file']['#value'];
  $filename = theme_editor_new_file_form_submit($form, $form_state);


  $file = (!$_POST['filename'] && !$_POST['filetype']) ? NULL : $_POST['filename'].$_POST['filetype'];	
  
  $out =   theme_editor_build_file_tree($form['dir']['#value'],$form['theme']['#value'], $file,$form_state['values']['current_file']);
  $output = theme('status_messages').$out[0];
 
}
    print drupal_to_js(array('status' => TRUE, 'data' => $output));
  exit;
}





/**
 * Menu-callback for Main Theme Editor Page.
 */
function theme_editor_editor($theme) {
	drupal_add_css(drupal_get_path('module', 'theme_editor') .'/theme_editor.css', 'module');
	if (variable_get('theme_editor_use_icons',NULL) != NULL) {
	drupal_add_css(drupal_get_path('module', 'theme_editor') .'/theme_editor_icons.css', 'module');
	}
	drupal_add_js(drupal_get_path('module', 'theme_editor') .'/theme_editor.js', 'module');
	$dir = str_replace('/'.$theme->name.'.info','',$theme->filename);
	//$special = file_scan_directory($dir,'\.[a-z0-9]',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	//drupal_set_message('<pre>'.print_r($special,1).'</pre>','error');
		$out = theme_editor_build_file_tree($dir, $theme);
	$file=$out[1];
	$out=$out[0];
	$showname = $file != null ? '&nbsp;&nbsp;&nbsp;File: '.$file.'' : '';
	$head = '<h4>Theme Directory: '.$dir.$showname.'</h4>
	';
	$tabs = '<div class="theme_editor_tabs_wrap"><ul class="tabs secondary theme_editor_tabs">
	<li class="active"><a href="#theme_editor_file_editor_form" id="editor_tab" >'.$file.' Editor</a></li>
	<li><a href="#theme_editor_file_backup_form" id="backup_tab" class="not_active">'.$file.' Backup</a></li>
	</ul></div>';
	$edit_form='<div class="theme_editor_form_wrap">'.$tabs.'<div id="theme_editor_file_editor_form" class="theme_editor_file_editor_form form_wrap">'.drupal_get_form('theme_editor_editor_form', $dir,$file).'</div><div class="theme_editor_file_backup_form form_wrap" id="theme_editor_file_backup_form">'.drupal_get_form('theme_editor_backup_form', $dir,$file).'</div></div>';
	$newfile=(!user_access('Add New Files to themes') ? '':'<div id="new_file_form_wrap">'.drupal_get_form('theme_editor_new_file_form',$dir,$theme,$file).'</div>');
	$delete = (!user_access('Delete files from themes') ? '' : '<div id="theme_editor_delete_form_wrap">'.drupal_get_form('theme_editor_delete_files_form',$dir,$file,$theme).'</div>');
	$out = $head.$edit_form.'<div class="theme_editor_files_links"><div id="theme_editor_filelist_wrap">'.$out.'</div>'.$newfile.$delete.'</div>';
	return $out;
}


/**
 * Create the module System Settings form.
 **/
  
  function theme_editor_settings() {
  //This is a work in progress I have not yet determined what if anything I will use the settings page for.
  //For Now this is just here as a holder for future use.
  $form['style_settings']=array(
  '#type'=>'fieldset',
  '#title'=>t('Style Settings'),
  '#collapsible'=>TRUE,
  '#collapsed'=>FALSE,
  '#tree'=>FALSE,
   );
  $form['style_settings']['theme_editor_use_icons']=array(
  '#type'=>'checkbox',
  '#title'=>t('Use the Icons Style sheet for Theme Editor'),
  '#default_value'=>variable_get('theme_editor_use_icons',TRUE),
  );
  return system_settings_form($form);
 } 




/****************************
|							|
|	Form Callbacks			|
|							|
****************************/

/**
 *Form for creating a new file in the theme.
 */
function theme_editor_new_file_form($form_state, $directory, $theme, $file=NULL) {
	$options=array('.css'=>'CSS', '.htm'=>'HTML','.js'=>'Javascript','.txt'=>'Text');
	if (user_access('Edit PHP based files')) {
		$options['.php']='PHP';
		$options['.tpl.php']='.tpl.php';
		$options['.inc']='.inc';
	}
	$form['new_file']=array(
	'#type' => 'fieldset',
  '#title' => t('Add A New File'),
  '#weight' => 3,
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
  '#tree' => FALSE,
	);
	
	$form['new_file']['filename']=array(
	'#type' => 'textfield',
  '#title' => t('File Name'),
  '#maxlength' => 255,
   '#size' => 15,
   '#required'=>TRUE,
	);
	$form['new_file']['filetype']=array(
	 '#type' => 'select',
 	 '#title' => t('File Type'),
 	 '#default_value' => '.css',
 	 '#options' => $options,
 	 '#multiple' => FALSE,
 	 '#size' => 1,
	);
	$form['new_file']['warn']=array(
	'#value'=>'<span class="theme_editor_red">You should save any changes you have made to the open files as they may be lost when you submit this form!</span>',
	);
	$form['new_file']['create'] = array(
      '#type' => 'submit',
      '#value' => t('Add New File'),
      '#name' => 'create',
      '#attributes'=>array('class' => 'add_file_button theme_editor_button'),
      '#ahah' => array(
        'path' => 'theme_editor/js',
        'wrapper' => 'theme_editor_filelist_wrap',
      'method' => 'replace',
      'effect' => 'fade',
      'progress' => array('type' => 'throbber', 'message' => t('Please wait...')),
      ),
      '#submit' => array('theme_editor_new_file_form_submit'),
    );
    $form['theme'] = array(
	'#type' => 'value',
	'#value' => $theme,
	);
    $form['dir'] = array(
    '#type'=>'value',
		'#value'=>$directory,
	);
	$form['file_ext']=array(
	'#type' => 'value',
	'#value' => $options,
	);
	$form['current_file']=array(
	'#type'=>'value',
	'#value'=>(!$file ? arg(5) : $file),
	);
	return $form;
}


/**
 * Theme Editor Form for Editing the files
 * 
 **/
 function theme_editor_editor_form($form_state, $dir,$file) {
 	$contents = _file_get_contents($dir.'/'.$file);
	$form['file_name'] = array(
	'#type' => 'value',
	'#value' => $file,
	);
	$form['file_dir'] = array(
	'#type' => 'value',
	'#value' => $dir,
	);
 	 	$form['file_editor']= array(
 	'#type' =>'textarea',
 	'#description' => t('Make any edits to the file and then hit the save button. 
 	<br /><span class="theme_editor_red">NOTE: THERE IS NOTHING THAT CHECKS A FILE FOR COMPATIBILITY AND CORRECT SYNTAX. IF YOU ARE EDITING <span class="theme_editor_blue">.php</span> FILES YOU MAY CAUSE ERRORS IF WORKING ON THE CURRENT THEME!</span>'),
 	'#default_value' => $contents,
 	'#rows'=>17,
 	);
 	$form['create_backup']=array(
 	'#type'=>'checkbox',
 	'#title'=>t('Save original to the backup file.'),
 	'#value'=>'yes',
 	'#default_value' => TRUE, 	
 	);
 	$form['file_editor_save']= array(
 	'#type' =>'submit',
 	'#value' => 'Save File',
 	'#attributes'=>array('class'=>'theme_editor_save_button theme_editor_button'),
 	);
 	return $form;
}


/**
 * Create the Backup file form
 **/
function theme_editor_backup_form($form_state, $dir,$file) {
 	$contents = theme_editor_get_backup($file,$dir);
 	$rows = !$contents ? 5 : 16 ;
 	$file_cont = _file_get_contents($dir.'/'.$file, 1);
 	if ($file_cont==$contents) {
 		$form['is_same']=array(
 		'#value'=>'<div class="error">Back Up file and orginal file contain same data.</div>',
 		);
	}
 	$form['file_name'] = array(
	'#type' => 'value',
	'#value' => $file,
	);
	$form['file_dir'] = array(
	'#type' => 'value',
	'#value' => $dir,
	);
 	 	$form['file_editor']= array(
 	'#type' =>'textarea',
 	'#description' => t('<span class="theme_editor_red">This file is not editable. It is only displayed for you to see the contents of the backup. Any edits you may try to make will not be used.</span>'),
 	'#default_value' => (!$contents ? 'No backup created yet' : $contents),
 	'#rows'=>$rows,
 	'#attributes' => array('readonly' => 'readonly'),
 	);
 	if (!$contents) {
 		$form['create_backup']=array(
 		'#type' => 'value',
 		'#value' => 1,
 		);
 		$form['file_editor_save']= array(
 	'#type' =>'submit',
 	'#value' => 'Create Backup',
 	'#attributes'=>array('class'=>'theme_editor_create_backup_button theme_editor_button')
	);
	}
	else {
 	$form['file_editor_save']= array(
 	'#type' =>'submit',
 	'#value' => 'Restore Backup',
 	'#attributes'=>array('class'=>'theme_editor_restore_backup_button theme_editor_button')
 	);
}
 	return $form;
}


/**
 * Create the form for deleteing files
 **/
function theme_editor_delete_files_form($form_state,$dir,$file,$theme) {
	$filesA = theme_editor_get_files($dir,array('info','img'));
	foreach($filesA as $key => $value) {
		foreach($value as $filepath => $val) {
			if ($file != $val->basename) {
			$files[$filepath] = $val->basename;
		}
		}
	}
	$form['delete_file']=array(
	'#type' => 'fieldset',
  '#title' => t('Delete A File'),
  '#weight' => 3,
  '#collapsible' => TRUE,
  '#collapsed' => TRUE,
	);
	$form['theme'] = array(
	'#type' => 'value',
	'#value' => $theme,
	);
	$form['delete_file']['file']=array(
	 '#type' => 'select',
 	 '#title' => t('File'),
 	 '#options' => $files,
 	 '#multiple' => FALSE,
 	 '#size' => 1,
 	 '#required' =>TRUE,
	);
	$form['delete_file']['warn']=array(
	'#value'=>'<span class="theme_editor_red">You should save any changes you have made to the open files as they WILL be lost when you submit this form!</span>',
	);
	$form['delete_file']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete File'),
      '#name' => 'delete',
      '#attributes'=>array('class' => 'delete_file_button theme_editor_button'),
    );
    $form['dir'] = array(
    '#type'=>'value',
		'#value'=>$dir,
	);
	$form['no_delete']= array(
	'#type' => 'value',
	'#value' => $dir.'/'.$file,
	);
	$form['options']= array(
	'#type' => 'value',
	'#value' => $files,
	);
	return $form;
}



/****************************
|							|
|  Form Handling Callbacks	|
|	Submit and Validate		|
|							|
****************************/



/**
 * Submission function for new file creation
 **/
function theme_editor_new_file_form_submit($form, &$form_state) {
	$file_template=array(
	'.js' => '//Javascript File Created by Theme Editor for Drupal',
	'.php' => '
<?php
//PHP File Created by Theme Editor for Drupal',
	'.tpl.php' => '
<?php
//PHP Templete File Created by Theme Editor for Drupal',
	'.htm' => '
<html>
   <head>
	 <title>HTML File Created by Theme Editor for Drupal</title>
   </head>
   <body>
   </body>
</html>
	',
	'.css' => '
/********************************************
*                                           *   
*CSS File Created by Theme Editor for Drupal*
*                                           *
********************************************/',
	'.txt' => 'Text File Created by Theme Editor for Drupal',
	'.inc' => '
<?php
//PHP Include File Created by Theme Editor for Drupal',
	);
	
	if (empty($form_state['values']['filename'])) {
drupal_set_message('You must enter a file name.', 'error');
return false;
}
else {

	if (!isset($form_state['values']['file_ext'][$form_state['values']['filetype']])) {

	drupal_set_message('You cannot try create file types that are not listed in the form, no matter how hard you try buddy.', 'error');
		return false;
	}


	$filedata = $file_template[$form_state['values']['filetype']];
	$filename = $form_state['values']['filename'].$form_state['values']['filetype'];
	if (file_exists($form_state['values']['dir'].'/'.$filename)) {
		drupal_set_message('File already exists', 'error');
		return false;
	}
	else {
	if (!file_put_contents($form_state['values']['dir'].'/'.$filename,$filedata)) {
	drupal_set_message('File was not created', 'error');
	return false;
}
else {
drupal_set_message('File Created');
	return $filename;
}
}
}
}

/**
 * Submit function for theme editor form
 */
function theme_editor_editor_form_submit($form, &$form_state) {
if (isset($form_state['values']['create_backup'])) {
		
		theme_editor_create_backup($form_state['values']['file_name'],$form_state['values']['file_dir']);
		drupal_set_message('Backup was created');
	}
$filedata = $form_state['values']['file_editor'];
if (!_file_write_theme_file($filedata,$form_state['values']['file_dir'],$form_state['values']['file_name'])) {
	drupal_set_message('File was not saved', 'error');
}
else {
	drupal_set_message('File was saved successfully');
	return 'test test';
}
}


/**
 * Backup Restore Submit Function
 **/
function theme_editor_backup_form_submit($form, &$form_state) {
	if (isset($form_state['values']['create_backup'])) {
		theme_editor_create_backup($form_state['values']['file_name'],$form_state['values']['file_dir']);
	drupal_set_message('Backup was created');
	}
	else {
if (!theme_editor_restore_backup($form_state['values']['file_name'],$form_state['values']['file_dir'])) {
	drupal_set_message('Backup was not restored, please try again', 'error');
}
else {
	drupal_set_message('Backup was restored successfully');
	return 'test test';
}
}
}


/**
 * Submission function for delete files form
 **/
function theme_editor_delete_files_form_submit($form, &$form_state) {
	if ($form_state['values']['file'] == $form_state['values']['no_delete']) {
	drupal_set_message('You can not delete the current working file. Hence the reason for you needing to hack the form to try and do that','error');
	return false;
	}
	elseif (!isset($form_state['values']['options'][$form_state['values']['file']])) {
	drupal_set_message('You can not delete files not listed in the form. Please don\'t try to hack the forms. It is not very nice','error');
	return false;
	}
	else {
		if (!unlink($form_state['values']['file'])) {
	drupal_set_message('Oops! I had the Hiccups and the file wasn\'t deleted. Why don\'t you give it another shot.','error');
		return false;
		}
		else {
			drupal_set_message('The file was deleted');
		}
	}
}



/****************************
|							|
|  File Handling Functions	|
|							|
****************************/


/**
 * Reads the contents of a file and outputs the string
 */
function _file_get_contents($file, $mess=1) {
	if (filesize($file) >=1) {
	
$contents = file_get_contents($file);

return $contents;
}
else {
	if ($mess != NULL) {
	drupal_set_message('This file contains no data. You may still edit this file. When you add something and save it you will no longer get this message','error');
}
	return 'File is empty. It contains no data.';
}
}

/*
 * Writes the files back to the original destination
 *
**/
function _file_write_theme_file($data, $dir,$file, $mess=NULL) {
$filename = $dir.'/'.$file;
$somecontent = $data;

// Let's make sure the file exists and is writable first.
if (is_writable($filename)) {
	
    //open file in write mode to write the data contents to it.
    
	if (!!file_put_contents($filename,$somecontent)) {		
    ($mess==NULL ? drupal_set_message('File changes written to file ('.$filename.')') : '');
	}

return TRUE;
} 
else {
   ($mess==NULL ? drupal_set_message('The file ('.$filename.') is not writeable','error') : '');
         return FALSE;
}
}

/**
 * Mimics the php 5 function file_put_contents for php 4 and lower
 **/
if (!function_exists('file_put_contents')) {
function file_put_contents($filename, $somecontent) {
	if (!$handle = fopen($filename, 'w')) {
         
         return FALSE;

    }

    // Write $somecontent to our opened file.
    if (fwrite($handle, $somecontent) === FALSE) {
         return FALSE;
    }
    fclose($handle);
    return TRUE;
}
}

/**
 * Checks for a backup version of the passed file and if available returns the contents
 * If no backup is available returns false
 **/
function theme_editor_get_backup($file, $dir) {
	if (!file_exists($dir.'/theme_editor_backup/'.$file.'.bak')) {
		return false;
	}
	else {
		$contents = _file_get_contents($dir.'/theme_editor_backup/'.$file.'.bak', NULL);
		return $contents;
	}
}

/**
 * Function to restore the backup file to the original file
 **/
function theme_editor_restore_backup($file, $dir) {
	$backup=theme_editor_get_backup($file, $dir);
	if (!$backup) {
	return false;
	}
	else {
	if ($backup=='File is empty. It contains no data.') {
		$backup='';
	}
	if (!file_put_contents($dir.'/'.$file,$backup)) {
		return false;
	}
	else {
		return true;
	}
}
}


/**
 * Create a buck up of the current file
 **/
function theme_editor_create_backup($file,$dir) {
	$filename = $dir.'/'.$file;
	if (file_exists($dir.'/theme_editor_backup')) {
	if (copy($filename, $dir.'/theme_editor_backup'.'/'.$file.'.bak')) {
		drupal_set_message('Backup created of file ('.$filename.')');
	}
	else {
		drupal_set_message('Unable to create backup of file ('.$filename.')','error');
	}
}
else {
	if (mkdir($dir.'/theme_editor_backup')) {
		if (copy($filename, $dir.'/theme_editor_backup'.'/'.$file.'.bak')) {
		drupal_set_message('Backup created of file ('.$filename.')');
	}
	else {
		drupal_set_message('Unable to create backup of file ('.$filename.')','error');
	}
	}
	else {
		drupal_set_message('Unable to create backup of file ('.$filename.'). Unable to create the (theme_editor_backup) folder. Check your filesystem permmisions to fix this error','error');
	}
}
}


/****************************
|							|
|  Theme Editor Functions	|
|							|
****************************/


/**
 * Creates the list of files for the file tree
 **/
function theme_editor_get_files($dir,$excl=array()) {
if (!in_array('info',$excl) && user_access('Edit theme .info files')) {
		$files['Theme Info'] = file_scan_directory($dir,'\.info$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
		}
		if (!in_array('php',$excl) && user_access('Edit PHP based files')) {
	$files['PHP'] = file_scan_directory($dir,'[^(\.tpl)]\.((php)|(inc))$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	//$files['PHP'] = file_scan_directory($dir,'\.tpl\.php$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	if (!in_array('js',$excl)) {
	$files['Javascript'] = file_scan_directory($dir,'\.js$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	if (!in_array('tpl',$excl) && user_access('Edit PHP based files')) {
	$files['Template'] = file_scan_directory($dir,'\.tpl\.php$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	if (!in_array('css',$excl)) {
	$files['Style Sheet'] = file_scan_directory($dir,'\.css$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	if (!in_array('htm',$excl)) {
	$files['Basic'] = file_scan_directory($dir,'\.((txt)|(htm)|(html))$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	if (!in_array('img',$excl)) {
	$files['Image'] = file_scan_directory($dir,'\.((png)|(jpg)|(jpeg)|(gif))$',array('.', '..', 'CVS'), 0, false, 'filename', 0);
	}
	return $files;
}


/**
 * Build the file menu tree for Theme Editor
 **/
function theme_editor_build_file_tree($dir, $theme,$new=NULL,$current=NULL) {
	$file_img = array('txt'=>'txt_img','info'=>'txt_img','css'=>'css_img','php'=>'php_img','inc'=>'php_img','js'=>'js_img','htm'=>'htm_img','html'=>'htm_img','png'=>'img_img','jpg'=>'img_img','jpeg'=>'img_img','gif'=>'img_img',);
	$files=theme_editor_get_files($dir);
	$file= !$current ? arg(5) : $current;
	$i=0;
	
	$out='
	<ul class="theme_editor_filelist" >';
	foreach($files as $key => $value) {
	$collapsed=TRUE;
	foreach($value as $type => $val) {
			if (!$file) {
			$file = $val->basename;
			}
			if ($val->basename == $file) {
			$collapsed = FALSE;
			$testing = "collap1";
			}
			if ($new == $file) {
			$collapsed = FALSE;
			$testing .= " collap2";
			}
			
		}
		if (!empty($value)) {
		$form[$key]=array(
		'#type'=>'fieldset',
		'#title'=>t($key.' Files'),
		'#collapsible'=>TRUE,
		'#collapsed'=>$collapsed,
		'#prefix'=>'
		<li class="theme_editor_filetype_title" ref="'.$testing.'"><div>',
		'#suffix'=>'</div></li>',
		'#weight'=>$i,
		);
		$form[$key]['ul']=array(
		'#prefix'=>'<ul class="theme_editor_filetype_list">',
		'#tree'=>TRUE,
		'#suffix'=>'</ul>'
		);
		foreach($value as $type => $val) {
	
			$file_type=end(explode('.',$val->basename));
			if ($key=='Other' || $key=='Image') {
						$li ='<li class="theme_editor_filename unwriteable" ref="'.$type.'"><span class="'.$file_img[$file_type].'">'.$val->basename.'<br /><span class="file_ne">(Not Editable)</span></span></li>';
			}
			else {
				$file_type=end(explode('.',$val->basename));
			if (is_writable($dir.'/'.$val->basename)) {
			$li='<li class="theme_editor_filename '.($file == $val->basename ? 'active_file' : '').''.($new == $val->basename ? ' new_file' : '').'" ref="'.$type.'">'.l($val->basename.($new == $val->basename ? ' <span class="new_sup">New</span>' : ''),'admin/build/themes/editor/'.$theme->name.'/'.$val->basename, array('html'=>TRUE)).'</li>';
			/*
			Took out the hrdcoded link: '<a href="'.base_path().'admin/build/themes/editor/'.$theme->name.'/'.$val->basename.'" class="'.$file_img[$file_type].'">'.$val->basename.''.($new == $val->basename ? ' <span class="new_sup">New</span>' : '').'</a>
			*/
		}
		else {
			$li='<li class="theme_editor_filename unwriteable" ref="'.$type.'"><span class="'.$file_img[$file_type].'">'.$val->basename.'<br /><span class="file_ne">(Not Editable)</span></span></li>';
		}
	}
	$form[$key]['ul'][$val->basename]=array(
			'#value'=>$li,
			);
		
	
	}
}
	$i++;
	}
	$out .= drupal_render($form);
	$out .='</ul>';
	
	
		return array($out,$file);
}

