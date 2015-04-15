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
      $machine_name = $vocabulary->machine_name;
      $name = $vocabulary->name;
      $options[$machine_name] = array(
        'name' => $name,
        'group' => 'Taxonomy',
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
    foreach ($options as $name => $info) {
      $possible_values[$name] = array(
        'value type' => 'string',
      );
    }

    return $possible_values;
  }
}
