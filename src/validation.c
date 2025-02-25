#include "validation.h"

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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

// Error reporting
// ---------------

void report_error_non_unique_identifiers(const char* object, const char* id) {
  fprintf(stderr, "error: %s identifier %s is non unique\n", object, id);
  exit(1);
}

void report_error_invalid_identifier(const char* id, int line_number) {
  fprintf(stderr, "error: invalid identifier \"%s\" (line #%d)\n", id,
          line_number);
  exit(1);
}

void report_error_invalid_int(const char* s, int line_number) {
  fprintf(stderr, "error: invalid integer \"%s\" (line #%d)\n", s, line_number);
  exit(1);
}

void report_error_invalid_positive_int(const char* s, int line_number) {
  fprintf(stderr, "error: invalid positive integer \"%s\" (line #%d)\n", s,
          line_number);
  exit(1);
}

void report_error_scene_first_line(void) {
  fprintf(stderr, "error: first line must be exactly 'begin scene'\n");
  exit(1);
}

void report_error_unrecognized_line(int line_number) {
  fprintf(stderr, "error: unrecognized line (line #%d)\n", line_number);
  exit(1);
}

void report_error_line_wrong_arguments_number(const char* object,
                                              int line_number) {
  fprintf(stderr,
          "error: %s line has wrong number of arguments (line #%d)\n",
          object,
          line_number);
  exit(1);
}

void report_error_scene_last_line(void) {
  fprintf(stderr, "error: last line must be exactly 'end scene'\n");
  exit(1);
}

void report_error_overlapping_buildings(const char* id1, const char* id2) {
  fprintf(stderr, "error: buildings %s and %s are overlapping\n", id1, id2);
  exit(1);
}

void report_error_same_position_antennas(const char* id1, const char* id2) {
  fprintf(stderr, "error: antennas %s and %s have the same position\n",
          id1, id2);
  exit(1);
}

void report_error_mandatory_subcommand(void) {
  fprintf(stderr, "error: subcommand is mandatory\n");
  exit(1);
}

void report_error_unrecognized_subcommand(const char* subcommand) {
  fprintf(stderr, "error: subcommand '%s' is not recognized\n", subcommand);
  exit(1);
}
