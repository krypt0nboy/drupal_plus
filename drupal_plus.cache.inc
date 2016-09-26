<?php

/**
 * @file
 *
 * This file provides with cache related functions.
 */

/**
 * Numbers of seconds in an hour.
 */
define('CACHE_HOURLY', 3600);

/**
 * Number of seconds in a day.
 */
define('CACHE_DAILY', 86400);

/**
 * Number of seconds in a week.
 */
define('CACHE_WEEKLY', 604800);

/**
 * Cid to store cache_info.
 */
define('DRUPAL_PLUS_CACHE_INFO', 'cache_info');

/**
 * Clear the cache for every entity_toolbox cache id.
 */
function drupal_plus_cc_all() {
  cache_clear_all('*', DRUPAL_PLUS_DB__CACHE, TRUE);
}

/**
 * Sets cache in a cache table.
 *
 * @param string $table
 *   The table to cache set to.
 * @param string $cid
 *   The cache ID.
 * @param mixed  $data
 *   The data to cache.
 * @param int    $expire
 *   The expiration in seconds.
 */
function generic_cache_set($table, $cid, $data, $expire = CACHE_PERMANENT) {
  $cache_enabled = variable_get('dp_cache_enabled');
  list($expires, $cache) = array_values(drupal_plus_cache_get_info($table, $cid));
  if ($cache_enabled && $cache) {
    cache_set($cid, $data, $table, isset($expires) ? $expires : $expire);
  }
}

/**
 * Gets cache from a table.
 *
 * @param string $table
 *   The table to clear.
 * @param string $cid
 *   The cache ID.
 *
 * @return null|object
 */
function generic_cache_get($table, $cid) {
  return cache_get($cid, $table);
}

/**
 * Returns cache data.
 *
 * @param string $table
 *   The table to clear.
 * @param string $cid
 *   The cache ID.
 *
 * @return false|array
 */
function generic_cache_get_data($table, $cid) {
  $cache = generic_cache_get($table, $cid);

  return $cache ? $cache->data : $cache;
}

/**
 * Clear a table specific cache cid.
 * Once cleared, related cache IDs are cleared as well.
 *
 * @param string $table
 *   The table to clear.
 * @param string $cid
 *   The cache ID.
 *
 * @see hook_cache_info().
 */
function generic_cache_clear($table, $cid) {
  cache_clear_all($cid, $table);
  $c2clrs = drupal_plus_cache_cascade_clear_get_info($table, $cid);
  foreach ($c2clrs as $c2clr) {
    cache_clear_all($c2clr, $table);
  }
}

/**
 * Sets cache in drupal_plus cache table.
 *
 * @param string $cid
 *   The cache ID.
 * @param mixed  $data
 *   The data to cache.
 * @param int    $expire
 *   The expiration in seconds.
 */
function drupal_plus_cache_set($cid, $data, $expire = CACHE_PERMANENT) {
  generic_cache_set(DRUPAL_PLUS_DB__CACHE, $cid, $data, $expire);
}

/**
 * Returns cache from cache_drupal_plus.
 * Gets the cache from drupal_plus cache table.
 *
 * @param $cid
 *   The cache ID.
 *
 * @return null|object
 */
function drupal_plus_cache_get($cid) {
  return generic_cache_get(DRUPAL_PLUS_DB__CACHE, $cid);
}

/**
 * Returns the cache data from drupal_plus cache table.
 *
 * @param $cid
 *   The cache ID.
 *
 * @return array|false
 */
function drupal_plus_cache_get_data($cid) {
  return generic_cache_get_data(DRUPAL_PLUS_DB__CACHE, $cid);
}

/**
 * Implements hook_cache_info().
 */
function drupal_plus_cache_info() {
  $info                                           = array();
  $info[DRUPAL_PLUS_DB__CACHE]['module_info']     = array();
  $info[DRUPAL_PLUS_DB__CACHE]['modules_classes'] = array();

  return $info;
}