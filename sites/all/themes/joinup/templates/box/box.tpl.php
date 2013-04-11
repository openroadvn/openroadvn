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
 *
 * @see template_preprocess()
 */

?>
<?php if ($title): ?>
 	<h3 id="comment-form-title" class="new-comment"><?php print $title ?></h3>
<?php endif; ?>
<div class="node box-id-<?php print $id; ?>">
	<div class="node-content">
		<div class="even nodes-row-first clearfix">
			<?php if ($user_picture): ?>
			<div class="colspan-1 first fields nodes-field-user-infos">
				<div class="field field-users-photo"><?php print $user_picture; ?></div>
			</div>
            <?php endif; ?>
			<div class="colspan-7 last fields views-field-topic-infos">
				<div class="field field-content"><?php print $content ?></div>
			</div>
		</div>
	</div>
</div>