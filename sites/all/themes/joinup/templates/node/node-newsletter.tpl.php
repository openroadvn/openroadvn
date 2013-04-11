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
* $group_type : the type of group (software)
*
* -- taxonomie --
 * $statistics_counter
* $taxonomy_terms : array of terms by vocabulary name
* $author : author lastname and firstname
* flag_count
*
* @see _joinup_preprocess_newsletter()
* @see joinup_preprocess_node()
*/
?>
<div id="node-<?php print $node->nid; ?>" class="node node-type-<?php print $node->type?> <?php if ($sticky) { print ' sticky'; } ?> <?php if (!$status) { print ' node-unpublished'; } ?> bg-content clear-block">
	<div class="node-content">
		<div class="field field-submitted"><?php print $submitted; ?></div>
		<div class="field field-vote-rating"><?php print $vote_rating; ?></div>
		<div class="field field-title">
			<h2><?php print $title ?></h2>
		</div>
		<div class="field field-content-body"><?php print $node->content['body']['#value']; ?></div>
		<div id="node-information" class="box information">
			<h3 class="accessibility-info"><?php print t('Information'); ?></h3>
			<div class="odd nodes-row-first nodes-row-last clearfix">
				<dl class="colspans-2-5 push-1 last fields">
					<?php foreach ($taxonomy_terms as $vocab => $terms): ?>
						<?php if ($terms): ?>
							<dt class="field field-taxonomy-<?php print strtolower($vocab); ?>-term"><?php print t($vocab); ?>:</dt>
							<dd class="field field-taxonomy-<?php print strtolower($vocab); ?>-description"><?php print $terms; ?></dd>
						<?php endif; ?>
					<?php endforeach; ?>
				</dl>
			</div>
		</div>
	</div>
</div>
