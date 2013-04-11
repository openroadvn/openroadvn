// $id Javascript portion for the Context Help Module

Drupal.contexthelp = {};

if (Drupal.jsEnabled) {
  $(document).ready(function() {
    var txt = $('#help-content-0').find('div.item-list ul li').html();
    $('#help-content-0').find('div.item-list').children().remove();
    $('#help-content-0').find('div.item-list').html(txt);

    // Contextual help
    var helpToggle = $('#contextual-help-toggle');
    var helpDialog = $('#contextual-help-dialog');
    // If they do not exist then user did not have permissions to view context help
    if (helpToggle.size() > 0 && helpDialog.size() > 0) {
      var helpInitialized = false;
      helpDialog.jqm({trigger: helpToggle, onShow: function(hash) {
        // Defer parsing the DOM till when the dialog is shown for the first time
        if (!helpInitialized) {
          helpDialog.find('#contextual-help-tabs li').click(function() {
            var index = $(this).attr('id').split('-').pop();
            var content = helpDialog.find('#help-content-' + index);

            $(this).siblings().removeClass('active');
            $(this).addClass('active');

            content.siblings().hide();
            content.show();
          });
          // Request all help from the system
          Drupal.contexthelp.retrieveHelp(helpDialog);
          // Request any FAQs from the system
          Drupal.contexthelp.retrieveFAQs(helpDialog);
          helpInitialized = true;
          // If no help content and user is not an admin show 2nd tab
          if (typeof Drupal.contexthelp.noContentHelp != 'undefined') {
            helpDialog.find('#contextual-help-tabs li#help-tab-1').click();
          }
        }
        $('html').addClass('modal');
        hash.w.css('opacity', 1).show();
      }, onHide: function(hash) {
        $('html').removeClass('modal');
        hash.w.css('opacity', 0).hide();
        hash.o.remove();
      }});
      // Checking if we have any context help or not on this page
      if (typeof Drupal.settings.contexthelp != 'undefined' && typeof Drupal.settings.contexthelp.currentpagehelp != 'undefined') {
        helpDialog.addClass('contextual-help-available');
      }
      // We don't have help for this page so check if the administrator wants the button hide
      if (typeof Drupal.settings.contexthelp != 'undefined' && typeof Drupal.settings.contexthelp.hide_help_button != 'undefined') {
        helpToggle.hide();
      }
    }
  });
}

/**
 * Retrieve all of the Help Topics in the system
 */
Drupal.contexthelp.retrieveHelp = function(helpDialog) {
  // If all help is disabled we do not need to do the request for the data
  if (typeof Drupal.settings.contexthelp != 'undefined' && typeof Drupal.settings.contexthelp.ajax_url != 'undefined' && typeof Drupal.settings.contexthelp.curl != 'undefined') {
    $.ajax({
      type: "POST",
      url: Drupal.settings.contexthelp.ajax_url,
      data: { url: Drupal.settings.contexthelp.curl },
      success: function(data) {
        helpDialog.find('#help-content-1').html(data);
        // Turn help popup views into accordions
        helpDialog.find('#help-content-1').find('ul').accordion({
          header: 'div.views-field-title',
          animated: false,
          autoheight: false,
          alwaysOpen: false,
          active: false
        });
      },
      error: function(xhr, status, thrown) {
        // Error occurred but we do not want to pop up alert or anything here
      }
    });
  }
}

/**
 * Retrieve all of the FAQs in the system
 */
Drupal.contexthelp.retrieveFAQs = function(helpDialog) {
  // If FAQ is disabled we do not need to do the request for the data
  if (typeof Drupal.settings.contexthelp != 'undefined' && typeof Drupal.settings.contexthelp.faq_url != 'undefined') {
    $.ajax({
      type: "POST",
      url: Drupal.settings.contexthelp.faq_url,
      success: function(data) {
        helpDialog.find('#help-content-2').html(data);
        // Turn help popup views into accordions
        helpDialog.find('#help-content-2').find('ul').accordion({
          header: 'div.views-field-title',
          animated: false,
          autoheight: false,
          alwaysOpen: false,
          active: false
        });
      },
      error: function(xhr, status, thrown) {
        // Error occurred but we do not want to pop up alert or anything here
      }
    });
  }
}
