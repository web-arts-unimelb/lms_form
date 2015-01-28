<?php
/**
 * @file
 * lms_form.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function lms_form_views_default_views() {
  $export = array();

  $view = new view();
  $view->name = 'lms';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'node';
  $view->human_name = 'lms';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'lms';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'node';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  /* Sort criterion: Content: Post date */
  $handler->display->display_options['sorts']['created']['id'] = 'created';
  $handler->display->display_options['sorts']['created']['table'] = 'node';
  $handler->display->display_options['sorts']['created']['field'] = 'created';
  $handler->display->display_options['sorts']['created']['order'] = 'DESC';
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['enabled'] = FALSE;
  $handler->display->display_options['path'] = 'lms';

  /* Display: Export LMS survey data */
  $handler = $view->new_display('views_data_export', 'Export LMS survey data', 'views_data_export_1');
  $handler->display->display_options['defaults']['title'] = FALSE;
  $handler->display->display_options['title'] = 'Export LMS survey data';
  $handler->display->display_options['pager']['type'] = 'none';
  $handler->display->display_options['pager']['options']['offset'] = '0';
  $handler->display->display_options['style_plugin'] = 'views_data_export_csv';
  $handler->display->display_options['defaults']['fields'] = FALSE;
  /* Field: Content: Post date */
  $handler->display->display_options['fields']['created']['id'] = 'created';
  $handler->display->display_options['fields']['created']['table'] = 'node';
  $handler->display->display_options['fields']['created']['field'] = 'created';
  $handler->display->display_options['fields']['created']['label'] = 'Date';
  $handler->display->display_options['fields']['created']['date_format'] = 'medium';
  $handler->display->display_options['fields']['created']['second_date_format'] = 'long';
  /* Field: Content: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Survey title';
  $handler->display->display_options['fields']['title']['exclude'] = TRUE;
  $handler->display->display_options['fields']['title']['alter']['word_boundary'] = FALSE;
  $handler->display->display_options['fields']['title']['alter']['ellipsis'] = FALSE;
  $handler->display->display_options['fields']['title']['hide_empty'] = TRUE;
  /* Field: Global: Regular Expression rewrite */
  $handler->display->display_options['fields']['regex_rewrite']['id'] = 'regex_rewrite';
  $handler->display->display_options['fields']['regex_rewrite']['table'] = 'views';
  $handler->display->display_options['fields']['regex_rewrite']['field'] = 'regex_rewrite';
  $handler->display->display_options['fields']['regex_rewrite']['label'] = 'Username';
  $handler->display->display_options['fields']['regex_rewrite']['alter']['text'] = '[title]';
  $handler->display->display_options['fields']['regex_rewrite']['alter']['text_patterns'] = '/LMS survey: (\\w+)/';
  $handler->display->display_options['fields']['regex_rewrite']['alter']['text_replacements'] = '$1';
  /* Field: Content: Do your students submit their assessment via Turnitin? */
  $handler->display->display_options['fields']['field_survey_turnitin']['id'] = 'field_survey_turnitin';
  $handler->display->display_options['fields']['field_survey_turnitin']['table'] = 'field_data_field_survey_turnitin';
  $handler->display->display_options['fields']['field_survey_turnitin']['field'] = 'field_survey_turnitin';
  $handler->display->display_options['fields']['field_survey_turnitin']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_survey_turnitin']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Content: Do you give feedback to students online via the LMS? */
  $handler->display->display_options['fields']['field_survey_online_feedback']['id'] = 'field_survey_online_feedback';
  $handler->display->display_options['fields']['field_survey_online_feedback']['table'] = 'field_data_field_survey_online_feedback';
  $handler->display->display_options['fields']['field_survey_online_feedback']['field'] = 'field_survey_online_feedback';
  $handler->display->display_options['fields']['field_survey_online_feedback']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_survey_online_feedback']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Content: Would you like further information or a customized session for your tutors, on using Turnitin, marking online and using the Grade Centre? */
  $handler->display->display_options['fields']['field_survey_more_turnitin']['id'] = 'field_survey_more_turnitin';
  $handler->display->display_options['fields']['field_survey_more_turnitin']['table'] = 'field_data_field_survey_more_turnitin';
  $handler->display->display_options['fields']['field_survey_more_turnitin']['field'] = 'field_survey_more_turnitin';
  $handler->display->display_options['fields']['field_survey_more_turnitin']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_survey_more_turnitin']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Content: When do you prefer training? */
  $handler->display->display_options['fields']['field_lms_survey_time']['id'] = 'field_lms_survey_time';
  $handler->display->display_options['fields']['field_lms_survey_time']['table'] = 'field_data_field_lms_survey_time';
  $handler->display->display_options['fields']['field_lms_survey_time']['field'] = 'field_lms_survey_time';
  $handler->display->display_options['fields']['field_lms_survey_time']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Content: Would you like more information about: */
  $handler->display->display_options['fields']['field_lms_survey_incorp']['id'] = 'field_lms_survey_incorp';
  $handler->display->display_options['fields']['field_lms_survey_incorp']['table'] = 'field_data_field_lms_survey_incorp';
  $handler->display->display_options['fields']['field_lms_survey_incorp']['field'] = 'field_lms_survey_incorp';
  $handler->display->display_options['fields']['field_lms_survey_incorp']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_lms_survey_incorp']['settings'] = array(
    'field_formatter_class' => '',
  );
  $handler->display->display_options['fields']['field_lms_survey_incorp']['delta_offset'] = '0';
  /* Field: Content: What are your top 5 priority areas for eTeaching professional development in 2015? */
  $handler->display->display_options['fields']['field_lms_survey_area']['id'] = 'field_lms_survey_area';
  $handler->display->display_options['fields']['field_lms_survey_area']['table'] = 'field_data_field_lms_survey_area';
  $handler->display->display_options['fields']['field_lms_survey_area']['field'] = 'field_lms_survey_area';
  $handler->display->display_options['fields']['field_lms_survey_area']['label'] = 'What are your top 5 priority areas for eTeaching professional development?';
  $handler->display->display_options['fields']['field_lms_survey_area']['type'] = 'taxonomy_term_reference_plain';
  $handler->display->display_options['fields']['field_lms_survey_area']['settings'] = array(
    'field_formatter_class' => '',
  );
  $handler->display->display_options['fields']['field_lms_survey_area']['delta_offset'] = '0';
  /* Field: Content: Do you enable statistics tracking on items in your LMS subject site? */
  $handler->display->display_options['fields']['field_survey_tracking']['id'] = 'field_survey_tracking';
  $handler->display->display_options['fields']['field_survey_tracking']['table'] = 'field_data_field_survey_tracking';
  $handler->display->display_options['fields']['field_survey_tracking']['field'] = 'field_survey_tracking';
  $handler->display->display_options['fields']['field_survey_tracking']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_survey_tracking']['settings'] = array(
    'field_formatter_class' => '',
  );
  /* Field: Content: Would you like to make better use of collecting data in your LMS subject site for items within (and external to) the LMS? */
  $handler->display->display_options['fields']['field_survey_collect_data']['id'] = 'field_survey_collect_data';
  $handler->display->display_options['fields']['field_survey_collect_data']['table'] = 'field_data_field_survey_collect_data';
  $handler->display->display_options['fields']['field_survey_collect_data']['field'] = 'field_survey_collect_data';
  $handler->display->display_options['fields']['field_survey_collect_data']['hide_empty'] = TRUE;
  $handler->display->display_options['fields']['field_survey_collect_data']['settings'] = array(
    'field_formatter_class' => '',
  );
  $handler->display->display_options['defaults']['filter_groups'] = FALSE;
  $handler->display->display_options['defaults']['filters'] = FALSE;
  /* Filter criterion: Content: Published */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'node';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = 1;
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
  /* Filter criterion: Content: Type */
  $handler->display->display_options['filters']['type']['id'] = 'type';
  $handler->display->display_options['filters']['type']['table'] = 'node';
  $handler->display->display_options['filters']['type']['field'] = 'type';
  $handler->display->display_options['filters']['type']['value'] = array(
    'lms_survey' => 'lms_survey',
  );
  $handler->display->display_options['path'] = 'export/lms-survey/data.csv';
  $handler->display->display_options['sitename_title'] = 0;
  $export['lms'] = $view;

  return $export;
}
