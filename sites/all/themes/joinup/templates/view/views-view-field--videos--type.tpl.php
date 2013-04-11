<?php
/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
  global $base_url;
$theme = variable_get ('theme_default', NULL);
 $theme_path = drupal_get_path('theme', $theme);
?>

<div class="field field-data-<?php print $output;?>-image"><img width="70" height="70" alt="<?php print $output; ?>" title="<?php print $output; ?>" src="<?php print $base_url .'/'.$theme_path; ?>/images/logo/<?php print $output; ?>.png" /></div>
