/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
if(Drupal.jsEnabled)
{
     
    $(document).ready(function() {
        $("#edit-project-release-files-addmf").css('display', 'block'); //display only if js is actived
        $("#edit-project-release-files-addmf").click( function() {
            var link_id = this.name;
            link_id = parseInt(link_id)
            link_id++
            $("#edit-project-release-files-file-wrapper").append('<label>File:</label><input class="form-file" id="edit-project-release-files-file'+link_id+'"  size="60" type="file" name="files[project_release_files'+link_id+']" />');
            this.name = link_id
            return false
        } );
    }); 
}