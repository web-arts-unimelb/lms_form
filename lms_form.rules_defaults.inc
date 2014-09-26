<?php
/**
 * @file
 * lms_form.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function lms_form_default_rules_configuration() {
  $items = array();
  $items['rules_lms_form_email_result'] = entity_import('rules_config', '{ "rules_lms_form_email_result" : {
      "LABEL" : "lms_form_email_result",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "mimemail" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "lmsform" : "lmsform" } } } }
      ],
      "DO" : [
        { "mimemail" : {
            "key" : [ "node:title" ],
            "to" : "gliang@unimelb.edu.au,[node:field-lms-uni-email]",
            "from_name" : "Arts LMS Support",
            "from_mail" : "arts-eTeaching@unimelb.edu.au",
            "subject" : "LMS, [node:field-lms-primary-subject], [node:field-lms-full-name]",
            "body" : "\\u003Cp\\u003EUni email: [node:field-lms-uni-email]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003EFull name: [node:field-lms-full-name]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003EPhone: [node:field-lms-phone]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003EPrimary subject: [node:field-lms-primary-subject]\\u003C\\/p\\u003E\\r\\n\\r\\n[node:field-lms-cohort-label]\\r\\n\\u003Cp\\u003E[node:field-lms-cohort-collection:0:field_lms_cohort_subject]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-cohort-collection:1:field_lms_cohort_subject]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-cohort-collection:2:field_lms_cohort_subject]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-cohort-collection:3:field_lms_cohort_subject]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-cohort-collection:4:field_lms_cohort_subject]\\u003C\\/p\\u003E\\r\\n\\r\\n\\u003Cp\\u003E\\u0026nbsp;\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003EHow to create: [node:field-lms-how-create]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-subject-code]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-copy-forward-verify]\\u003C\\/p\\u003E\\r\\n\\r\\n[node:field-lms-staff-detail-label]\\r\\n\\u003Cp\\u003E[node:field-lms-staff-collection:0:field_lms_staff_name] [node:field-lms-staff-collection:0:field_lms_staff_email] [node:field-lms-staff-collection:0:field_lms_staff_role]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-staff-collection:1:field_lms_staff_name] [node:field-lms-staff-collection:1:field_lms_staff_email] [node:field-lms-staff-collection:1:field_lms_staff_role]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-staff-collection:2:field_lms_staff_name] [node:field-lms-staff-collection:2:field_lms_staff_email] [node:field-lms-staff-collection:2:field_lms_staff_role]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-staff-collection:3:field_lms_staff_name] [node:field-lms-staff-collection:3:field_lms_staff_email] [node:field-lms-staff-collection:3:field_lms_staff_role]\\u003C\\/p\\u003E\\r\\n\\u003Cp\\u003E[node:field-lms-staff-collection:4:field_lms_staff_name] [node:field-lms-staff-collection:4:field_lms_staff_email] [node:field-lms-staff-collection:4:field_lms_staff_role]\\u003C\\/p\\u003E\\r\\n\\r\\n[node:field-lms-note-label]\\r\\n\\u003Cp\\u003E[node:field-lms-note]\\u003C\\/p\\u003E\\r\\n\\r\\n\\u003Chr\\/\\u003E\\r\\n\\r\\n\\u003Cp\\u003EThanks for your formal nomination of your LMS subject site.  Your request will be checked and forwarded to the central LMS team who will create the site. The enrolled students will then be fed in from ISIS and the staff members will be added. You can then begin customising and updating your site, before making it available to the students.\\u003C\\/p\\u003E\\r\\n\\r\\n\\u003Chr\\/\\u003E\\r\\n\\r\\n\\u003Cp\\u003ENeed help with your subject\\u0027s LMS site? We are running LMS drop-in Help sessions. For dates and further information please see the \\u003Ca href=\\u0022http:\\/\\/staff.arts.unimelb.edu.au\\/eteaching\\u0022\\u003EArts Faculty staff intranet\\u003C\\/a\\u003E. There is no need to register for these sessions, staff are free to drop in for as long or short a time as they have available.\\u003C\\/p\\u003E",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
