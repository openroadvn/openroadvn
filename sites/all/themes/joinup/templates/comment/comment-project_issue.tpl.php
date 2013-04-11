<?php
// $Id: comment-project_issue.tpl.php,v 1.4.2.1 2008/03/21 21:58:28 goba Exp $

/**
 * @file comment-project_issue.tpl.php
 * Default theme implementation for comments.
 *
 * Available variables:
 * - $author: Comment author. Can be link or plain text.
 * - $content: Body of the post.
 * - $date: Date and time of posting.
 * - $links: Various operational links.
 * - $new: New comment marker.
 * - $picture: Authors picture.
 * - $signature: Authors signature.
 * - $status: Comment status. Possible values are:
 *   comment-unpublished, comment-published or comment-preview.
 * - $submitted: By line with date and time.
 * - $title: Linked title.
 *
 * These two variables are provided for context.
 * - $comment: Full comment object.
 * - $node: Node object the comments are attached to.
 *
 * @see template_preprocess_comment()
 * @see theme_comment()
 */
?>
<div class="node comment-id-<?php print $id; ?> <?php print ($comment->new) ? ' comment-new' : ''; print ' '. $status ?>">
	<div class="node-content">
		<div class="even <?php if ($id == 1): print 'nodes-row-first'; endif; ?> clearfix">
			<div class="colspan-1 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $comment->picture; ?></div>
			</div>
			<div class="colspan-7 last fields views-field-topic-infos">
				<div class="field field-comment-links"><?php print $links; ?></div>
				<div class="field field-title"><strong><?php print $title ?></strong></div>
				<div class="field field-created"><?php print $submitted; ?></div>
				<div class="field field-body"><?php print $content; ?></div>
			</div>
		</div>
	</div>
</div>