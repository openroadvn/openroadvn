<?php
// $Id: views-view--block.tpl.php,v 0.00.0.1 2011/02/02 sebastien.millart Exp $
/**
 * @file views-view--block.tpl.php
 * Main view template
 * @ingroup views_templates
 */
?>

<div class="<?php print $classes; ?>">
	<?php if ($admin_links): ?>
		<div class="views-admin-links views-hide">
			<?php print $admin_links; ?>
		</div>
	<?php endif; ?>
	<?php if ($more): ?>
		<?php print $more; ?>
	<?php endif; ?>
	<?php if ($header): ?>
		<div class="view-header">
			<?php print $header; ?>
		</div>
	<?php endif; ?>
	<?php if ($exposed): ?>
		<div class="view-filters">
			<?php print $exposed; ?>
		</div>
	<?php endif; ?>
	<?php if ($attachment_before): ?>
		<div class="attachment attachment-before">
			<?php print $attachment_before; ?>
		</div>
	<?php endif; ?>
	<?php if ($rows): ?>
		<div class="view-content">
			<?php print $rows; ?>
		</div>
	<?php elseif ($empty): ?>
		<div class="view-empty">
			<?php print $empty; ?>
		</div>
	<?php endif; ?>
	<?php if ($pager): ?>
		<?php print $pager; ?>
	<?php endif; ?>
	<?php if ($attachment_after): ?>
		<div class="attachment attachment-after">
			<?php print $attachment_after; ?>
		</div>
	<?php endif; ?>
	<?php if ($footer): ?>
		<div class="view-footer">
			<?php print $footer; ?>
		</div>
	<?php endif; ?>
	<?php if ($feed_icon): ?>
		<div class="feed-icon">
			<?php print $feed_icon; ?>
		</div>
	<?php endif; ?>
</div>
<?php /* class view */ ?>
