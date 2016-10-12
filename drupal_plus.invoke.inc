<?php

/**
 * @file
 *
 * This file provides with hook invoke functions.
 */

/**
 * Returns cache info for a given cache table.
 *
 * @param null|string $table
 *   The table to get info for.
 * @param null|string $cid
 *   The cache cid to get info for.
 *
 * @return array
 *
 * @see hook_cache_info().
 */
function drupal_plus_cache_get_info($table = NULL, $cid = NULL) {
  $hook = 'cache_info';
  // Cached since used very often
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast[$hook] = &drupal_static(__FUNCTION__);
  }
  $cache_info = &$drupal_static_fast[$hook];
  if (empty($cache_info)) {
    if ($cache = cache_get($hook)) {
      $cache_info = $cache->data;
    }
    else {
      $data = module_invoke_all($hook);
      drupal_alter($hook, $data);
      $info = array();
      /**
       * @var string  $key
       *   A cache table.
       * @var array[] $values
       *   Cache cids values.
       */
      foreach ($data as $key => $values) {
        /**
         * @var string $id
         *   A cache cid.
         * @var array  $dt
         *   A cache cid info.
         */
        foreach ($values as $id => $dt) {
          $info[$key][$id] = $dt;
          $info[$key][$id] += array(
            'cache'   => TRUE,
            'expires' => CACHE_PERMANENT
          );
        }
      }

      $cache_info = $info;
      cache_set(DRUPAL_PLUS_CACHE_INFO, $cache_info);
    }
  }

  if (isset($table)) {
    return isset($cid) ? $cache_info[$table][$cid] : $cache_info[$table];
  }
  else {
    return $cache_info;
  }
}

/**
 * Returns the hooks that trigger a cache to be cleared.
 *
 * @param string $hook
 *   The hook name.
 *
 * @return array[]
 */
function drupal_plus_clear_cache_triggers_get_info($hook) {
  $hooks = &drupal_static(__FUNCTION__);
  if (empty($hooks[$hook])) {
    $cache_info = drupal_plus_cache_get_info();
    foreach ($cache_info as $table => $cids) {
      $buffer = array();
      foreach ($cids as $cid => $info) {
        if (in_array($hook, $info['hooks'])) {
          $buffer[] = $cid;
        }
      }
      if (!empty($buffer)) {
        $hooks[$hook][$table] = $buffer;
      }
    }
  }

  return $hooks[$hook];
}

/**
 * Returns a list of hooks set to clear a cache cid when invoked.
 *
 * @return string[]
 */
function drupal_plus_cache_hooks_get_info() {
  $hooks = &drupal_static(__FUNCTION__, array());
  if (!isset($hooks)) {
    $cache_info = drupal_plus_cache_get_info();
    $buffer     = array();
    foreach ($cache_info as $cid => $info) {
      foreach ($info['hooks'] as $hook) {
        $buffer[] = $hook;
      }
    }
    $hooks = array_unique($buffer);
  }

  return $hooks;
}

/**
 * Returns the cache bins and cids to clear when a cache has been cleared.
 *
 * @param string $bin
 *   The cache bin table.
 * @param string $cid
 *   The cache cid.
 *
 * @return array
 */
function drupal_plus_cache_cascade_clear_get_info($bin, $cid) {
  $clears = &drupal_static(__FUNCTION__, array());
  if (empty($clears[$bin])) {
    $cache_info = drupal_plus_cache_get_info();
    foreach ($cache_info as $bin => $cids) {
      $buffer = array();
      foreach ($cids as $cid => $info) {
        if (!empty($info['clears'])) {
          $buffer[$cid] = $info['clears'];
        }
      }
      if (!empty($buffer)) {
        $clears[$bin] = $buffer;
      }
    }
  }

  if (!empty($clears[$bin])) {
    return !empty($clears[$bin][$cid]) ? $clears[$bin][$cid] : array();
  }
  else {
    return array();
  }
}

/**
 * Returns a module info.
 *
 * @param string $module
 *   The module's name.
 *
 * @return mixed
 */
function module_get_info($module) {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!in_array($module, $modules)) {
    $info           = system_get_info('module', $module);
    $buffer         = array();
    $buffer['path'] = drupal_get_path('module', $module);
    $buffer += $info;
    $modules[$module] = $buffer;
  }

  return $modules[$module];
}

/**
 * Retrieves any module that implement a drupal plus feature in its .info file.
 *
 * @param null|string $module
 *   The module's name.
 *
 * @return array
 */
function drupal_plus_implementations($module = NULL) {
  $cid  = 'implementations';
  $hook = 'implementations';
  // Cached since used very often
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast[$hook] = &drupal_static(__FUNCTION__);
  }
  $implementations = &$drupal_static_fast[$hook];
  if (empty($implementations)) {
    if ($cache = drupal_plus_cache_get($cid)) {
      $implementations = $cache->data;
    }
    else {
      $modules_info    = system_get_info('module');
      $implementations = array();
      foreach ($modules_info as $name => $info) {
        if (!empty($info['drupal_plus'])) {
          $implementations[$module] = $info['drupal_plus'];
        }
      }
      drupal_plus_cache_set($cid, $implementations);
    }
  }

  return isset($module) ? $implementations[$module] : $implementations;
}

/**
 * Clears cache when a hook is invoked.
 * The hook has to be invoked by a module to be able to clear the cache using this function.
 *
 * @param string $hook
 *   The hook name.
 */
function _cache_clear($hook) {
  $tables = drupal_plus_clear_cache_triggers_get_info($hook);
  if (!empty($tables)) {
    foreach ($tables as $table => $cids) {
      $cleared = array();
      foreach ($cids as $cid) {
        //Prevents the same cache cid to be cleared twice.
        if (!in_array($cid, $cleared)) {
          generic_cache_clear($table, $cid);
          $cleared[] = $cid;
        }
      }
    }
  }
}

/**
 * Retrieves permission tpl info.
 *
 * @param null|string $name
 *   The permission name.
 *
 * @return array
 *
 * @see hook_permission_tpl_info().
 */
function drupal_plus_permission_tpl_get_info($name = NULL) {
  $cid  = 'permission_tpl';
  $hook = 'permission_tpl_info';
  // Cached since used very often
  static $drupal_static_fast;
  if (!isset($drupal_static_fast)) {
    $drupal_static_fast[$hook] = &drupal_static(__FUNCTION__);
  }
  $permission_tpl_info = &$drupal_static_fast[$hook];
  if (empty($permission_tpl_info)) {
    if ($cache = drupal_plus_cache_get($cid)) {
      $permission_tpl_info = $cache->data;
    }
    else {
      $invoke = invoke_and_alter($cid);
      $buffer = array();
      foreach ($invoke as $permission => $info) {
        $tpl                 = $info['tpl'];
        $info['components']  = !empty($info['components']) ? $info['components'] : array();
        $info['title']       = !empty($info['title']) ? $info['title'] : permission_tpl2title($tpl);
        $info['description'] = !empty($info['description']) ? $info['description'] : permission_tpl2description($tpl);
        $info['components'] += unprocessed_permission_tpl_xtract($tpl);
      }
      $permission_tpl_info = $buffer;
      drupal_plus_cache_set($cid, $permission_tpl_info);
    }
  }

  return isset($name) ? $permission_tpl_info[$name] : $permission_tpl_info;
}