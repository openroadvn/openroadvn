<?php
// $Id: admin.inc,v 1.1 2009/03/17 23:16:13 dww Exp $

/**
 * @file
 * Code necessary for the project_usage administrative UI.
 */

/**
 * Form builder for the module settings page.
 */
function project_usage_settings_form(&$form_state) {
  $times = array(
     3 * PROJECT_USAGE_YEAR,
     2 * PROJECT_USAGE_YEAR,
     1 * PROJECT_USAGE_YEAR,
    26 * PROJECT_USAGE_WEEK,
    12 * PROJECT_USAGE_WEEK,
     8 * PROJECT_USAGE_WEEK,
     4 * PROJECT_USAGE_WEEK,
  );
  $age_options = drupal_map_assoc($times, 'format_interval');
  $form['project_usage_life_daily'] = array(
    '#type' => 'select',
    '#title' => t('Daily usage data lifespan'),
    '#default_value' => variable_get('project_usage_life_daily', 4 * PROJECT_USAGE_WEEK),
    '#options' => $age_options,
    '#description' => t('Discard the daily usage data after this amount of time has passed.'),
  );
  $form['project_usage_life_weekly_project'] = array(
    '#type' => 'select',
    '#title' => t('Weekly project data lifespan'),
    '#default_value' => variable_get('project_usage_life_weekly_project', PROJECT_USAGE_YEAR),
    '#options' => $age_options,
    '#description' => t('Discard the weekly usage for project nodes after this amount of time has passed.'),
  );
  $form['project_usage_life_weekly_release'] = array(
    '#type' => 'select',
    '#title' => t('Weekly release data lifespan'),
    '#default_value' => variable_get('project_usage_life_weekly_release', 26 * PROJECT_USAGE_WEEK),
    '#options' => $age_options,
    '#description' => t('Discard the weekly usage for project nodes after this amount of time has passed.'),
  );
  return system_settings_form($form);
}

