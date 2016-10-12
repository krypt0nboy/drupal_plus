<?php

/**
 * Returns the available elements that accept input.
 *
 * @param null|string $element
 *   The element name.
 *
 * @return array
 */
function input_elements_get_info($element = NULL) {
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast['cache'] = &drupal_static(__FUNCTION__);
  }
  $cache = &$drupal_static_fast['cache'];

  if (!isset($cache)) {
    $invoke = array_filter_by_value(invoke_and_alter('element_info'), TRUE, '#input');
    $buffer = array();
    foreach ($invoke as $element_type => $info) {
      $buffer[$element_type]          = $info;
      $buffer[$element_type]['#type'] = $element_type;
    }
    $cache = $buffer;
  }

  return isset($element) ? $cache[$element] : $cache;
}

