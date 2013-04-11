<?php
// $Id: views-view-field.tpl.php,v 0.1 2011/02/08 10:22:32 sebastien.millart Exp $
/*	*
	* This template is used to print a single field in a view. It is not
	* actually used in default Views, as this is registered as a theme
	* function which has better performance. For single overrides, the
	* template is perfectly okay.
	*
	* Variables available:
	* - $view: The view object
	* - $field: The field handler object that can process the input
	* - $row: The raw SQL result that can be used
	* - $output: The processed output that will normally be used.
	*
	* When fetching output from the $row, this construct should be used:
	* $data = $row->{$field->field_alias}
	*
	* The above will guarantee that you'll always get the correct data,
	* regardless of any changes in the aliasing that might happen if
	* the view is modified.
	*/
  
	$result = str_replace('_', '-', $field -> field_alias);
	$search = array('-field', 'node-');
	$class = str_replace($search, '', $result);
	($field -> field_alias == 'node_title') ? $content = '<strong>'. $output .'</strong>' : $content = $output;
	
?>
<?php if ($view->name == 'faq_topics') : ?>
  <?php print $content; ?>
<?php elseif (!empty($content)) : ?>
	<div class="field field-<?php print $class; ?>"><?php print $content; ?></div>
<?php endif; ?>