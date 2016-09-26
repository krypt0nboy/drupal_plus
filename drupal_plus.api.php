<?php

/**
 * @file
 *
 * This file provides with the module's API documentation.
 *
 * @author Harold Cohen <me@harold-cohen.com>
 */

/**
 * Declares cache cids for a module cache table.
 *
 * @return array
 *   An associative array where the keys are the cache name and whose values are :
 *   - expires : The duration of the cache in seconds.
 *   - clear : An array of cache cids to clear when clearing this one.
 *   - hooks : An array where the values hook names, which when invoked will clear the cid cache.
 */
function hook_cache_info() {
  $info                             = array();
  $info['cache_table']['cache_cid'] = array(
    'expires' => CACHE_PERMANENT,
    'clear'   => array(),
    'hooks'   => array(),
  );

  return $info;
}