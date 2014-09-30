<?php
/**
 * @file
 * lms_form.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function lms_form_user_default_roles() {
  $roles = array();

  // Exported role: staff.
  $roles['staff'] = array(
    'name' => 'staff',
    'weight' => '7',
  );

  return $roles;
}
