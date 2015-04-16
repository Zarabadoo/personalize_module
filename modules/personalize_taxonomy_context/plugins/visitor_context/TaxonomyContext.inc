<?php

/**
 * @file
 * Provides a visitor context plugin for taxonomy.

 */

class TaxonomyContext extends PersonalizeContextBase {
  /**
   * Implements PersonalizeContextInterface::create().
   */
  public static function create(PersonalizeAgentInterface $agent = NULL, $selected_context = array()) {
    return new self($agent, $selected_context);
  }

  /**
   * Implements PersonalizeContextInterface::getOptions().
   */
  public static function getOptions() {
    $vocabularies = taxonomy_get_vocabularies();
    $free_tagging_vocabulary_machine_names = SELF::getFreeTaggingVocabulariesMachineNames();
    $options = array();
    foreach ($vocabularies as $vocabulary) {
      $machine_name = $vocabulary->machine_name;
      // If the vocabulary is a free-tagging vocabulary, exclude it.
      if (in_array($machine_name, $free_tagging_vocabulary_machine_names)) {
        continue;
      }
      $vid = $vocabulary->vid;
      $name = $vocabulary->name;
      $options[$machine_name] = array(
        'name' => $name,
        'group' => 'Taxonomy',
        'id' => $vid,
      );
    }
    return $options;
  }

  /**
   * Implements PersonalizeContextInterface::getPossibleValues().
   */
  public function getPossibleValues($limit = FALSE) {
    $possible_values = array();
    $options = $this->getOptions();
    foreach ($options as $machine_name => $info) {
      $terms = taxonomy_get_tree($info['id']);
      $values = array();
      foreach($terms as $term) {
        $values[$term->tid] = $term->name;
      }
      $name = $options[$machine_name]['name'];
      $possible_values[$name] = array(
        'value type' => 'predefined',
        'values' => $values,
      );
    }
    return $possible_values;
  }

/**
 * Get an array of machine names that are of free tagging vocabularies.
 *
 * @return array
 *   An array of machine names.
 */
  public static function getFreeTaggingVocabulariesMachineNames() {
    // Free-tagging vocabulary "is generally accomplished by using a term reference
    // field with an auto-complete widget on the content creation page."
    $query =
      'SELECT bundle ' .
      'FROM {field_config_instance} ' .
      'WHERE entity_type = \'taxonomy_term\' ' .
      'AND data LIKE \'%s:4:"type";s:21:"taxonomy_autocomplete"%\'';
    $results = db_query($query);
    return $results->fetchCol() ?: array();
  }
}
