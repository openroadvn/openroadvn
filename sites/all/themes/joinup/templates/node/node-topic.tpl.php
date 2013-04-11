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
 *
 * --------- Additional variables ---------
 *
 * @see joinup_preprocess_node () :
 *
 * $body : the body of topic (html)
 * $forum : the forum title (linked to forum)
 * $domains : domains list (html)
 * $languages : language list (html)
 * $keywords : keywords list (html)
 * $highlight_link : the link for highlight the topic
 * $statistic_counter : the number of reading content
 * $user_domains : domains list of user (html)
 * $user_languages : languages list of user (html)
 * $user_countries : countries list of user (html)
 * $user_company_name : company name of user
 * $joined_group :
 */
?>
<div id="top" class="node node-Forum-single-topic node-id-<?php print $node->nid; ?><?php if ($sticky) { print ' sticky'; } ?><?php if (!$status) {  print ' node-unpublished'; } ?> content-colspans-2-6 clear-block">
	<div class="node-content">
		<?php if ($flags_view): ?>
			<div class="field field-flags-view"><?php print $flags_view; ?></div>
		<?php endif; ?>
		<div class="item-list">
			<div class="item-number"><?php print $comment_count ?> <?php print t('message(s)'); ?></div>
		</div>
		<div class="odd nodes-row-first nodes-row-last clearfix">
			<div class="colspan-2 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $picture; ?></div>
			</div>
			<div class="colspan-6 last fields views-field-topic-infos">
				<div class="field field-title"><strong><?php print $title; ?></strong></div>
				<div class="field field-created"><?php print $submitted; ?>.</div>
				<div class="field field-users-company-name">(<?php print $user_company_name; ?>, <?php print $user_countries; ?>)<?php if (isset($joined_group)): ?> - <span class="field field-users-joined"><label><?php print t('Joined') ?>:</label> <?php print $joined_group; endif; ?></span></div>
				<div class="field field-forum"><?php print $forum; ?></div>
				<div class="field field-taxonomies"><label><?php print t('Domains'); ?>:</label> <?php print $domains; ?><?php if ($languages): ?> | <label><?php print t('Languages'); ?>:</label> <?php print $languages; ?><?php endif; ?><?php if ($keywords): ?> | <label><?php print t('Keywords'); ?>:</label> <?php print $keywords; ?><?php endif; ?>.</div>
				<div class="field field-rating"><?php print $vote_rating; ?></div>
				<div class="field field-type">
					<div class="quote topic">&quot;</div>
				</div>
				<div class="field field-body"><?php print $node->content['body']['#value']; ?></div>
			</div>
		</div>
	</div>
	<?php if (!empty($node->comment_count)): ?>
		<h3><?php print t('Comments'); ?></h3>
	<?php endif;?>
</div>
