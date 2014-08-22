<?php
/**
 * @file
 * lms_form.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function lms_form_user_default_permissions() {
  $permissions = array();

  // Exported permission: create lms_survey content.
  $permissions['create lms_survey content'] = array(
    'name' => 'create lms_survey content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: create lmsform content.
  $permissions['create lmsform content'] = array(
    'name' => 'create lmsform content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any lms_survey content.
  $permissions['delete any lms_survey content'] = array(
    'name' => 'delete any lms_survey content',
    'roles' => array(
      0 => 'site admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any lmsform content.
  $permissions['delete any lmsform content'] = array(
    'name' => 'delete any lmsform content',
    'roles' => array(
      0 => 'site admin',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own lms_survey content.
  $permissions['delete own lms_survey content'] = array(
    'name' => 'delete own lms_survey content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own lmsform content.
  $permissions['delete own lmsform content'] = array(
    'name' => 'delete own lmsform content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any lms_survey content.
  $permissions['edit any lms_survey content'] = array(
    'name' => 'edit any lms_survey content',
    'roles' => array(
      0 => 'site admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any lmsform content.
  $permissions['edit any lmsform content'] = array(
    'name' => 'edit any lmsform content',
    'roles' => array(
      0 => 'site admin',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own lms_survey content.
  $permissions['edit own lms_survey content'] = array(
    'name' => 'edit own lms_survey content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own lmsform content.
  $permissions['edit own lmsform content'] = array(
    'name' => 'edit own lmsform content',
    'roles' => array(
      0 => 'site admin',
      1 => 'staff',
    ),
    'module' => 'node',
  );

  return $permissions;
}
