<?php
// $Id: project-views-view-row-project-node.tpl.php,v 1.1 2009/01/12 20:34:07 dww Exp $
/**
 * @file project-views-view-row-project-node.tpl.php
 * Default view template to display a single project node.
 *
 * The following variables are available:
 * - $options['project_teaser']: If set only the project node's teaser should be displayed.
 * - $options['project_term_links']: If set the project term links should be displayed.
 * - $options['project_release_download_table']: If set the release download table should be displayed.
 * - $options['project_release_download_link']: If set a link to directly download the most recent version should be displayed.
 * - $options['project_issue_issues_link']: If set a link to the project's issue queue should be displayed.
 *
 * - $project:  This is a version of the project node object with only a few selected fields, all of which have been sanitized.
 *     Note:  Several of these properties will not be set if the corresponding option was not set.
 *     The following properties are available:
 *       ->class: odd or even, useful for zebra striping.  Will always be present.
 *       ->terms: themed links to terms associated with the node.  May be absent.
 *       ->title: linked title of the project node.  Will always be present.
 *       ->changed: text to display to indicate the last time the node was changed/modified.
 *       ->body: the rendered project body (or, teaser, if $options['project_teaser'] is set).  Will always be present.
 *       ->download_table: the rendered release download table.  May be absent.
 *       ->issues_link: Link to the project's issue queue.  May be absent.
 *       ->new_date:  if set, the changed time of this project is different enough from the last
 *                    project that a header containing the date should be displayed.
 */
?>
<?php if (!empty($project->new_date)) { ?>
  <h3><?php print format_date($project->new_date, 'custom', 'F j, Y'); ?></h3>  
<?php } ?>

<div class="<?php print $project->class; ?>">
  <h2><?php print $project->title; ?></h2>
  <?php if (!empty($project->changed)) { print '<p><small>' . $project->changed . '</small></p>'; } ?>
  <?php print $project->body; ?>
  <?php if (!empty($project_release->download_table)) { print $project_release->download_table; } ?>
  <?php if (!empty($project->terms)) { print $project->terms; } ?>
</div>
