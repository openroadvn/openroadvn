<?php
// $Id: node.tpl.php,v 1.4.2.1 2009/08/10 10:48:33 goba Exp $

/**
 * @file node.tpl.php
 *
 * Theme implementation to display a node.
 *
 * Available variables:
 * - $title: the (sanitized) title of the node.
 * - $content: Node body or teaser depending on $teaser flag.
 * - $picture: The authors picture of the node output from
 *   theme_user_picture().
 * - $date: Formatted creation date (use $created to reformat with
 *   format_date()).
 * - $links: Themed links like "Read more", "Add new comment", etc. output
 *   from theme_links().
 * - $name: Themed username of node author output from theme_username().
 * - $node_url: Direct url of the current node.
 * - $terms: the themed list of taxonomy term links output from theme_links().
 * - $submitted: themed submission information output from
 *   theme_node_submitted().
 *
 * Other variables:
 * - $node: Full node object. Contains data that may not be safe.
 * - $type: Node type, i.e. story, page, blog, etc.
 * - $comment_count: Number of comments attached to the node.
 * - $uid: User ID of the node author.
 * - $created: Time the node was published formatted in Unix timestamp.
 * - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
 *   teaser listings.
 * - $id: Position of the node. Increments each time it's output.
 *
 * Node status variables:
 * - $teaser: Flag for the teaser state.
 * - $page: Flag for the full page state.
 * - $promote: Flag for front page promotion state.
 * - $sticky: Flags for sticky post setting.
 * - $status: Flag for published status.
 * - $comment: State of comment settings for the node.
 * - $readmore: Flags true if the teaser content of the node cannot hold the
 *   main body content.
 * - $is_front: Flags true when presented in the front page.
 * - $logged_in: Flags true when the current user is a logged-in member.
 * - $is_admin: Flags true when the current user is an administrator.
 *
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
?>

<div id="node-<?php print $node->nid; ?>" class="node node-type-<?php print $node->type ?> <?php if ($sticky) { print ' sticky'; } ?> <?php if (!$status) { print ' node-unpublished'; } ?> clear-block">
	<div class="node-content">
      <?php if ($page): ?>
		<?php if ($flags_view): ?>
			<div class="field field-flags-view"><?php print $flags_view ?></div>
		<?php endif; ?>
		<?php if ($submitted): ?>
			<div class="field field-submitted"><?php print $submitted ?></div>
		<?php endif; ?>
		<?php if ($vote_rating): ?>
			<div class="field field-vote-rating"><?php print $vote_rating; ?></div>
		<?php endif; ?>
		<div class="field field-mission-description">
			<h3><?php print t('Description'); ?></h3>
			<?php print $node->content['og_mission']['#value']; ?>
		</div>
		<?php if (!empty($field_community_expectations['0']['value'])): ?>
			<div class="field field-expectations">
				<h3><?php print t('Expectations'); ?></h3>
				<?php print $field_community_expectations['0']['value']; ?>
			</div>
		<?php endif; ?>
	  <?php else: ?>
        <?php if ($edit_link): ?><div class="edit-link"><?php print $edit_link; ?> </div><?php endif; ?>
      <?php endif; ?>
		<div id="node-information" class="box information">
			<h3 class="accessibility-info"><?php print t('Information'); ?></h3>
			<div class="odd nodes-row-first nodes-row-last clearfix">
				<dl class="colspans-4-4 first last fields">
					<?php if (!empty($field_community_sponsor_logo['0']['view'])): ?>
						<dt class="field field-sponsor-logo-term"><?php print t('Sponsor logo'); ?>:</dt>
						<dd class="field field-sponsor-logo-description"><?php print $field_community_sponsor_logo['0']['view']; ?>&nbsp;</dd>
					<?php endif; ?>
						<dt class="field field-abstract-term"><?php print t('Abstract') ?>:</dt>
						<dd class="field field-dabstract-description"><?php print $node->og_description; ?></dd>
					<?php if ($mailing_list): ?>
						<dt class="field field field-mailing-list-term"><?php print t('Mailing list') ?>:</dt>
						<dd class="field field field-mailing-list-description"><?php print $mailing_list; ?></dd>
					<?php endif; ?>
					<?php if (!empty($field_community_estimated_launch['0']['view'])): ?>
						<dt class="field field-estimate-members-launch-term"><?php print t('Estimated members at launch') ?>:</dt>
						<dd class="field field-estimate-members-launch-description"><?php print $field_community_estimated_launch['0']['view']; ?></dd>
					<?php endif; ?>
					<?php if (!empty($node->field_community_estimated_after['0']['view'])): ?>
						<dt class="field field-estimate-members-after-term"><?php print t('Estimated members after 1 year') ?>:</dt>
						<dd class="field field-estimate-members-after-description"><?php print $node->field_community_estimated_after['0']['view']; ?></dd>
					<?php endif; ?>
					<?php if (!empty($field_community_url['0']['view'])): ?>
						<dt class="field field-existing-url-term"><?php print t('Existing URL on the Internet') ?>:</dt>
						<dd class="field field-existing-url-description"><?php print $field_community_url['0']['view']; ?></dd>
					<?php endif; ?>
				</dl>
			</div>
		</div>
	</div>
</div>
