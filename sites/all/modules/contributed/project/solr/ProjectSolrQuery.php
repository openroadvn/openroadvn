<?php

include_once drupal_get_path('module', 'apachesolr') .'/Solr_Base_Query.php';

class ProjectSolrQuery extends Solr_Base_Query {
  function default_sorts() {
    $sorts = array();
    if (!empty($this->keys)) {
      // The user has searched for a keyword.
      $sorts['score'] = array('title' => t('Relevancy'), 'default' => 'asc');
    }
    $sorts['sort_title'] = array('title' => t('Title'), 'default' => 'asc');
    $sorts['created'] = array('title' => t('Creation date'), 'default' => 'desc');
    if (module_exists('project_release')) {
      $sorts['ds_project_latest_release'] = array('title' => t('Last release'), 'default' => 'desc');
      $sorts['ds_project_latest_activity'] = array('title' => t('Recent activity'), 'default' => 'desc');
    }
    if (module_exists('project_usage')) {
      $sorts['sis_project_release_usage'] = array('title' => t('Usage statistics'), 'default' => 'desc');
    }
    return $sorts;
  }
}
