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

/**
 * Declares a permission template.
 *
 * @return array
 *   An associative array where the keys are the template name and the values are :
 *    - tpl : The permission template.
 *    - title : The permission title.
 *    - description : The permission description.
 */
function hook_permission_tpl_info() {
  $info                               = array();
  $info['%op%.%scope%.%arg1%.%arg2%'] = array(
    'tpl'         => '%op%.%scope%.%arg1%.%arg2%',
    'components'  => array(
      'op'    => 'some op',
      'scope' => 'some scope',
      'arg1'  => 'foo',
      'arg2'  => 'bar',
    ),
    'title'       => 'A title',
    'description' => 'A description.',
    'group'       => 'group'
  );
  $info['add_any_bundle']             = array(
    'tpl'         => 'add.any.%bundle%.%entity_type%',
    'components'  => array(
      'op'          => 'add',
      'scope'       => 'any',
      'bundle'      => '%bundle%',
      'entity_type' => '%entity_type%',
    ),
    'title'       => 'add any %bundle% %entity_type%',
    'description' => 'Add any %bundle% %entity_type%',
    'group'       => 'entities'
  );

  return $info;
}


/**
 * Declares settings variables.
 *
 * @return array
 */
function hook_settings_var_info() {
  $info     = array();
  $info[''] = array(
    'type'    => '',
    'default' => ''
  );

  return $info;
}