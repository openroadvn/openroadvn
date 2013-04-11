<?php
/**
* $author Link to author profile.
* $comment (object) Comment object as passed to the theme_comment function.
* $content  Content of link.
* $date Formatted date for post.
* $directory  The directory the theme is located in, e.g. themes/garland or themes/garland/minelli.
* $id The sequential ID of the comment being displayed.
* $is_front True if the front page is currently being displayed.
* $links  Contextual links below comment.
* $new  Translated text for 'new', if the comment is in fact new.
* $picture  User picture HTML (include <a> tag.) , if display is enabled and picture is set.
* $submitted  Translated post information string.
* $title  Link to the comment title.
* $zebra  Alternates between odd/even in a list
*
*--------- Additional variables ---------
*
* @see joinup_preprocess_comment
* $user_countries : countries list of user (html)
* $user_company_name : company name of user
* $joined_group :
* $links : @see isa_toolbox_link_alter ()
*/
?>
<div class="node node-Forum-single-topic comment-id-<?php print $id; ?>">
	<div class="node-content">
		<div class="even <?php if ($id == 1): print 'nodes-row-first'; endif; ?> clearfix">
			<div class="colspan-1 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $comment->picture; ?></div>
			</div>
			<div class="colspan-7 last fields views-field-topic-infos">
				<div class="field field-comment-links"><?php print $links; ?></div>
				<div class="field field-title"><strong>Re: <?php print $node->title; ?></strong></div>
				<div class="field field-created"><?php print $submitted; ?></div>
				<div class="field field-users-company-name">(<?php print $user_company_name; ?>, <?php print $user_countries; ?>)<?php if (isset($joined_group)): ?> - <span class="field field-users-joined"><label><?php print t('Joined') ?>:</label> <?php print $joined_group; endif; ?></span></div>
				<div class="field field-type">
					<div class="quote topic">&quot;</div>
				</div>
				<div class="field field-body"><?php print $content; ?></div>
			</div>
		</div>
	</div>
</div>
