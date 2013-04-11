// $Id: tvi.js,v 1.1 2009/12/04 01:40:55 awebb Exp $

Drupal.behaviors.tvi_initialize = function(context) {
	
	//--------------------------------------------------------------------------
	// Properties

	var current_display = $('#tvi-display-selector').val();

	//--------------------------------------------------------------------------
	// Handlers
	
	var set_active_option = function() {
		$('#tvi-display-selector').val(current_display);
	}
	
	//--------------------------------------------------------------------------
	
	var view_change_handler = function() {
		var view_id = $('#tvi-view-selector').val();
		
		if (!view_id) {
			view_id = $('#tvi-view-selector option:first').val();
			$('#tvi-view-selector').val(view_id);
		}		
		// Load new view displays.
		$('#tvi-display-selector').load(
			'/tvi/js/display_options?view_id=' + view_id, 
			'', set_active_option
		);
	}
	
	//--------------------------------------------------------------------------
	// Start	
	
	// Javascript is enabled.
	$('.javascript-warning').hide();
	
	// Reload displays when views are changed.
	$('#tvi-view-selector').change(view_change_handler);
	view_change_handler();
}