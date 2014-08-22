<?php
/**
 * @file
 * lms_form.field_group.inc
 */

/**
 * Implements hook_field_group_info().
 */
function lms_form_field_group_info() {
  $export = array();

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_accessment|node|lms_survey|form';
  $field_group->group_name = 'group_accessment';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lms_survey';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Assessment, marking and feedback',
    'weight' => '4',
    'children' => array(
      0 => 'field_survey_turnitin',
      1 => 'field_survey_online_feedback',
      2 => 'field_survey_more_turnitin',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => 'group-accessment field-group-div',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_accessment|node|lms_survey|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_analytic|node|lms_survey|form';
  $field_group->group_name = 'group_analytic';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lms_survey';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Learning Analytics',
    'weight' => '6',
    'children' => array(
      0 => 'field_survey_tracking',
      1 => 'field_survey_collect_data',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'label' => 'Learning Analytics',
      'instance_settings' => array(
        'required_fields' => 1,
        'id' => '',
        'classes' => 'group-analytic field-group-div',
        'description' => '',
        'show_label' => '1',
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
      ),
      'formatter' => 'open',
    ),
  );
  $export['group_analytic|node|lms_survey|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_incop_video|node|lms_survey|form';
  $field_group->group_name = 'group_incop_video';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lms_survey';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Incorporating video (and the "flipped classroom") in your teaching',
    'weight' => '5',
    'children' => array(
      0 => 'field_survey_incop_video_multi',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => 'group-incop-video field-group-div',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_incop_video|node|lms_survey|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lms_staff_1|node|lmsform|form';
  $field_group->group_name = 'group_lms_staff_1';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lmsform';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '10',
    'children' => array(
      0 => 'field_lms_staff_name_1',
      1 => 'field_lms_staff_role_1',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'formatter' => 'collapsible',
      'instance_settings' => array(
        'description' => '',
        'classes' => '',
        'required_fields' => 1,
      ),
    ),
  );
  $export['group_lms_staff_1|node|lmsform|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lms_staff_2|node|lmsform|form';
  $field_group->group_name = 'group_lms_staff_2';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lmsform';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '15',
    'children' => array(
      0 => 'field_lms_staff_name_2',
      1 => 'field_lms_staff_role_2',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Staff',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_lms_staff_2|node|lmsform|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lms_staff_3|node|lmsform|form';
  $field_group->group_name = 'group_lms_staff_3';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lmsform';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '19',
    'children' => array(
      0 => 'field_lms_staff_name_3',
      1 => 'field_lms_staff_role_3',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Staff',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_lms_staff_3|node|lmsform|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_lms_staff_4|node|lmsform|form';
  $field_group->group_name = 'group_lms_staff_4';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lmsform';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Staff',
    'weight' => '21',
    'children' => array(
      0 => 'field_lms_staff_name_4',
      1 => 'field_lms_staff_role_4',
    ),
    'format_type' => 'fieldset',
    'format_settings' => array(
      'label' => 'Staff',
      'instance_settings' => array(
        'required_fields' => 1,
        'classes' => '',
        'description' => '',
      ),
      'formatter' => 'collapsed',
    ),
  );
  $export['group_lms_staff_4|node|lmsform|form'] = $field_group;

  $field_group = new stdClass();
  $field_group->disabled = FALSE; /* Edit this to true to make a default field_group disabled initially */
  $field_group->api_version = 1;
  $field_group->identifier = 'group_other|node|lms_survey|form';
  $field_group->group_name = 'group_other';
  $field_group->entity_type = 'node';
  $field_group->bundle = 'lms_survey';
  $field_group->mode = 'form';
  $field_group->parent_name = '';
  $field_group->data = array(
    'label' => 'Other',
    'weight' => '7',
    'children' => array(
      0 => 'field_survey_other_need',
    ),
    'format_type' => 'div',
    'format_settings' => array(
      'formatter' => 'open',
      'instance_settings' => array(
        'description' => '',
        'show_label' => 1,
        'label_element' => 'h3',
        'effect' => 'none',
        'speed' => 'fast',
        'classes' => 'group-other field-group-div',
        'required_fields' => 1,
        'id' => '',
      ),
    ),
  );
  $export['group_other|node|lms_survey|form'] = $field_group;

  return $export;
}
