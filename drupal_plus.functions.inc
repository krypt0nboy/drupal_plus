<?php

/**
 * @file
 *
 * This file provides with very handy functions.
 */

/**
 * Returns a module's path.
 *
 * @param string $module
 *   The module's name.
 *
 * @return string
 */
function module_get_path($module) {
  return drupal_get_path('module', $module);
}

/**
 * Returns the path of a module's info file.
 *
 * @param string $module
 *   The module's name.
 *
 * @return string
 */
function module_info_file_get_path($module) {
  return module_get_path($module) . '/' . $module . '.info';
}

/**
 * Returns the path of a module's .module file.
 *
 * @param string $module
 *   The module's name.
 *
 * @return string
 */
function module_file_get_path($module) {
  return module_get_path($module) . '/' . $module . '.module';
}

/**
 * Returns the module parsed info file.
 *
 * @param string $module
 *   The module's name.
 *
 * @return mixed
 */
function module_parsed_info_file($module) {
  return drupal_parse_info_file(module_info_file_get_path($module));
}

/**
 * Returns the files of a module.
 *
 * @param string $module
 *   The module's name.
 *
 * @return array
 */
function module_scan_directories($module) {
  $path  = module_get_path($module);
  $files = file_scan_directory($path, '/*/');

  return $files;
}

/**
 * Indicates if an entity is installed.
 *
 * @param string $entity_type
 *   The entity type.
 *
 * @return bool
 */
function entity_install_get_info($entity_type) {
  $installed = &drupal_static(__FUNCTION__, array());
  if (!in_array($entity_type, $installed)) {
    $info = entity_get_info();
    if (in_array($entity_type, array_keys($info))) {
      if (db_table_exists($info[$entity_type]['base table'])) {
        $installed[] = $entity_type;
      }
    }
  }

  return in_array($entity_type, $installed);
}

/**
 * Returns the inc files of a module.
 *
 * @param string $module
 *   The module's name.
 *
 * @return array
 */
function module_get_inc_files($module) {
  $path  = module_get_path($module);
  $files = file_scan_directory($path, RGX__FILE_EXT_INC);

  return $files;
}

/**
 * Returns the parsed php file of a module.
 *
 * @param string $module
 *   The module's name.
 *
 * @return array
 */
function module_get_parsed_php_files($module) {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!isset($modules[$module])) {
    $path         = module_get_path($module);
    $files        = file_scan_directory($path, RGX__FILE_EXT_INC);
    $parsed_files = array();
    foreach ($files as $file) {
      $parsed_files[$file->name] = array(
        'uri'    => $file->uri,
        'parsed' => parse_php_file($file->uri),
      );
    }
    $modules[$module] = $parsed_files;
  }

  return $modules[$module];
}

/**
 * Returns a module's classes.
 *
 * @param string $module
 *   The module's name.
 *
 * @return array
 */
function module_get_classes($module) {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!isset($modules[$module])) {
    $files   = module_get_parsed_php_files($module);
    $classes = array();
    foreach ($files as $file) {
      foreach ($file['parsed']['classes'] as $class) {
        $classes[$class] = array(
          'file' => $file['uri'],
        );
      }
    }
    $modules[$module] = $classes;
  }

  return $modules[$module];
}

/**
 * Returns a module's traits.
 *
 * @param string $module
 *   The module's name.
 *
 * @return array
 */
function module_get_traits($module) {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!isset($modules[$module])) {
    $files  = module_get_parsed_php_files($module);
    $traits = array();
    foreach ($files as $file) {
      foreach ($file['parsed']['traits'] as $trait) {
        $traits[$trait] = array(
          'file' => $file['uri'],
        );
      }
    }
    $modules[$module] = $traits;
  }

  return $modules[$module];
}

/**
 * Returns a module's entities.
 *
 * @param string $module
 *   The module's name.
 *
 * @return string[]
 */
function module_get_entities($module) {
  $modules = &drupal_static(__FUNCTION__, array());
  if (!isset($modules[$module])) {
    $invoke           = module_invoke($module, 'entity_info');
    $modules[$module] = array_keys($invoke);
  }

  return $modules[$module];
}

/**
 * Invokes a hook and alters the retrieved data.
 *
 * @param string $hook
 *   The hook to invoke.
 *
 * @return array
 */
function invoke_and_alter($hook) {
  $data = module_invoke_all($hook);
  drupal_alter($hook, $data);

  return $data;
}

/**
 * Require once a trait from a given module.
 *
 * @param string $module
 *   The module's name.
 * @param string $trait
 *   The trait to include.
 */
function module_trait_inc($module, $trait) {
  $traits = module_get_traits($module);
  if (!empty($traits[$trait])) {
    require_once $traits[$trait]['file'];
  }
}

/**
 * Require once a class from a given module.
 *
 * @param string $module
 *   The module's name.
 * @param string $class
 *   The class to include.
 */
function module_class_inc($module, $class) {
  $classes = module_get_classes($module);
  if (!empty($classes[$class])) {
    require_once $classes[$class]['file'];
  }
}