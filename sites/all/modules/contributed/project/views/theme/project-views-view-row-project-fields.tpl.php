<?php
// $Id: project-views-view-row-project-fields.tpl.php,v 1.1 2009/01/12 20:34:07 dww Exp $
/**
 * @file project-views-view-row-project-fields.tpl.php
 * View template to display available project fields in a row.
 *
 * - $view: The view in use.
 * - $fields: an array of $field objects. Each one contains:
 *   - $field->content: The output of the field.
 *   - $field->raw: The raw data for the field, if it exists. This is NOT output safe.
 *   - $field->handler: The Views field handler object controlling this field. Do not use
 *     var_export to dump this object, as it can't handle the recursion.
 *   - $field->class: The safe class id to use.
 *   - $field->label: The label of the field.
 * - $row: The raw result object from the query, with all data it fetched.
 *
 */
?>
<?php foreach ($fields as $id => $field): ?>
  <div class="project-fields views-field-<?php print $field->class; ?>">
    <?php if ($field->label): ?>
      <label class="views-label-<?php print $field->class; ?>">
        <?php print $field->label; ?>:
      </label>
    <?php endif; ?>
      <span class="field-content">
        <?php if ($field->class == 'title'): ?>
          <?php print '<h2>' . $field->content . '</h2>'; ?>
        <?php else: ?>
          <?php print $field->content; ?>
        <?php endif; ?>
      </span>
  </div>
<?php endforeach; ?>
