<?php
/**
 * @file forums.tpl.php
 * Default theme implementation to display a forum which may contain forum
 * containers as well as forum topics.
 *
 * Variables available:
 *
 * $create_topic_link : the link for create a topic
 * $terms : array of terms
 *
 * 
 * @see template_preprocess_forums()
 * @see isa_forum_theme()
 */
?>

<div class="view-Topic-list">
  <?php if (empty($terms)): ?>
    <div class="view-content">
      <p>There are no forum categories</p>
    </div>
  <?php else: ?>
      <div class="view-content">
        <table>
          <thead>
            <tr>
              <th class="views-field views-field-title"><?php print t('Title'); ?></th>
              <th class="views-field views-field-topic-count"><?php print t('Topics'); ?></th>
              <th class="views-field views-field-post-count"><?php print t('Posts'); ?></th>
              <th class="views-field views-field-last-comment-name"><?php print t('Last post'); ?></th>
            </tr>
          </thead>
          <tbody>
        <?php foreach ($terms as $term): ?>
          <tr class="odd">
            <td class="views-field views-field-title">
              <div class="field field-title">
                <strong><?php print l('<span class="forum-title-icon">' . $term->name . '</span>', $term->url , array('html' => TRUE)); ?></strong>
              </div>
            <?php if (!empty($term->description)): ?>
              <div class="field field-description"><?php print $term->description; ?></div>
            <?php endif ?>
            </td>
            <td class="views-field views-field-topic-count">
              <div class="field field-topic-count"><?php print $term->topics_number; ?></div>
            </td>
            <td class="views-field views-field-post-count">
              <div class="field field-post-count"><?php print $term->posts_number; ?></div>
            </td>
            <td class="views-field views-field-last-comment-name">
            <?php if (!empty($term->last_post_user)): ?>
                <div class="field field-comment-statistics-last-post-url"><em><?php print $term->last_post_url; ?></em></div>
                <div class="field field-comment-statistics-last-comment-name"><?php print t('by ') . '<strong>' . $term->last_post_user . '</strong>'; ?></div>
                <div class="field field-comment-statistics-last-comment-timestamp"><?php print $term->last_post_timestamp; ?></div>
            <?php else: ?>
                  <div class="field field-comment-statistics-no-post">No post</div>
            <?php endif ?>
                </td>
              </tr>
        <?php endforeach; ?>
                </tbody>
              </table>
            </div>
  <?php endif ?>
</div>
