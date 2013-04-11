/* $Id: theme_editor.js,v 1.1.2.1 2009/03/18 14:28:22 arcaneadam Exp $
*/
$(document).ready(function(){
	
$("#theme_editor_file_backup_form").css("display","none");
var active = 'editor';
$("#backup_tab").click(function(){
	if(active=="editor"){
	$(".theme_editor_tabs > li.active").removeClass("active");
 $(this).parent().addClass("active");
 $(this).removeClass("not_active");
 $("#editor_tab").addClass("not_active");
 $("#theme_editor_file_editor_form").fadeOut("normal",function(){
 	$("#theme_editor_file_backup_form").fadeIn("normal");
 	});
active = "backup";
}
});
$("#editor_tab").click(function(){	
	if(active == "backup"){
	$(".theme_editor_tabs > li.active").removeClass("active");
 $(this).parent().addClass("active");
 $(this).removeClass("not_active")
  $("#backup_tab").addClass("not_active");
 $("#theme_editor_file_backup_form").fadeOut("normal",function(){
 	$("#theme_editor_file_editor_form").fadeIn("normal");
});
active = "editor";
}
});

});
