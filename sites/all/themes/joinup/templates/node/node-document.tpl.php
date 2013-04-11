<?php
// $Id: node-document.tpl.php,v 1.4.2.1 2010/05/09 13:37:33 smillart Exp $

/**
 * @file node-document.tpl.php
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
<div id="node-<?php print $node->nid; ?>" class="node<?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) { print ' node-unpublished'; } ?> node-type-<?php print $node->type; ?> clear-block">
	<div class="node-content">
		<?php if ($flags_view): ?>
			<div class="field field-flags-view"><?php print $flags_view ?></div>
		<?php endif; ?>
		<div class="field field-submitted"><?php print $submitted;?></div>
		<div class="field field-vote-rating"><?php print $vote_rating; ?></div>
		<div class="field field-title">
			<h2><?php print $title ?></h2>
		</div>
		<div class="field field-content-body"><?php print $node->content['body']['#value']; ?></div>
        
		<?php if (isset($additional_doc)): ?>
			<div class="field field-documentation">
				<h3><?php print t('Additional documentation'); ?></h3>
				<ul>
					<?php foreach ($additional_doc as $key => $value): ?>
						<?php if ($value): ?>
							<li>
                <?php print $value; ?>
              </li>
						<?php endif; ?>
					<?php endforeach; ?>
				</ul>
			</div>
		<?php endif; ?>
		<div id="node-information" class="box information">
			<h3 class="accessibility-info"><?php print t('Information'); ?></h3>
			<div class="odd nodes-row-first nodes-row-last clearfix">
				<dl class="colspans-3-5 first last fields">
					<?php foreach ($document_info as $vocab => $terms): ?>
						<?php if ($terms): ?>
							<dt class="field field-document-info-term"><?php print t($vocab); ?>:</dt>
							<dd class="field field-document-info-description"><?php print $terms; ?></dd>
						<?php endif; ?>
					<?php endforeach; ?>
					<?php if (!empty($original_url['url'])): ?>
						<dt class="field field-original-url-term"><?php print t('Original URL') ?>:</dt>
						<dd class="field field-original-url-description"><a href="<?php print $original_url['url']; ?>"><?php (!empty($original_url['title']))? print $original_url['title'] : print $original_url['url']; ?></a></dd>
					<?php endif; ?>
					<?php 
						if ($taxonomy_terms && !empty ($taxonomy_terms)):?>
					<?php foreach ($taxonomy_terms as $vocab => $terms): ?>
						<?php if ($terms): ?>
							<dt class="field field-taxonomy-<?php print strtolower($vocab); ?>-term"><?php print t($vocab); ?>:</dt>
							<dd class="field field-taxonomy-<?php print strtolower($vocab); ?>-description"><?php print $terms; ?></dd>
						<?php endif; ?>
					<?php endforeach; ?>
					<?php endif; ?>
                    <?php if ($email_contact): ?>
                      <dt class="field field-email"><?php print t('Email contact') ?>:</dt>
                      <dd class="field field-email-description"><?php print $email_contact; ?></dd>
                    <?php endif; ?>
				</dl>
			</div>
		</div>
	</div>
</div>
