<?php
// $Id: block-introduction-details.tpl.php

/**
 * @file block-introduction-details.tpl.php
 *
 * Theme implementation to display introduction details.
 *
 * Available variables:
 * $node->title       : General title
 * $node->picture     : Default picture
 * $node->description : General description
 * $node->rating      : Fire Stars rating
 * $node->taxonomy_terms : Taxonomies
 * $node->group_status   : Group status (moderated or public)
 * $node->user_status    : User status (is a member or not)
 *
 * @see hook_block_introduction_details()
 */
?>
<div id="introduction" class="clearfix">
	<div class="field field-picture"><?php print $node->picture; ?></div>
    <div id="introduction-fields" class="clearfix">
        
        <?php if (isset($node->rating)): ?>
        <div id="introduction-title" class="clearfix">
        <h2 class="field field-title-rating"><?php print $node->title; ?></h2>
            <div class="bracket">(</div>
                <?php if ($node->rating): ?>
                    <div class="field field-rating"><?php print $node->rating; ?></div>
                <?php endif; ?>
            <div class="bracket">)</div>
        </div>
        <div class="fields fields-details-after-rating">
        <?php else: ?>
            <h2 class="field field-title"><?php print $node->title; ?></h2>
            <div class="fields fields-details">
        <?php endif; ?>
          <?php if ($node->description): ?>
              <div class="field field-description"><?php print $node->description; ?></div>
          <?php endif; ?>
          <?php if (isset($node->group_status)): ?>
              <div class="field field-flag-group-status" title="<?php print $node->group_status; ?>"><strong><?php print $node->group_status; ?></strong></div>
          <?php endif; ?>
          <?php if (isset($node->user_status)): ?>
              <div class="field field-flag-member" title="<?php print $node->user_status; ?>"><strong><?php print $node->user_status; ?></strong></div>
          <?php endif; ?>
          <?php if (isset($node->taxonomy_terms)): ?>
              <?php foreach($node->taxonomy_terms as $vocab => $terms): ?>
                  <?php if ($terms): ?>
                      <div class="field field-taxonomy">
                          <p><label><?php print ucfirst(t($vocab)); ?>:</label>&nbsp;<?php print $terms; ?></p>
                      </div>
                  <?php endif; ?>
              <?php endforeach; ?>
          <?php endif; ?>
      </div>
    </div>
</div>
