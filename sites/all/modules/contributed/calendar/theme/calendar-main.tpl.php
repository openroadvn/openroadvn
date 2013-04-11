<?php
// $Id: calendar-main.tpl.php,v 1.2.2.4 2009/01/10 20:04:18 karens Exp $
/**
 * @file
 * Template to display calendar navigation and links.
 * 
 * @see template_preprocess_calendar_main.
 *
 * $view: The view.
 * $calendar_links: Array of formatted links to other calendar displays - year, month, week, day.
 * $calendar_popup: The popup calendar date selector.
 * $display_type: year, month, day, or week.
 * $mini: Whether this is a mini view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * 
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>

<div class="calendar-calendar">
  <?php if (!empty($calendar_popup)) print $calendar_popup;?>
  <?php if (empty($block)) print theme('links', $calendar_links);?>

  <?php
  if ($display_type == 'year') {

    // generate the html for the calendar navigation
    $nav = theme('date_navigation', $view);

    // define html containers to insert links into
    $prev = '<div class="date-prev">';
    $next = '<div class="date-next">';

    // create link for previous/next year
    $prevLink = l('<< prev', 'calendar/' . (arg(1)-1));
    $nextLink = l('next >>', 'calendar/' . (arg(1)+1));

    // find position of end of previous div container
    $prevPos = stripos($nav,$prev) + strlen($prev);

    // insert previous link
    $nav = substr($nav, 0, $prevPos) . $prevLink . substr($nav, $prevPos);

    // find position of end of next div container
    $nextPos = stripos($nav,$next) + strlen($next);

    // insert next link
    $nav = substr($nav, 0, $nextPos) . $nextLink . substr($nav, $nextPos);

    print $nav;

  } else {
    print theme('date_navigation', $view);
  }
  ?>

</div>