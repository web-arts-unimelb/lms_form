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
		$form['#validate'][] = '_validate_primary_subject';
		$form['#validate'][] = '_validate_email_domain';
		$form['#validate'][] = '_validate_staff_collection';
		$form['#validate'][] = '_validate_course_code';

		$form['#validate'][] = '_validate_cohort_subjects';


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
function _validate_primary_subject($form, &$form_state) {
	$lang = LANGUAGE_NONE;
	$subject = $form_state['input']['field_lms_primary_subject'][$lang]['0']['value'];

	// https://api.drupal.org/api/drupal/modules%21field%21field.info.inc/function/field_info_instance/7
	$instance = field_info_instance('node', 'field_lms_primary_subject', 'lmsform');
	$options = explode("\n", $instance['widget']['settings']['suggested_values']);
  $options = array_map('trim', $options);
  $options = array_filter($options, 'strlen');

	if(in_array($subject, $options)) {

	}
	else {
		form_set_error("primary_subject_exists_in_list", "Primary subject is not in the list.");
	}
}


// Validate
function _validate_cohort_subjects($form, &$form_state) {
	$lang = LANGUAGE_NONE;
	$collection = $form_state['input']['field_lms_cohort_collection'][$lang];

	if(empty($collection[0]['field_lms_cohort_subject'][$lang][0]['value'])) {

	}
	else {
		// https://api.drupal.org/api/drupal/modules%21field%21field.info.inc/function/field_info_instance/7
    $instance = field_info_instance('field_collection_item', 'field_lms_cohort_subject', 'field_lms_cohort_collection');
    $options = explode("\n", $instance['widget']['settings']['suggested_values']);
    $options = array_map('trim', $options);
    $options = array_filter($options, 'strlen');

		foreach($collection as $item) {
			$input = $item['field_lms_cohort_subject'][$lang][0]['value'];
			if(in_array($input, $options)) {
				
			}
			else {
				form_set_error("cohort_subject_exists_in_list", "Cohort subject is not in the list.");
			}
		}
	}

	/*
	//test
	$msg = "<pre>". print_r($collection, true). "</pre>";
	watchdog('lms_form', $msg);
	*/
}


// Validate
function _validate_email_domain($form, &$form_state) {
	$lang = LANGUAGE_NONE;
	$pattern = "/^[a-zA-Z0-9._%+-]+@unimelb\.edu\.au$/";

	// Submitter email
	$submitter_email = $form_state['input']['field_lms_uni_email'][$lang][0]['email']; 
	if(preg_match($pattern, $submitter_email)) {
	}
	else {
		form_set_error("uni_email_format", "University email address must be xxxx@unimelb.edu.au");	
	}

	// Staff collection fields
	foreach($form_state['input']['field_lms_staff_collection'][$lang] as $field_set) {
		$collection_email = $field_set['field_lms_staff_email'][$lang]['0']['email'];
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
	$lang = LANGUAGE_NONE;
	$staff_role_store = array();

	foreach($form_state['input']['field_lms_staff_collection'][$lang] as $field_set) {
		$staff_name = $field_set['field_lms_staff_name'][$lang];	

		if(empty($staff_name)) {
		}
		else {
			$collection_email = $field_set['field_lms_staff_email'][$lang]['0']['email'];
			if(empty($collection_email)) {
				form_set_error("empty_staff_email", "Staff email address cannot be empty.");
			}		

			$staff_role_tid = $field_set['field_lms_staff_role'][$lang];
			$term = taxonomy_term_load($staff_role_tid);
			$staff_role_store[] = strtolower($term->name);
			if($staff_role_tid === '_none') {
				form_set_error("empty_staff_role", "Staff role cannot be empty.");
			}
		}
	}

	// At least one subject coordinator
	$is_sub_coordinator = strtolower($form_state['input']['field_lms_is_sub_coord'][$lang]);

	if($is_sub_coordinator == 'no') {
		$test_term = 'subject coordinator';
		if(in_array($test_term, $staff_role_store)) {

		}
		else {
			form_set_error("subject_coordinator_role", "There must be a subject coordinator.");
		}
	}

}

// Validate
function _validate_course_code($form, &$form_state) {
	$lang = LANGUAGE_NONE;

	foreach($form_state['input']['field_lms_copy_sub_collection'][$lang] as $field_set) {
		$course_letter = $field_set['field_lms_copy_abb'][$lang]['0']['value'];
		$course_num = $field_set['field_lms_copy_num'][$lang]['0']['value'];
		$course_year = $field_set['field_lms_copy_year'][$lang];
		$course_study_period = $field_set['field_lms_copy_study_period'][$lang];

		$letter_pattern = "/^[A-Z]{4}$/";
		$num_pattern = "/^[0-9]{5}$/";		

		// All empty
		if(
			empty($course_letter) &&
      empty($course_num) && 
			$course_year == '_none' &&
			$course_study_period == '_none'
		) {

		}
		else if(
			preg_match($letter_pattern, $course_letter) && 
			preg_match($num_pattern, $course_num) &&
			$course_year != '_none' &&
      $course_study_period != '_none'
		) {
	
		}
  	else {
    	form_set_error("course_code_format", "Course code needs to be 4 capital letters followed by 5 numbers. e.g. HIST12345. Year and study period cannot be empty.");
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
	$lang = LANGUAGE_NONE;

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

		// Prepare
		$user_email_part = $node->field_lms_uni_email[$lang][0]['email']; 
		$user_name_part = $node->field_lms_full_name[$lang][0]['value']; 
		$user_phone_part = $node->field_lms_phone[$lang][0]['value']; 
		$primary_subject_part = $node->field_lms_primary_subject[$lang][0]['value'];
		$teach_more_part = $node->field_lms_cohort_or_not[$lang][0]['value'];

		// Fore
		$out_fore_part = "
			<p><strong>Uni email</strong>: $user_email_part</p>
			<p><strong>Full name</strong>: $user_name_part</p>
			<p><strong>Phone</strong>: $user_phone_part</p>
			<p><strong>Subject code and subject name (e.g. JAPN10005_2014_SM1 - Japanese 7)</strong>:</p>
			<p>$primary_subject_part</p>
			<p><strong>Are you intending to teach two or more subjects (i.e. groups of students), through this LMS site</strong>: $teach_more_part</p>	
		";

		// Cohort
		$out_cohort_part = _lms_form_handle_cohort($node->field_lms_cohort_collection[$lang]); 

		// How to create
		$create_part = $node->field_lms_how_create[$lang][0]['value'];
		$out_create_part = "
			<p><strong>How would you like to your subject to be created and filled with content</strong>: $create_part</p>
		";	

		// Copy forward
		$out_copy_part = _lms_form_handle_copy($node->field_lms_copy_sub_collection[$lang]);

		// Subject cooridnator
		$coor_part = $node->field_lms_is_sub_coord[$lang][0]['value'];
		$out_coor_part = "
			<p><strong>Are you the Subject Coordinator, or will you add the the Subject Coordinator (and other staff) later</strong>: $coor_part</p>
		";

		// Staff
		$out_staff_part = _lms_form_handle_staff($node->field_lms_staff_collection[$lang]);

		// Note
		$note_part = $node->field_lms_note[$lang][0]['value'];
		$out_note_part = "
			<p><strong>Note</strong></p><p>$note_part</p>
		";

		$out = 
			$out_fore_part. 
			$out_cohort_part. 
			$out_create_part. 
			$out_copy_part.
			$out_coor_part.
			$out_staff_part.
			$out_note_part
		;
 
		$replacements[$context['tokens']['field-lms-email']] = $out;

  }
}


function _lms_form_handle_cohort($cohort_collection) {
	$out = '';

	$out_label = "<p><strong>Linked subject code and subject name (e.g. JAPN10005_2014_SM1 - Japanese 7). You can add multiple cohort linked subjects</strong></p>";
	$collection = array();

	if(empty($cohort_collection)) {

	}
	else {
		foreach($cohort_collection as $item) {
			$entity_id = $item['value'];
			$bundle = 'field_lms_cohort_collection';

			$sql = "
				SELECT
					b.field_lms_cohort_subject_value	
				FROM
					field_data_field_lms_cohort_collection a
				INNER JOIN 
					field_data_field_lms_cohort_subject b
				ON 
					a.field_lms_cohort_collection_value = b.entity_id
				WHERE
					b.entity_id = :entity_id AND
					b.bundle = :bundle
			";
			
			$array = array(
				':entity_id' => $entity_id, 
				':bundle' => $bundle,
			);
			$res = db_query($sql, $array);
			$value = $res->fetchField(); // Single value
			$collection[] = $value;
		}

		$out_collection = '<p>'. implode(', ', $collection). '</p>';

		$out = $out_label. " ". $out_collection;	
	}

	return $out;
}


function _lms_form_handle_copy($input_collection) {
	$out = '';

  $out_label = "<p><strong>Copy forward this subject's content</strong></p>";
  $collection = array();

  if(empty($input_collection)) {

  }
	else {
		// It only loop once
		foreach($input_collection as $item) {
      $entity_id = $item['value'];
      $bundle = 'field_lms_copy_sub_collection';

			// First 4 letters
			$table_coll = 'field_data_field_lms_copy_sub_collection';
			$table_field = 'field_data_field_lms_copy_abb';
			$field_coll = 'field_lms_copy_sub_collection_value';
			$field_field = 'field_lms_copy_abb_value';
			$letter = _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field);

			// 5 numbers
			$table_coll = 'field_data_field_lms_copy_sub_collection';
      $table_field = 'field_data_field_lms_copy_num';
      $field_coll = 'field_lms_copy_sub_collection_value';
      $field_field = 'field_lms_copy_num_value';
      $number = _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field);

			// Year
      $table_coll = 'field_data_field_lms_copy_sub_collection';
      $table_field = 'field_data_field_lms_copy_year';
      $field_coll = 'field_lms_copy_sub_collection_value';
      $field_field = 'field_lms_copy_year_value';
      $year = _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field);

			// Study period 
      $table_coll = 'field_data_field_lms_copy_sub_collection';
      $table_field = 'field_data_field_lms_copy_study_period';
      $field_coll = 'field_lms_copy_sub_collection_value';
      $field_field = 'field_lms_copy_study_period_value';
      $study_period = _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field);
		}

		$out = $out_label. '<p>'. $letter. $number. "_". $year. "_". $study_period. '</p>';
	}

	return $out;
}


function _lms_form_handle_staff($input_collection) {
	$out = '';

  $out_label = "<p><strong>Staff</strong></p>";
  $collection = array();

  if(empty($input_collection)) {
	
  }
	else {
		foreach($input_collection as $item) {
			$entity_id = $item['value'];
      $bundle = 'field_lms_staff_collection';

			// Share
			$table_coll = 'field_data_field_lms_staff_collection';
			$field_coll = 'field_lms_staff_collection_value';

			// Staff name 
      $table_field = 'field_data_field_lms_staff_name';
      $field_field = 'field_lms_staff_name_value';
			$field_taxo = 'field_lms_staff_name_tid';
      $staff_name = _lms_form_get_field_from_collection_taxo($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field, $field_taxo);			

			// Staff email
      $table_field = 'field_data_field_lms_staff_email';
      $field_field = 'field_lms_staff_email_email'; // The field name is different.
      $staff_email = _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field);

			// Staff role
			$table_field = 'field_data_field_lms_staff_role';
      $field_field = 'field_lms_staff_role_value';
			$field_taxo = 'field_lms_staff_role_tid';
      $staff_role = _lms_form_get_field_from_collection_taxo($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field, $field_taxo);
			
			$collection[] = $staff_name. ', '. $staff_email. ', '. $staff_role;
    }
		$out = $out_label. '<p>'. implode('; ', $collection). '</p>';
	}

	return $out;	
}


function _lms_form_get_field_from_collection($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field) {
	$return = '';

	$sql = "
  	SELECT
    	b.$field_field  
    FROM
      $table_coll a
    INNER JOIN 
      $table_field b
    ON 
      a.$field_coll = b.entity_id
    WHERE
      b.entity_id = :entity_id AND
      b.bundle = :bundle
  ";

  $array = array(
  	':entity_id' => $entity_id,
    ':bundle' => $bundle,
	);
  $res = db_query($sql, $array);
  $return = $res->fetchField();

	return $return;
}


function _lms_form_get_field_from_collection_taxo($entity_id, $bundle, $table_coll, $table_field, $field_coll, $field_field, $field_taxo) {
  $return = '';

  $sql = "
    SELECT
      c.name  
    FROM
      $table_coll a
    INNER JOIN 
      $table_field b
    ON 
      a.$field_coll = b.entity_id
		INNER JOIN
      taxonomy_term_data c
		ON
			b.$field_taxo = c.tid
    WHERE
      b.entity_id = :entity_id AND
      b.bundle = :bundle
  ";

  $array = array(
    ':entity_id' => $entity_id,
    ':bundle' => $bundle,
  );
  $res = db_query($sql, $array);
  $return = $res->fetchField();

  return $return;
}

