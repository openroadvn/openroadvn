<?php
// $Id: anoymous_validation_page.tpl.php

/**
 * @file anoymous_validation_page.tpl.php
 *
 *
 * Available variables:
 * $vars['info_text_login'] : information to login for anonymous user when he want download release
 * $vars['login_link'] : link to login page
 * $vars['info_text_download'] : information about the anonymous download
 * $vars['file_link'] : the file link
 * $node->taxonomy_terms : Community taxonomies
 * $node->vfs : related virtual forges names
 * $node->user_status : Community push link(s)
 * $node->members_count : number of members
 * $node->rating : the fivestar form
 *
 * @see isa_private_files_anonymous_page()
 */
?>

<div class="anonymous-release-file clearfix">
  <?php if ($vars['info_text_login']): ?>
    <div class ="field-text field-text-login">
      <?php print $vars['info_text_login']; ?>
    </div>
  <?php endif; ?>
  <?php if ($vars['login_link']): ?>
    <div class="field-login ">
    <?php print $vars['login_link']; ?>
    </div>
  <?php endif; ?>
  <?php if ($vars['file_link']): ?>
    <div class="field-link">
      <?php print $vars['file_link']; ?>
    </div>
  <?php endif; ?>
</div>