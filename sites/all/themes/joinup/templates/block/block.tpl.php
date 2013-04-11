<?php // $Id: block.tpl.php,v 0.1 2010/01/19 15:44:36 sebastien.millart Exp $ ?>

<!-- BEGIN of the Template: block.tpl.php

	Title:		<?php ($block -> subject) ? print $block -> subject : print '(no title)'; ?>,
	Module:		<?php print $block -> module; ?>,
	Delta:		<?php print $block -> delta; ?>,
	Region:		<?php print $block -> region; ?>,
	Block ID:	<?php print $block_id; ?>.

-->

<div id="block-<?php print $block -> module .'-'. $block -> delta; ?>" class="block<?php if ($block_class): print ' ' . $block_class; endif; ?> clearfix">
	<?php if (!empty($block -> subject)): ?>
		<<?php print $heading_type; ?><?php if ($accessibility_class): print $accessibility_class; endif; ?>><?php print $block -> subject; ?></<?php print $heading_type; ?>>
	<?php endif;?>
	<div class="content">
		<?php print $block -> content; ?>
	</div>
</div>

<!-- END of the Template: block.tpl.php -->

