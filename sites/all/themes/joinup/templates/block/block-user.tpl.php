<?php // $Id: block-user.tpl.php,v 0.1 2010/01/20 11:17:36 sebastien.millart Exp $ ?>

<!-- BEGIN of the Template: block-user.tpl.php

	Title:		<?php ($block -> subject) ? print $block -> subject : print '(no title)'; ?>,
	Module:		<?php print $block -> module; ?>,
	Delta:		<?php print $block -> delta; ?>,
	Region:		<?php print $block -> region; ?>,
	Block ID:	<?php print $block_id; ?>.
		
-->

<div id="block-<?php print $block -> module .'-'. $block -> delta; ?>" class="clearfix block">
	<h2<?php if ($accessibility_class): print $accessibility_class; endif; ?>><?php print t('Navigation top bar'); ?></h2>
	<?php if (!empty($block -> subject) && $logged_in): ?>
        <h3><?php print t('Welcome') . ' ' . strip_tags(theme('username',user_load(array('name' => $block -> subject)))); ?></h3>
	<?php endif;?>
	<div class="content">
		<?php print $block -> content; ?>
	</div>
</div>

<!-- END of the Template: block-user.tpl.php -->

