#include "validation.h"
#include "scene.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Validation
// ----------

bool is_valid_id(const char* s) {
  if (*s == '\0' || (!isalpha(*s) && *s != '_'))
    return false;
  while (*s != '\0') {
    if (!isalnum(*s) && *s != '_')
      return false;
    ++s;
  }
  return true;
}

bool is_valid_integer(const char* s) {
  if (*s == '0' && *(s + 1) == '\0')
    return true;
  if (*s == '-')
    ++s;
  if (*s < '1' || *s > '9')
    return false;
  ++s;
  while (*s != '\0') {
    if (!isdigit(*s))
      return false;
    ++s;
  }
  return true;
}

bool is_valid_positive_integer(const char* s) {
  if (*s < '1' || *s > '9')
    return false;
  ++s;
  while (*s != '\0') {
    if (!isdigit(*s))
      return false;
    ++s;
  }
  return true;
}

void init_validation_error(struct ValidationError* error) {
  error->has_error = false;
  error->message[0] = '\0';
}

bool is_scene_valid(const struct Scene* scene, struct ValidationError* error) {
  init_validation_error(error);
  
  // Check for structure overlaps
  if (validate_structures_overlaps(scene, error)) {
      return false;
  }
  
  // Check for antenna issues
  if (validate_antennas(scene, error)) {
      return false;
  }
  
  return true;
}

const char* get_validation_error(const struct ValidationError* error) {
  if (error->has_error) {
    return error->message;
  }
  return "unknown error";
}

// Error reporting
// ---------------
/**
 * Report an error and exit with error code
 */
void report_error(const char* format, ...) {
  printf("not ok\n");
  
  va_list args;
  va_start(args, format);
  fprintf(stderr, "error: ");
  vfprintf(stderr, format, args);
  va_end(args);
  
  exit(1);
}

void report_error_non_unique_identifiers(const char* object, const char* id) {
  report_error("%s identifier %s is non unique\n", object, id);
}

void report_error_invalid_identifier(const char* id, int line_number) {
  report_error("invalid identifier \"%s\" (line #%d)\n", id, line_number);
}

void report_error_invalid_int(const char* s, int line_number) {
  report_error("invalid integer \"%s\" (line #%d)\n", s, line_number);
}

void report_error_invalid_positive_int(const char* s, int line_number) {
  report_error("invalid positive integer \"%s\" (line #%d)\n", s, line_number);
}

void report_error_scene_first_line(void) {
  report_error("first line must be exactly 'begin scene'\n");
}

void report_error_unrecognized_line(int line_number) {
  report_error("unrecognized line (line #%d)\n", line_number);
}

void report_error_line_wrong_arguments_number(const char* object,
                                            int line_number) {
  report_error("%s line has wrong number of arguments (line #%d)\n",
        object, line_number);
}

void report_error_scene_last_line(void) {
  report_error("last line must be exactly 'end scene'\n");
}

void report_error_overlapping_buildings(const char* id1, const char* id2) {
  report_error("buildings %s and %s are overlapping\n", id1, id2);
}

void report_error_same_position_antennas(const char* id1, const char* id2) {
  report_error("antennas %s and %s have the same position\n", id1, id2);
}

void report_error_mandatory_subcommand(void) {
  fprintf(stderr, "error: subcommand is mandatory\n");
  exit(1);
}

void report_error_unrecognized_subcommand(const char* subcommand) {
  fprintf(stderr, "error: subcommand '%s' is not recognized\n", subcommand);
  exit(1);
}