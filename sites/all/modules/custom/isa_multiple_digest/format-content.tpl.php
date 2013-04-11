<?php

// $Id: format_content.tpl.php,v 1.4.2.1 2011/07/07 11:15:33 goba Exp $


/**
*
* $body_view
* $date_view
* $title_view
*
*
*
*
*/

?>
<div>
<?php print $title_view;?>
</div>
<?php if (isset($date_view)):?>
<div style="color:#969696 ; font-size:0.8em;">
  <?php print $date_view;?>
</div>
 <?php endif;?>

<p style="margin : 0em 1.2em 1.2em 0em">
<?php print $body_view ;?>
</p>


