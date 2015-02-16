<?php

/**
 * @file
 * Theme hooks.
 */

/**
 * Implements hook_preprocess().
 */

 function str_startswith($source, $prefix) {
   return strncmp($source, $prefix, strlen($prefix)) == 0;
 }
 
 function islandora_entities_preprocess_islandora_person(array &$variables) {
  module_load_include('inc', 'islandora_entities', 'includes/utilities');
  module_load_include('inc', 'islandora', 'includes/metadata');
  module_load_include('inc', 'islandora_entities', 'includes/entities_rss');
  drupal_add_css(drupal_get_path('module', 'islandora_entities') . '/css/islandora-person.css');

  $biography = "";
  $object = $variables['object'];
  $object_pid = $object->id;
  
  if ($object['BIOGRAPHY']) {
	$html = $object['BIOGRAPHY']->content;
	$biography = (string) $html;
    $variables['biography'] = $biography;
  }
  
  $rels = $object->relationships->get();
  $related = array();
  
  foreach ($rels as $key => $rel) {
    if ($rel['predicate']['namespace'] == "http://d-nb.info/standards/elementset/gnd#") {
       try {
         $identity = $object->repository->getObject($rel['object']['value']);
       } catch (Exception $e) {
         continue;
       }
       if ($identity) {
         $related[] = $identity;
       } 
    }
  	if ($rel['predicate']['namespace'] == "http://schema.org/") {
       $sameAs = $rel['object']['value'];
	   if (str_startswith($sameAs, "http://dbpedia.org/resource")) {
			$wkp = $sameAs;
	   }
	   if (str_startswith($sameAs, "http://dimenovels.org")) {
			$leblanc = $sameAs;
	   }
	   if (str_startswith($sameAs, "http://viaf.org")) {
			$viaf = $sameAs;
	   }
    }
	if ($rel['predicate']['namespace'] == "http://xmlns.com/foaf/0.1/") {
		$wiki_page = $rel['object']['value'];
	}
  }
  
  $variables['related'] = $related;
  $variables['leblanc'] = $leblanc;
  $variables['viaf'] = $viaf;
  $variables['wiki_page'] = $wiki_page;
  
  require_once "easyrdf/lib/EasyRdf.php";
  
  EasyRdf_Namespace::set('category', 'http://dbpedia.org/resource/Category:');
  EasyRdf_Namespace::set('dbpedia', 'http://dbpedia.org/resource/');
  EasyRdf_Namespace::set('dbo', 'http://dbpedia.org/ontology/');
  EasyRdf_Namespace::set('dbp', 'http://dbpedia.org/property/');
  
  $sparql = new EasyRdf_Sparql_Client('http://dbpedia.org/sparql');
  
  /* DBPedia seems to go down a lot. Check to make sure we actually have a connection first. **/
  if ($sparql) {
	  $wikipedia = $sparql->query(
			'SELECT ?abstract WHERE {'.
			'  <'.$wkp.'> dbo:abstract ?abstract .'.
			'  FILTER ( langMatches(lang(?abstract), "EN") )'.
			'}'
		);
	
	
	  foreach ($wikipedia as $row) {
			$abstract = $row->abstract;
	  }
		
	  $variables['abstract'] = $abstract;
  }

  if ($object['MADS']) {
    $mads = $object['MADS']->content;
    $simplexml = simplexml_load_string($mads);
/**    $notes = $simplexml->note;
    foreach ($notes as $note) {
      if ($note['type'] == 'history') {
        $biography = (string) $note;
        $variables['biography'] = str_replace("\n", "<br />", $biography);
      }
    }*/
    $variables['title'] = (string) $simplexml->authority->titleInfo->title;
    $activities = $simplexml->fieldOfActivity;
    foreach ($activities as $activity) {
      if ((string) $activity) {
        $fields_of_activity[] = (string) $activity;
      }
    }
    if (isset($fields_of_activity)) {
      $list_variables = array(
        'items' => $fields_of_activity,
        'title' => t('Research Interests'),
        'type' => 'ul',
        'attributes' => array('class' => 'research_interests'),
      );
      $variables['activities'] = theme_item_list($list_variables);
    }
    $depts = $simplexml->affiliation->organization;
    $identifiers = $simplexml->identifier;
    foreach ($identifiers as $identifier) {
      if ($identifier['type'] == 'u1' && (string) $identifier) {
        $themed_links = islandora_entities_get_related((string) $identifier, $variables['title'], 'citations');
      }
    }
    $name = $variables['title'];
    // Add the RSS image, with menu callback.
    $form = array();
    $form['bookmark_rss'] = array(
      '#weight' => -1,
      '#markup' => l(theme('image', array(
          'path' => drupal_get_path('module', 'islandora_entities') . '/images/rss.png',
          'title' => t('Present Entity data as list in an RSS form'),
          'attributes' => array(),
          )
        ), "islandora/entities/$object_pid/rss", array('html' => TRUE)),
      '#prefix' => '<div><div id="islandora-entities-rss-format" style="float:left;">',
      '#suffix' => '</div><div class="islandora-entities-rss-format-name" style="float:left;">Subscribe to ' . $name . ' RSS feed</div></div>',
    );
    $variables['rss_feed'] = drupal_render($form);
    if (!empty($themed_links)) {
      $variables['recent_citations'] = $themed_links;
    }
  }
  if ($object["TN"]) {
    $variables['tn'] = "/islandora/object/$object_pid/datastream/TN/view";
  }
  $variables['metadata'] = islandora_retrieve_metadata_markup($object, TRUE);
}