<h2><?php //print t('Comments awaiting moderation'); ?></h2>
<?php foreach($list as $item): ?>
  <h3><?php print $item['title']; ?></h3>
  <?php print $item['view']; ?>
<?php endforeach; ?>