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
 * --------- Additional variables ---------
 * 
 * $taxonomy_terms : array of terms by vocabulary name
 * @see template_preprocess()
 * @see template_preprocess_node()
 */
?>

<div id="node-<?php print $node->nid; ?>" class="node node-type-<?php print $node->type?> <?php if ($sticky) { print ' sticky'; } ?> <?php if (!$status) { print ' node-unpublished'; } ?> clear-block">
	<div class="node-content">
      <?php if ($page): ?>
		<?php if ($flags_view): ?>
			<div class="field field-flags-view"><?php print $flags_view; ?></div>
		<?php endif; ?>
		<div class="field field-submitted"><?php print $submitted; ?></div>
		<div class="field field-vote-rating"><?php print $vote_rating; ?></div>
		<div class="field field-usage -count"><?php print $i_use_this_project ?></div>
		<div class="field field-mission-description">
			<h3><?php print t('Description'); ?></h3>
			<?php print $node->content['og_mission']['#value']; ?>
		</div>
	  <?php else: ?>
        <?php if ($edit_link): ?><div class="edit-link"><?php print $edit_link; ?> </div><?php endif; ?>
      <?php endif; ?>
		<div id="node-information" class="box information">
			<h3 class="accessibility-info"><?php print t('Information'); ?></h3>
			<div class="odd nodes-row-first nodes-row-last clearfix">
				<dl class="colspans-3-5 first last fields">
					<?php  if(!empty($node->og_description)):?>
						<dt class="field field-description-term"><?php print t('Abstract') ?>:</dt>
						<dd class="field field-description-description"><?php print $node->og_description; ?></dd>
					<?php endif; ?>
					<?php if(!empty($node->field_project_common_contact['0']['view'])):?>
						<dt class="field field-email-contact-term"><?php print t('Email contact') ?>:</dt>
						<dd class="field field-email-contact-description"><?php print isa_toolbox_protect_email ($node->field_project_common_contact['0']['view']); ?></dd>
					<?php endif;?>
					<?php if($mailing_list):?>
						<dt class="field field-mailing-list-term"><?php print t('Mailing list') ?>:</dt>
						<dd class="field field-mailing-list-description"><?php print $mailing_list; ?></dd>
					<?php endif;?>
					<?php if ($maven_url): ?>
						<dt class="field field field-maven-term"><?php print t('Maven') ?>:</dt>
						<dd class="field field field-maven-description"><?php print $maven_url; ?></dd>
					<?php endif; ?>
					<?php if ($svn_url): ?>
						<dt class="field field field-svn-term"><?php print t('SVN Directory') ?>:</dt>
						<dd class="field field field-svn-description"><?php print $svn_url; ?></dd>
					<?php endif; ?>
					<?php  if(!empty($node->field_project_asset_owner['0']['value'])):?>
						<dt class="field field-asset-owner-term"><?php print t('Asset owner') ?>:</dt>
						<dd class="field field-asset-owner-description"><?php print $node->field_project_asset_owner['0']['value']; ?></dd>
					<?php endif; ?>
					<?php if(!empty($taxonomy_terms)):?>
						<?php foreach ($taxonomy_terms as $vocab => $terms): ?>
							<?php if ($terms): ?>
								<dt class="field field-taxonomy-<?php print strtolower($vocab); ?>-term"><?php print t($vocab); ?>:</dt>
								<dd class="field field-taxonomy-<?php print strtolower($vocab); ?>-description"><?php print $terms; ?></dd>
							<?php endif; ?>
						<?php endforeach; ?>
					<?php endif;?>
				</dl>
			</div>
		</div>
	</div>
</div>
