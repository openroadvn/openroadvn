<?php
// $Id: tvi.admin.inc,v 1.1 2009/12/04 01:40:56 awebb Exp $

//******************************************************************************
// TVI Administration Interface
//******************************************************************************

//------------------------------------------------------------------------------
// Menu callbacks
//------------------------------------------------------------------------------

/**
 * Return html option set of available displays for view with specified view id
 */
function tvi_display_options_ahah() {
  
  $view_id = $_GET['view_id'];
  $options = '';
  
  if ($view_id > 0) {
    foreach (tvi_get_view_displays($view_id) as $name => $label) {
      $options .= "<option value='$name'>$label</option>";
    }
  }  
  print $options;
  exit;
}

//------------------------------------------------------------------------------
// Administrative forms
//------------------------------------------------------------------------------

/**
 * Adds TVI administration form to the vocabulary edit page
 *
 * admin/content/taxonomy/edit/vocabulary/$vid
 */
function tvi_vocab_form(&$form) {
  
	if (!($vid = $form['vid']['#value'])) {
		return;
	}  
  // A bit of re-ordering
  $form['identification']['#weight'] = -4; // core taxonomy fieldset
  $form['content_types']['#weight']  = -3; // core taxonomy fieldset
  $form['settings']['#weight']       = -2; // core taxonomy fieldset
  
  tvi_include('query');  
  tvi_taxonomy_admin_form($form, tvi_load_settings($vid, TVI_TYPE_VOCAB));
}

//------------------------------------------------------------------------------

/**
 * Adds TVI administration form to the term edit page
 *
 * admin/content/taxonomy/edit/term/$tid
 */
function tvi_term_form(&$form) {
  
  if (!($tid = $form['#term']['tid'])) {
    return;
  }
  // A bit of re-ordering
  $form['identification']['#weight'] = -2; // core taxonomy fieldset
  $form['advanced']['#weight']       =  0; // core taxonomy fieldset
  
  tvi_include('query');  
  tvi_taxonomy_admin_form($form, tvi_load_settings($tid, TVI_TYPE_TERM));
}

//------------------------------------------------------------------------------

/**
 * The main TVI administration form.
 *
 * This is added to both vocabulary and term edit forms.
 */
function tvi_taxonomy_admin_form(&$form, $settings) {
    
  $path = drupal_get_path('module', 'tvi');
  drupal_add_js("$path/tvi.js");
  drupal_add_css("$path/tvi.css");
  
  // Add validate and submit handlers
  $form['#validate'][] = 'tvi_validate_handler';
  $form['#submit'][]   = 'tvi_submit_handler';
  
  // Form container
  $form['tvi'] = array(
    '#type' => 'fieldset',
    '#title' => t('View usage'),
    '#weight' => -1,
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#tree' => TRUE,
  );
  
  // Javascript warning message
  $form['tvi']['javascript_message'] = array(
    '#type' => 'markup',
    '#value' => '<p class="javascript-warning">' 
      . t('This form is best used with Javascript enabled.'
          . '  Please enable your Javascript, then refresh this page.') 
      . '</p>',
  );
  
  $form['tvi']['is_default'] = array(
    '#type' => 'value',
    '#value' => $settings->is_default,
  );
  
  $form['tvi']['type'] = array(
    '#type'  => 'value',
    '#value' => $settings->type,
  );
  
  $form['tvi']['xid'] = array(
    '#type'  => 'value',
    '#value' => $settings->xid,
  );
    
  //  View selector
  $form['tvi']['view_id'] = array(
    '#id' => 'tvi-view-selector',
    '#type' => 'select',
    '#title' => t('Using the view'),
    '#default_value' => ($settings->view_id ? $settings->view_id : NULL),
    '#options' => tvi_get_views(),
    '#description' => t('The view that you wish to use for this display'),
  );
  // Display selector
  $form['tvi']['display'] = array(
    '#id' => 'tvi-display-selector',
    '#type' => 'select',
    '#title' => t('View display'),
    '#default_value' => (
      $settings->view_id 
      ? $settings->view_id . ':' . $settings->display 
      : NULL
    ),
    '#options' => tvi_get_view_displays(), // Assume no Javascript
    '#description' => t('The view display you select from the option above will be used to render this taxonomy term. <b>NOTE:</b> Views defined for vocabulary are trumped by views for terms.'),
  );
  
  // Status toggle switch
  $form['tvi']['status'] = array(
    '#id' => 'tvi-status-check',
    '#type' => 'checkbox',
    '#title' => t('Use view override.'),
    '#default_value' => $settings->status,
    '#description' => t('Unchecking this field will disable the use of the view when displaying this taxonomy page.'), 
  );
}

//------------------------------------------------------------------------------

/**
 * Validate the TVI administration form submission.
 * 
 * If the view and the display do not line up, throw an error.
 */
function tvi_validate_handler($form, &$form_state) {
	
	$values = $form_state['values']['tvi'];
	$view   = views_get_view($values['view_id']);
  
  list($view_id, $display_name)           = explode(':', $values['display']);  
  $form_state['values']['tvi']['display'] = $display_name;
  
  if ($view->vid != $view_id) {
    form_set_error('tvi][display', 
      t('You must select a display from the selected view.'
        . '  You wouldn\'t have this problem if you would enable Javascript.')
    );
  } 
}

//------------------------------------------------------------------------------

/**
 * Save the view taxonomy data to the database.
 */
function tvi_submit_handler($form, &$form_state) {
	$settings = (object)$form_state['values']['tvi'];
	
	if (!$settings->is_default || $settings->status) {
    tvi_update_settings($settings);
	}
}

//------------------------------------------------------------------------------
// Internal utilities
//------------------------------------------------------------------------------

/**
 * Gather a listing of all views so that the admin may choose ANY view
 */
function tvi_get_views($return_object = FALSE) {
  
	$views = array();
  
  foreach (views_get_all_views() as $view) {
  	
  	if ($return_object) {
      $views[$view->vid] = $view;	
  	}
  	else {
      $views[$view->vid] = $view->name;
  	} 
  }
  return $views;
}

//------------------------------------------------------------------------------

/**
 * Gathers the available display options for the view operating on this term 
 * or vocabulary.
 */
function tvi_get_view_displays($view = NULL) {
  
  $displays = array();
  
  if ($view === NULL) {
    foreach(tvi_get_views(TRUE) as $view_id => $view) {
    	foreach($view->display as $display_name => $display) {
      	// Suppress the 'default' plugin display
        $add = ($display->display_plugin != 'default') ? ' - ' . $display->display_plugin : ''; 
        $displays[$view_id . ':' . $display_name] = $view->name . ' - ' . $display->display_title . $add;
      } 
    }
  }
  else {
    // If it is a name or vid we can use this...
    if (!is_object($view)) {
      $view = views_get_view($view);
    }
    if (is_object($view)) {
      foreach($view->display as $display_name => $display) {
        // Suppress the 'default' plugin display
        $add = ($display->display_plugin != 'default') ? ' - ' . $display->display_plugin : ''; 
        $displays[$view->vid . ':' . $display_name] = $display->display_title . $add;
      }
    }
  }
  return $displays;
}

//------------------------------------------------------------------------------