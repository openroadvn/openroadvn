<?php
// $Id: views-view-table.tpl.php,v 0.1 2011/02/08 11:43:43 sebastien.millart Exp $
/*	*
	* @file views-view-table.tpl.php
	* Template to display a view as a table.
	*
	* - $title : The title of this group of rows.  May be empty.
	* - $header: An array of header labels keyed by field id.
	* - $fields: An array of CSS IDs to use for each field id.
	* - $class: A class or classes to apply to the table, based on settings.
	* - $row_classes: An array of classes to apply to each row, indexed by row
	*   number. This matches the index in $rows.
	* - $rows: An array of row items. Each row is an array of content.
	*   $rows are keyed by row number, fields within rows are keyed by field ID.
	* @ingroup views_templates
	*/
 	
//	Store the label value of the table headers in an array (if not empty).
	$label_array = array();
    if (isset($header)) {
      foreach ($header as $field => $label) if (!empty($label)) $label_array[] = $label;
    }
	
//	Include the multi-dimension array of colspans.
	require './' . drupal_get_path('theme', 'joinup') . '/includes/template.view-colspans.inc';
	
//	Get the CSS class stored in the basic settings of the view object.
	$display = $view -> display[$view -> current_display];
	$css_class = $display -> display_options['css_class'];
	
//	View the contents of objects available in views theming
//	print dsm(get_defined_vars());
	
?>
<?php
//	Check if table headers contain a label value.
	if ($label_array):
?>
	<table class="<?php print $class; ?>">
		<?php if (!empty($title)) : ?>
			<caption><?php print $title; ?></caption>
		<?php endif; ?>
		<thead>
			<tr>
				<?php foreach ($header as $field => $label): ?>
					<th class="views-field views-field-<?php print $fields[$field]; ?>">
						<?php print $label; ?>
					</th>
				<?php endforeach; ?>
			</tr>
		</thead>
		<tbody>
			<?php foreach ($rows as $count => $row): ?>
				<tr class="<?php print implode(' ', $row_classes[$count]); ?>">
					<?php foreach ($row as $field => $content): ?>
						<td class="views-field views-field-<?php print $fields[$field]; ?>">
							<?php print $content; ?>
						</td>
					<?php endforeach; ?>
				</tr>
			<?php endforeach; ?>
		</tbody>
	</table>
<?php
//	If table headers are empty (no label value).
	else:
?>
	<?php foreach ($rows as $count => $row): ?>
		<div class="<?php print implode(' ', $row_classes[$count]); ?> clearfix">
			<?php $col_number = 0; ?>
			<?php foreach ($row as $field => $content): ?>
				<?php
					
					$col_count = count($row);
					$col_class = '';
					if ($col_number == 0) $col_class = ' first';
					if (count($row) == ($col_number + 1)) $col_class = ' last';
					if (count($row) == 1) $col_class = ' first last';
					
				?>
				<div class="colspan-<?php ($colspans[$col_count][$css_class][$col_number]) ? print $colspans[$col_count][$css_class][$col_number] : print $colspans[$col_count][$col_number]; ?> <?php print $col_class; ?> fields views-field-<?php print $fields[$field]; ?>">
					<?php print $content; ?>
				</div>
				<?php $col_number++; ?>
			<?php endforeach; ?>
		</div>
	<?php endforeach; ?>
<?php endif; ?>
