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
    $options = array();
    foreach ($vocabularies as $vocabulary) {
      $vid = $vocabulary->vid;
      $machine_name = $vocabulary->machine_name;
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
}
