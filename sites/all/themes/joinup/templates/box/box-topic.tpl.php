<?php
// $Id: box.tpl.php,v 1.3 2007/12/16 21:01:45 goba Exp $

/**
 * @file box.tpl.php
 *
 * Theme implementation to display a box.
 *
 * Available variables:
 * - $title: Box title.
 * - $content: Box content.
 *  -$joined_group
 * $user_picture : user picture
 * $user_name : username
 * $user_domains : domains list of user (html)
 * $user_languages : languages list of user (html)
 * $user_countries : countries list of user (html)
 * $user_company_name : company name of user
 * @see template_preprocess()
 */
?>
<?php if ($title): ?>
 	<h3 id="comment-form-title" class="new-comment"><?php print $title ?></h3>
<?php endif; ?>
<div class="node node-Forum-single-topic box-id-<?php print $id; ?>">
	<div class="node-content">
		<div class="even nodes-row-first clearfix">
			<div class="colspan-1 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $user_picture; ?></div>
			</div>
			<div class="colspan-7 last fields views-field-topic-infos">
				<div class="field field-content"><?php print $content ?></div>
			</div>
		</div>
	</div>
</div>
