/* $Id: project.js,v 1.3 2009/01/12 20:07:19 dww Exp $ */

Drupal.behaviors.projectAuto = function (context) {
  // The initially selected term, if any.
  var tid;
  $('div.project-taxonomy-element input:not(.projectAuto-processed)', context).addClass('projectAuto-processed').each(function () {
      if (this.checked) {
        tid = this.value;
      }
    })
    .click(function () {
      Drupal.projectSetTaxonomy(this.value);
    });
  Drupal.projectSetTaxonomy(tid);
}

Drupal.projectSetTaxonomy = function (tid) {
  $('div.project-taxonomy-element select').each(function () {
    // If this is the selector for the currently selected
    // term, show it (in case it was previously hidden).
    if (this.id == 'edit-tid-' + tid) {
      // Hide not the select but its containing div (which also contains
      // the label).
      $(this).parents('div.form-item').show();
    }
    // Otherwise, empty it and hide it.
    else {
      // In case terms were previously selected, unselect them.
      // They are no longer valid.
      this.selectedIndex = -1;
      $(this).parents('div.form-item').hide();
    }
  });
}
