<?php
// $Id: tvi.query.inc,v 1.1 2009/12/04 01:40:56 awebb Exp $

//******************************************************************************
// TVI Database Interface
//******************************************************************************

/**
 * Return a default setting object.
 */
function tvi_default_settings($type = TVI_TYPE_TERM, $xid = 0) {
	$settings = new stdClass();
	
  $settings->type = $type;
  $settings->xid  = $xid;
  
  $settings->view_id = 0;
  $settings->display = TVI_DEFAULT_DISPLAY;
  $settings->status  = 0;
  
  $settings->is_default = TRUE;
		
	return $settings;	
}

//------------------------------------------------------------------------------

/**
 * Load a setting from the database or return a default, if the $return_default
 * flag is set.
 */
function tvi_load_settings($xid, $type = TVI_TYPE_TERM, $return_default = TRUE) {
	
	$settings = db_fetch_object(
    db_query("SELECT * FROM {tvi_settings} ts"
              . " WHERE ts.type = '%s' AND ts.xid = %d", 
             $type, $xid
    )
  );
  
  if (!is_object($settings)) {
  	return ($return_default ? tvi_default_settings($type, $xid) : NULL);
  }
  
  $settings->is_default = FALSE;
  return $settings;
}

//------------------------------------------------------------------------------

/**
 * Save settings information for a taxonomy vocabulary or term to the database.
 */
function tvi_update_settings($settings) {
	
	if (is_array($settings)) {
	 $settings = (object)$settings;	
	}
	
  $result = db_query(
    "UPDATE {tvi_settings} SET view_id = %d, display = '%s', status = %d"
    . " WHERE type = '%s' AND xid = %d",
    $settings->view_id,
    $settings->display,
    $settings->status,
    $settings->type,
    $settings->xid
  );

  if (!db_affected_rows()) {
    $result = db_query(
      "INSERT INTO {tvi_settings} (`type`, `xid`, `view_id`, `display`, `status`)"
      . " VALUES ('%s', %d, %d, '%s', %d)", 
      $settings->type,
      $settings->xid,
      $settings->view_id,
      $settings->display,
      $settings->status    
    );
  }
  return $result;
}

//------------------------------------------------------------------------------

function tvi_remove_settings($xid, $type = TVI_TYPE_TERM) {
  return db_query(
    "DELETE FROM {tvi_settings} WHERE type = '%s' AND xid = %d", 
    $type, $xid
  );	
}

//------------------------------------------------------------------------------