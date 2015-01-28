<?php
/**
 * @file
 * lms_form.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function lms_form_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "field_group" && $api == "field_group") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function lms_form_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function lms_form_node_info() {
  $items = array(
    'lms_survey' => array(
      'name' => t('lms_survey'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'lmsform' => array(
      'name' => t('lmsform'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  return $items;
}
