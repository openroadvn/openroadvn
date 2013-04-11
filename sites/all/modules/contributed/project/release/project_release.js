/* $Id: project_release.js,v 1.4 2010/01/17 01:03:58 dww Exp $ */

// IE doesn't support hiding or disabling select options, so we have to rebuild the list. :(
Drupal.projectReleaseRebuildSelect = function() {
  // Remove everything
  recommended = this.value;
  while (this.length > 1) {
    this.remove(1);
  }

  // Now add the choices back.
  choices = this;
  $(this).parents('table:eq(0)').find('input.form-checkbox.supported:checked').each(function () {
    $(this).parents('tr:eq(0)').find('td:first-child').each(function () {
      choices.appendChild(new Option(this.innerHTML, this.innerHTML));
      if (this.innerHTML == recommended) {
        choices.selectedIndex = choices.length-1;
      }
    });
  });

  // If removing a supported version changes the recommended version then highlight it.
  if (this.selectedIndex == 0 && recommended != -1) {
    $(this).parents('table:eq(0)').find('tr:last').css('background-color', '#FFFFAA');
  }
}

Drupal.behaviors.projectReleaseAutoAttach = function () {
  // Set handler for clicking a radio to change the recommended version.
  $('form#project-release-project-edit-form select.recommended').change(function () {
    $(this).parents('table:eq(0)').find('tr:last').css('background-color', '#FFFFAA');
  });
  
  // Set handler for clicking checkbox to toggle a version supported/unsupported.
  $('form#project-release-project-edit-form input.form-checkbox.supported').click(function() {
    $(this).parents('table:eq(0)').find('select').each(Drupal.projectReleaseRebuildSelect);

    if (this.checked) {
      // Marking this version as supported.
      $(this).parents('tr:eq(0)').find('.snapshot').removeAttr('disabled');
    }
    else {
      // Marking this version as unsupported, so disable row.
      $(this).parents('tr:eq(0)').find('.snapshot').attr('disabled','true').removeAttr('checked');
    }
  }).each( function() { // Disable unsupported versions on initial page load.
    if (!this.checked) {
      $(this).parents('tr:eq(0)').find('.snapshot').attr('disabled','true');
    }
  });

  // Go ahead and remove the unavailable choices from the recommended list.
  $('select.recommended').each(Drupal.projectReleaseRebuildSelect);
};
