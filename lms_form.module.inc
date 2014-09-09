<?php

//set_error_handler('exceptions_error_handler');

/*
 * Implement hook_form_alter
 */
function lms_form_form_alter(&$form, &$form_state, $form_id)
{
	// When create a new node
	if( $form_id === 'lmsform_node_form' && empty($form['nid']['#value']) )
	{
		$staff_info = _get_staff_info();
	
		$form['field_lms_uni_email']['und'][0]['email']['#default_value'] = $staff_info['uni_email'];
		$form['field_lms_full_name']['und'][0]['value']['#default_value'] = $staff_info['full_name'];
		$form['field_lms_phone']['und'][0]['value']['#default_value'] = $staff_info['phone'];
		
		drupal_set_title(t(_set_lms_form_title()));
		
		// Redirect after submission
		$form['actions']['submit']['#submit'][] = 'lms_form_submit_handler';

		// Validate
		$form['#validate'][] = '_validate_email_domain';
		$form['#validate'][] = '_validate_staff_collection';
		$form['#validate'][] = '_validate_course_code';

		// Tooltip
		drupal_add_js('
				jQuery(document).ready(function () {
					var msg = "Subject Coordinator (Full Access to all features);  Tutor (High level access, but cannot add/remove staff)"; 
					jQuery("select[name*=\'field_lms_staff_role\']").attr("title", msg);

					jQuery("#edit-field-lms-copy-sub-collection-und-0-remove-button").attr("disabled", "disabled");
				});	
			'
			, 'inline'
		);

		// Add css
		drupal_add_css(drupal_get_path('module', 'lms_form') . '/css/lms_form.css');
	}
	elseif($form_id === 'lms_survey_node_form' && empty($form['nid']['#value']))
	{
		$title = "eTeaching survey";
		drupal_set_title(t($title));
	
		// Remove N/A
		unset($form['field_survey_turnitin'][LANGUAGE_NONE]['#options']['_none']);		
		unset($form['field_survey_online_feedback'][LANGUAGE_NONE]['#options']['_none']);
		unset($form['field_survey_more_turnitin'][LANGUAGE_NONE]['#options']['_none']);
		unset($form['field_survey_tracking'][LANGUAGE_NONE]['#options']['_none']);
		unset($form['field_survey_collect_data'][LANGUAGE_NONE]['#options']['_none']);
		
		// Redirect after submission
		$form['actions']['submit']['#submit'][] = 'lms_survey_submit_handler';

		// Add css
    drupal_add_css(drupal_get_path('module', 'lms_form') . '/css/lms_form.css');
	}
}

function _get_staff_info()
{
	$path = dirname(dirname(dirname(__FILE__))). "/misc/confidential/ldap.php";
	if(!file_exists($path))
	{
		return;
	}
	else
	{
		include_once($path);
	}

	$output = array();
	$output["uni_email"] = "";
	$output["full_name"] = "";
	$output["phone"] = "";

	$ldap_username = $arts_ldap["ad1"]["username"];
	$ldap_password = $arts_ldap["ad1"]["password"];
	$ldap_url = $arts_ldap["ad1"]["url"];
	$ldap_port = $arts_ldap["ad1"]["port"];

	global $user;
	$search_cn = $user->name;

	$ldap_conn = ldap_connect($ldap_url, $ldap_port) or die("Could not connect to LDAP server.");
	// If you try to perform the searches on Windows 2003 Server Active Directory or above, 
	// it seems 	that you have to set the LDAP_OPT_REFERRALS option to 0:
	ldap_set_option($ldap_conn, LDAP_OPT_REFERRALS, 0);
	ldap_set_option($ldap_conn, LDAP_OPT_PROTOCOL_VERSION, 3);
	
	// Bind
	$ldap_dn_account = "cn=$ldap_username,ou=Local Accounts,ou=100,ou=Business Units,dc=unimelb,dc=edu,dc=au";	
	//$ldap_dn_search = "dc=staff,dc=unimelb,dc=edu,dc=au";
	$ldap_dn_search = "dc=unimelb,dc=edu,dc=au";
	
	if($ldap_conn)
	{
		try
		{
			$ldap_bind = ldap_bind($ldap_conn, $ldap_dn_account, $ldap_password);
		}
		catch(Exception $e)
		{
			drupal_set_message(t('Fail at ldap_conn'), 'error');
			
			//print_r($e);
			//die;
		}
	
		if($ldap_bind) 
		{
    	try
			{
				$filter = "(cn=$search_cn)";
				$ldap_search_res = ldap_search($ldap_conn, $ldap_dn_search, $filter); 	
				$info = ldap_get_entries($ldap_conn, $ldap_search_res);
				
				if(isset($info[0])) {
					$output["uni_email"] = $info[0]["mail"][0];
					$output["full_name"] = $info[0]["displayname"][0];
					$output["phone"] = $info[0]["telephonenumber"][0];
				}
				else {
					$output["uni_email"] = "";
					$output["full_name"] = "";
					$output["phone"] = "";	
				}
			}
			catch(Exception $e)
			{
				// Not able to find the user
				// Die silently.
				
				//print_r($e);
				//die;
			}
    } 
    else 
    {

    }
	}
	else
	{
		//print_r($e);
		//die;
	
		drupal_set_message(t('Fail at ldap_conn'), 'error');
	}
	
	return $output;
}

/*
 * hook_url_inbound_alter
 */
function lms_form_url_inbound_alter(&$path, $original_path, $path_language)
{
	global $user;	

	// https://www.drupal.org/files/1291536-4.patch
	// Detect cron
	// It seems updating this php file, can allow award import to proceed?

	// It seems when batch is running, all urls will be scanned.
	if(preg_match('|^node/add/lms-survey(/.*)?|', $path, $matches)) {
		if(_does_user_complete_survey_is_admin($user->name)) {
			// Stay in the survey
		}
		else if(_does_user_complete_survey($user->name))
    {
			/*
				Run drush cron, if the still go to node/add/lmsform
				Extra: 
				https://drupal.org/node/1140842
				https://drupal.org/node/1217668
				http://drupal.stackexchange.com/questions/89977/how-to-programmatically-determine-if-the-cron-is-running
			*/

			if(
					!drupal_is_cli() || 
					!preg_match('|^admin/reports/status/run-cron(/.*)?|', $path, $matches) ||
					variable_get('cron_semaphore', FALSE) // cron is running 
			) {
				/*
				//test
				$msg = '<pre>'. print_r($path, TRUE). print_r($user, TRUE). '</pre>';
				watchdog('lms_form 1', $msg);
				*/

				// Go to the lms form
      	$the_path = "node/add/lmsform";
      	drupal_goto($the_path);
			}   
    }
    else
    {
			// Stay in the survey
    }
	}
	else if(preg_match('|^node/add/lmsform(/.*)?|', $path, $matches)) {
		if(_does_user_complete_survey_is_admin($user->name)) {
      // Stay in the lms_form 
    }
  	if(_does_user_complete_survey($user->name))
  	{
 			// Stay in lmsform
  	}
  	else
  	{
			/*
				Run drush cron, if the still go to node/add/lmsform
        Extra: 
        https://drupal.org/node/1140842
        https://drupal.org/node/1217668
        http://drupal.stackexchange.com/questions/89977/how-to-programmatically-determine-if-the-cron-is-running
			*/
			if(
          !drupal_is_cli() || 
          !preg_match('|^admin/reports/status/run-cron(/.*)?|', $path, $matches) ||
					variable_get('cron_semaphore', FALSE) // cron is running
      ) {
				/*
				//test
        $msg = '<pre>'. print_r($path, TRUE). print_r($user, TRUE). '</pre>';
        watchdog('lms_form 2', $msg);
				*/	

  			// Go to the survey 
  			$the_path = "node/add/lms-survey";
  			drupal_goto($the_path);
			}
  	}
  }
}


// Validate
function _validate_email_domain($form, &$form_state){
	$pattern = "/^[a-zA-Z0-9._%+-]+@unimelb\.edu\.au$/";

	// Submitter email
	$submitter_email = $form_state['input']['field_lms_uni_email']['und'][0]['email']; 
	if(preg_match($pattern, $submitter_email)) {
	}
	else {
		form_set_error("uni_email_format", "University email address must be xxxx@unimelb.edu.au");	
	}

	// Staff collection fields
	foreach($form_state['input']['field_lms_staff_collection']['und'] as $field_set) {
		$collection_email = $field_set['field_lms_staff_email']['und']['0']['email'];
		if(empty($collection_email)) {
		}
		else if(preg_match($pattern, $collection_email)) { 
		}
		else {
			form_set_error("staff_email_format", "Staff email address must be xxxx@unimelb.edu.au");
		}
	}
}

// Validate
function _validate_staff_collection($form, &$form_state) {
	foreach($form_state['input']['field_lms_staff_collection']['und'] as $field_set) {
		$staff_name = $field_set['field_lms_staff_name']['und'];	

		if(empty($staff_name)) {
		}
		else {
			$collection_email = $field_set['field_lms_staff_email']['und']['0']['email'];
			if(empty($collection_email)) {
				form_set_error("empty_staff_email", "Staff email address cannot be empty.");
			}		

			$staff_role = $field_set['field_lms_staff_role']['und'];
			if($staff_role === "_none") {
				form_set_error("empty_staff_role", "Staff role cannot be empty.");
			}
		}
	}
}

// Validate
function _validate_course_code($form, &$form_state) {
	foreach($form_state['input']['field_lms_copy_sub_collection']['und'] as $field_set) {
		$course_code = $field_set['field_lms_copy_course_code']['und']['0']['value'];	
		$pattern = "/^[A-Z]{4}[0-9]{5}$/";
		
		if(preg_match($pattern, $course_code)) {
	
  	}
		elseif(empty($course_code)) {
			// it cannot be empty, but ...
		}
  	else {
    	form_set_error("course_code_format", "Course code needs to be 4 capital letters followed by 5 numbers. e.g. HIST12345");
  	}	
	}
}

// lms
function _does_user_complete_survey($username)
{
	$condi = false;

	if(_does_user_complete_survey_in_drupal_db($username))
	{
		$condi = true;
	}
	
	return $condi;
}

function _does_user_complete_survey_is_admin($username)
{
	$username_array = array(
		'test-site-admin',
		//'gliang',
		'mmhinze',
		'mbuzza'
	);

	if(in_array($username, $username_array)) {
		return true;
	}
	else {
		return false;
	}
}

/*
function _does_user_complete_survey_in_lms_db($username)
{
	// Back to default db
  db_set_active();

	$condi = false;

	$path = dirname(dirname(dirname(__FILE__))). "/misc/confidential/external_db.php";
	if(file_exists($path))
	{
		include_once($path);
	
		$other_database = array(
      'database' => $external_db['lms']['dbname'],
      'username' => $external_db['lms']['username'],
      'password' => $external_db['lms']['password'],
      'host' => $external_db['lms']['host'],
      'driver' => 'mysql',
		);
		// replace 'YourDatabaseKey' with something that's unique to your module
		Database::addConnectionInfo('lms_database_key', 'default', $other_database);
		db_set_active('lms_database_key');

		$sql = "SELECT COUNT(*) FROM record WHERE username = '$username'";
		$result = db_query($sql);
	
		if($result->rowCount() > 0)
		{
		  $condi = true;
		}
		else
		{
		  $condi = false;
		}
	}
	else
	{
		$condi = false;
	}

	// Back to default db
  db_set_active();

	return $condi;
}
*/

function _does_user_complete_survey_in_drupal_db($username)
{
	$account = user_load_by_name($username);
	if($account)
	{
		$sql = "SELECT * FROM {node} WHERE uid = '$account->uid' AND type = 'lms_survey'";	
		$result = db_query($sql);
		
		if($result->rowCount() > 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

function _set_lms_form_title()
{
	$curr_year_display = date("Y");
	$curr_month = date("n");
	
	if($curr_month <= 6)
	{
		$curr_month_display = "Semester One, January - June";
	}
	else
	{
		$curr_month_display = "Semester Two, July - December";
	}
	
	$return_title = "LMS Subject Nominations 2014";
	
	return $return_title;
}

function lms_survey_submit_handler(&$form, &$form_state) 
{
  $form_state['redirect'] = 'node/add/lmsform';
}

function lms_form_submit_handler(&$form, &$form_state)
{
	$msg = "";
	unset($_SESSION['messages']);
	drupal_set_message($msg);
	
	$form_state['redirect'] = _lms_confirmation_page();
}

function _lms_confirmation_page()
{
	global $base_root;
	$confirmation_uri = "lms-nomination-success";
	$confirmation_page_title = "LMS nomination success";
	
	if(_is_node_exist($confirmation_page_title))
	{
		
	}
	else
	{
		_create_lms_confirmation_page($confirmation_uri, $confirmation_page_title);
	}
	
	return $confirmation_uri;
}

function _create_lms_confirmation_page($confirmation_uri, $confirmation_page_title)
{
	//creating a bare node
	$node = new StdClass();
		
	//giving it type
	$node->type = 'page';
	
	//give it a published staus
	$node->status = 1;
	$node->language = LANGUAGE_NONE;	
	$node->uid = 1;
	
	//gives title
	$node->title = $confirmation_page_title;
	
	//gives body
	$page_body = "
		<p>You have sucessfully submitted a LMS nomination.</p>
		<p>Thanks for your formal nomination of your LMS subject site. Your request will be checked and forwarded to the central LMS team who will create the site. The enrolled students will then be fed in from ISIS and the staff members will be added. You can then begin customising and updating your site, before making it available to the students.</p>
		<p><a href='/node/add/lmsform'>Add another lms subject.</a></p>
	";
	$node->body[$node->language] = array(0 => array('format'=> 'full_html', 'value' => $page_body));
  $node->path = array('alias' => $confirmation_uri);
	
	//save it and give it the rest of the attributes
	node_save($node);
}

function _is_node_exist($node_title)
{
	$row = db_query('SELECT nid FROM {node} WHERE title = :mytitle', array(':mytitle' => $node_title))->fetchField();
 	
 	if($row) 
 	{
    return true;
 	} 
 	else 
 	{
    return false;
 	}
}

//hook_tokens_alter
function lms_form_tokens_alter(array &$replacements, array $context) {
  $options = $context['options'];

  if (isset($options['language'])) {
    $url_options['language'] = $options['language'];
    $language_code = $options['language']->language;
  }
  else {
    $language_code = NULL;
  }
  $sanitize = !empty($options['sanitize']);

  if ($context['type'] == 'node' && !empty($context['data']['node'])) {
    $node = $context['data']['node'];

		// Cohort linked subjects label
		if(isset($context['tokens']['field-lms-cohort-collection:0:field_lms_cohort_subject'])) {
			$first_cohort_subject = $replacements[$context['tokens']['field-lms-cohort-collection:0:field_lms_cohort_subject']];
			if(!empty($first_cohort_subject)) {
				$replacements[$context['tokens']['field-lms-cohort-label']] = '<p>&nbsp;</p><p>Cohort linked subjects:</p>';
			}
			else {
				$replacements[$context['tokens']['field-lms-cohort-label']] = '';
			}
		}

		//test
		/*
		$msg = "<pre> start --- ". print_r($replacements, TRUE). "</pre>";
		watchdog('lms', $msg);	

		$msg = "<pre>". print_r($context, TRUE). "</pre>";
    watchdog('lms', $msg);
		*/

		// Is copy forward verify radio button clicked? 
		if(isset($node->field_lms_copy_verify)) {
      $is_verify = $node->field_lms_copy_verify['und'][0]['value'];
      if(!empty($is_verify)) {
        $replacements[$context['tokens']['field-lms-copy-forward-verify']] = '<p>&nbsp;</p><p>All subject codes, subject names, years and study periods are correct: '. $is_verify. '</p>';
      }
      else {
        $replacements[$context['tokens']['field-lms-copy-forward-verify']] = '';
      }
    }

		// Subject code
    if(isset($node->field_lms_copy_sub_collection)) {
			if(count($node->field_lms_copy_sub_collection) > 0) {
				$entity_id = $node->field_lms_copy_sub_collection['und'][0]['value'];
				$entity_type = 'field_collection_item';
				$bundle = 'field_lms_copy_sub_collection';

				// Course code
				$field = 'field_lms_copy_course_code_value';
				$table = 'field_data_field_lms_copy_course_code';
				$course_code = _get_collection_item_value($entity_id, $entity_type, $bundle, $table, $field);

				// Year
				$field = 'field_lms_copy_year_value';
        $table = 'field_data_field_lms_copy_year';
        $year = _get_collection_item_value($entity_id, $entity_type, $bundle, $table, $field);

				// Study period 
        $field = 'field_lms_copy_study_period_value';
        $table = 'field_data_field_lms_copy_study_period';
        $study_period = _get_collection_item_value($entity_id, $entity_type, $bundle, $table, $field);

				// All three components will exist.
				$replacements[$context['tokens']['field-lms-subject-code']] = $course_code. "_". $year. "_". $study_period;  
			}
			else {
				$entity_id = null;
				$replacements[$context['tokens']['field-lms-subject-code']] = '';
			}

    }

		// Staff detail label
		if(isset($context['tokens']['field-lms-staff-collection:0:field_lms_staff_name'])) {
      $first_staff_name = $replacements[$context['tokens']['field-lms-staff-collection:0:field_lms_staff_name']];
      if(!empty($first_staff_name)) {
        $replacements[$context['tokens']['field-lms-staff-detail-label']] = '<p>&nbsp;</p><p>Staff details:</p>';
      }
      else {
        $replacements[$context['tokens']['field-lms-staff-detail-label']] = '';
      }
    }
	
		// Note label
    if(isset($context['tokens']['field-lms-note'])) {
      $note = $replacements[$context['tokens']['field-lms-note']];
      if(!empty($note)) {
        $replacements[$context['tokens']['field-lms-note-label']] = '<p>&nbsp;</p><p>Note:</p>';
      }
      else {
        $replacements[$context['tokens']['field-lms-note-label']] = '';
      }
    }
  }
}


function _get_collection_item_value($entity_id, $entity_type, $bundle, $table, $field) {
	$sql = "
 		SELECT 
   		n.$field
    FROM 
   		{$table} n 
    WHERE 
    	n.entity_type = :type AND
      n.bundle = :bundle AND
      n.entity_id = :entity_id
  ";
  $result = db_query($sql, array(':type' => $entity_type, ':bundle' => $bundle, ':entity_id' => $entity_id));
  $obj = $result->fetchObject();

  return $obj->$field;
}
