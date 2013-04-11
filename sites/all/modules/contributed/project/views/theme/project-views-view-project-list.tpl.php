<?php
// $Id: project-views-view-project-list.tpl.php,v 1.1 2009/01/12 20:34:07 dww Exp $
/**
 * @file project-views-view-project-list.tpl.php
 * Default simple view template to display a list of project rows.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * - $project['arguments'][0] is an array containing the sanitized properties
 *     of the term used as the first argument, if one is present.  Available
 *     keys include tid, vid, name, description, and weight.
 */
?>
<?php if (!empty($project['arguments'][0]['description'])) : ?>
  <p>
  <?php print $project['arguments'][0]['description']; ?>
  </p>
<?php endif; ?>

  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>

    <?php foreach ($rows as $row): ?>
      <?php print $row ?>
    <?php endforeach; ?>

