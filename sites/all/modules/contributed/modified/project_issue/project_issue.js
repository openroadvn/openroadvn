function project_issue_autocomplete_handler() {
  $('#edit-project-info-project-title').keyup(function(e) {
    if (e.keyCode == 13) {
      this.blur();
    }
  });
  $('#edit-project-info-project-title').blur(function(e) {
    $(this).parent().after($('<div class="wrapper-throbber">'));
  });
}