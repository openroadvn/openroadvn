/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
if(Drupal.jsEnabled)
{   
    $(document).ready(function() {
        if($('#edit-simplenews-digest-enable-yes').attr('checked')==true)
            $('#wrapper-digest').show(1);
        else
            $('#wrapper-digest').hide(1);
        
        $("#edit-simplenews-digest-enable-yes").click( function() {
            $('#wrapper-digest').show(1);
        } );
        $("#edit-simplenews-digest-enable-no").click( function() {
            $('#wrapper-digest').hide(250);
        } );
    }); 
}