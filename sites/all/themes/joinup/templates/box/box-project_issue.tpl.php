<?php
// $Id: box-project_issue.tpl.php,v 1.3 2007/12/16 21:01:45 goba Exp $

/**
 * @file box-project_issue.tpl.php
 *
 * Theme implementation to display a box.
 *
 * Available variables:
 * $user_picture : user picture
 * $user_name : username
 * $content : Box content
 *
 * @see joinup_preprocess_box()
 */
?>
<?php if ($title): ?>
 	<h3 id="comment-form-title" class="new-comment"><?php print $title ?></h3>
<?php endif; ?>
<div class="node box-id-<?php print $id; ?>">
	<div class="node-content">
		<div class="even nodes-row-first clearfix">
			<div class="colspan-1 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $user_picture; ?></div>
			</div>
			<div class="colspan-7 last fields views-field-topic-infos">
				<div class="field field-content">
					<div class="form-item">
						<label><?php print t('Your name'); ?></label>
						<?php print $user_name; ?>
					</div>
					<?php print $content ?>
				</div>
			</div>
		</div>
	</div>
</div>