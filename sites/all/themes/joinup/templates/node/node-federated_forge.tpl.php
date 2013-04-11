<?php
// $Id: node-federated_forge.tpl.php,v 1.4.2.1 2009/08/10 10:48:33 goba Exp $

/**
 * @file node-federated_forge.tpl.php
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
 * @see joinup_preprocess_node()
 */
?>
<div id="node-<?php print $node->nid; ?>" class="node node-type-<?php print $node->type ?> <?php if ($sticky) {
  print ' sticky';
} ?> <?php if (!$status) {
      print ' node-unpublished';
    } ?> clear-block">
  <div class="node-content">
<?php if ($flags_view): ?>
      <div class="field field-flags-view"><?php print $flags_view; ?></div>
      <?php endif; ?>
    <div class="field field-submitted"><?php print $submitted; ?></div>
    <div class="field field-mission-description">
      <h3><?php print t('Description'); ?></h3>
      <?php print $node->content['body']['#value']; ?>
    </div>
    <?php /* a block already display get involved
     * if (!empty($node->field_forges_get_involved['0']['value'])): ?>
      <h3><?php print t('Get involved'); ?></h3>
      <div class="field field-get-involved">
  <?php print $node->field_forges_get_involved['0']['value']; ?>
      </div>
          <?php endif; */?>
    <div id="node-information" class="box information">
      <h3 class="accessibility-info"><?php print t('Information'); ?></h3>
      <div class="odd nodes-row-first nodes-row-last clearfix">
        <dl class="colspans-3-5 first last fields">
          <?php if ($field_forges_homepage['0']['view']): ?>
            <dt class="field field-homepage-term"><?php print t('Homepage'); ?>:</dt>
            <dd class="field field-homepage-description"><?php print $field_forges_homepage['0']['view']; ?></dd>
          <?php endif; ?>
          <?php if ($field_forges_xml['0']['view']): ?>
            <dt class="field field-xml-term"><?php print t('XML'); ?>:</dt>
            <dd class="field field-xml-description"><?php print $field_forges_xml['0']['view']; ?></dd>
          <?php endif; ?>
          <?php foreach ($translations as $language => $link): ?>
            <?php if ($link): ?>
              <dt class="field field-translations-<?php print strtolower($language); ?>-term"><?php print t($language); ?>:</dt>
              <dd class="field field-translations-<?php print strtolower($language); ?>-description"><?php print $link; ?></dd>
  <?php endif; ?>
<?php endforeach; ?>
        </dl>
      </div>
    </div>
  </div>
</div>
