<?php

/**
 * Extracts the components of a permission tpl.
 *
 * @param string $template
 *   The template to extract.
 *
 * @return array
 */
function unprocessed_permission_tpl_xtract($template) {
  $wildcards = tpl_extract_wildcard($template, TRUE);
  $xtract    = array();
  foreach ($wildcards as $wildcard) {
    $xtract[$wildcard] = "%$wildcard%";
  }

  return $xtract;
}

/**
 * Formats a permission template into a permission title.
 *
 * @param string $template
 *   The template to format.
 *
 * @return string
 */
function permission_tpl2title($template) {
  return preg_replace('/\./', ' ', $template);
}

/**
 * Formats a permission template into a description.
 *
 * @param string $template
 *   The template to format.
 *
 * @return string
 */
function permission_tpl2description($template) {
  $format = permission_tpl2title($template);

  return ucfirst($format) . '.';
}

/**
 * Extracts the components to provide arguments for when evaluating access.
 *
 * @param array $components
 *   The components to extract.
 *
 * @return array
 */
function components_require_provision_xtract($components) {
  return array_filter(array_values($components), 'is_template_value');
}