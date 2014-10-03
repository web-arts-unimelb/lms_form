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
            "to" : "gliang@unimelb.edu.au,[node:field-lms-uni-email],mitch.buzza@unimelb.edu.au",
            "from_name" : "Arts LMS Support",
            "from_mail" : "arts-eTeaching@unimelb.edu.au",
            "subject" : "LMS, [node:field-lms-primary-subject], [node:field-lms-full-name]",
            "body" : "[node:field-lms-email]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
