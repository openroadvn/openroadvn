/**
 *  This javascript file adds "Highlight" fields under the <select> fields used
 * to choose users/experts to add to a community/workspace.
 */

// global variables for user
var users_select_id = 'edit-og-names';
var users_select_parent_div_id = users_select_id + '-wrapper';
var users_filter_input_id = 'filter-og-names-input';

var users_select;
var users_select_parent_div;
var users_filter_input;

// global variables for experts
var experts_select_id = 'edit-og-experts';
var experts_select_parent_div_id = experts_select_id + '-wrapper';
var experts_filter_input_id = 'filter-og-experts-input';

var experts_select;
var experts_select_parent_div;
var experts_filter_input;

function setVisible(obj, display) {
  if (obj.style) {
    // available alternatives to "hide" an <option>
    // we could disable it
    //if (display) obj.removeAttribute('disabled');
    //else obj.setAttribute('disabled', 'disabled');
    
    // we could hide it, but IE is unable to hide an <option>
    //obj.style.display = display ? '' : 'none';
    
    // we can change its color
    obj.style.color = display ? 'black' : '#cccccc';
  } else {
    obj.visibility = display ? 'visible' : 'hidden';
  }
}

function getText(obj) {
  if (obj.textContent) {
    return(obj.textContent);
  } else {
    return(obj.innerText);
  }
}


function filterUsersSelect() {
  filterSelect(users_select, users_filter_input.value);
}

function filterExpertsSelect() {
  filterSelect(experts_select, experts_filter_input.value);
}

function filterSelect(select_field, filter_value) {
  var select_options = select_field.childNodes;
  var match = new RegExp(filter_value, 'i');
  for (var i = 0 ; i < select_options.length ; ++ i) {
    if (filter_value == '') {
      setVisible(select_options[i], true);
    } else {
      setVisible(select_options[i], getText(select_options[i]).match(match));
    }
  }
}

// adds  fields 
function setupFilterFields() {
  // get the users select field, if any
  users_select = document.getElementById(users_select_id);
  users_select_parent_div = document.getElementById(users_select_parent_div_id);
  if (users_select != null && users_select_parent_div != null) {
    users_filter_input = document.createElement('input');
    users_filter_input.setAttribute('id', users_filter_input_id);
    users_filter_input.onkeyup  = filterUsersSelect;
    
    
    users_select_parent_div.insertBefore(document.createTextNode('Highlight: '), users_select);
    users_select_parent_div.insertBefore(users_filter_input,                     users_select);
    users_select_parent_div.insertBefore(document.createElement('br'),           users_select);
  }
  
  // get the experts select field, if any
  experts_select = document.getElementById(experts_select_id);
  experts_select_parent_div = document.getElementById(experts_select_parent_div_id);
  if (experts_select != null && experts_select_parent_div != null) {
    experts_filter_input = document.createElement('input');
    experts_filter_input.setAttribute('id', experts_filter_input_id);
    experts_filter_input.onkeyup  = filterExpertsSelect;
    
    experts_select_parent_div.insertBefore(document.createTextNode('Highlight: '), experts_select);
    experts_select_parent_div.insertBefore(experts_filter_input,                   experts_select);
    experts_select_parent_div.insertBefore(document.createElement('br'),           experts_select);
  }
}
window.onload = setupFilterFields;
