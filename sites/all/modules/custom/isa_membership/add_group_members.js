/**
 * This javascript file provides two functions used in the workspace and
 * community membership forms to ensure the numerous checkboxes remain
 * consistent.
 */

/**
 * This function has to be called when a non-default role checkbox is checked
 * for a given member. It ensure the default role checkbox is also checked for
 * that same user.
 * @param user_id id of the unchecked user
 * @param roles_array array of role ids that have to be unchecked
 * @param orig_checkbox Original unchecked checkbox
 */
function ensureUserIsCheked(user_id, orig_checkbox) {
  if (orig_checkbox.checked) {
    var target_checkbox = 'edit-user-roles-' + user_id + '-roles-default-role';
    document.getElementById(target_checkbox).checked = true;
  }
}

/**
 * This function has to be called when the default role checkbox is unchecked
 * for a given member. It unchecks the other roles for that same user.
 * @param user_id id of the unchecked user
 * @param roles_array array of role ids that have to be unchecked
 * @param orig_checkbox Original unchecked checkbox
 */
function ensureUserHasNoRole(user_id, roles_array, orig_checkbox) {
  if (!orig_checkbox.checked) {

    for (i = 0 ; i < roles_array.length ;  i++) {
		
      var target_checkbox = 'edit-user-roles-' + user_id + '-roles-' + roles_array[i];
      document.getElementById(target_checkbox).checked = false;
	  }
 
  }
}
