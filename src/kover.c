#include <ctype.h>
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "scene.h"
#include "validation.h"

// Constants
// ---------

// The help to display
#define HELP "Usage: kover SUBCOMMAND\n\
Handles positioning of communication antennas by reading a scene on stdin.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  bounding-box: returns a bounding box of the loaded scene\n\
  describe: describes the loaded scene in details\n\
  help: shows this message\n\
  summarize: summarizes the loaded scene\n\
\n\
A scene is a text stream that must satisfy the following syntax:\n\
\n\
  1. The first line must be exactly 'begin scene'\n\
  2. The last line must be exactly 'end scene'\n\
  3. Any line between the first and last line must either be a building line\n\
     or an antenna line\n\
  4. A building line has the form 'building ID X Y W H' (with any number of\n\
     blank characters before or after), where\n\
       ID is the building identifier\n\
       X is the x-coordinate of the building\n\
       Y is the y-coordinate of the building\n\
       W is the half-width of the building\n\
       H is the half-height of the building\n\
  5. An antenna line has the form 'antenna ID X Y R' (with any number of\n\
     blank characters before or after), where\n\
       ID is the building identifier\n\
       X is the x-coordinate of the antenna\n\
       Y is the y-coordinate of the antenna\n\
       R is the radius scope of the antenna\n"

// The maximum length of a line in a scene stream
#define MAX_LENGTH 50
// The maximum number of tokens in a line
#define MAX_NUM_TOKENS 6
// The maximum length of a token in a line
#define MAX_TOKEN_LENGTH 10

// Types
// -----

// A parsed line
struct ParsedLine {
  // The number of parsed token
  unsigned int num_tokens;
  // The tokens
  char tokens[MAX_NUM_TOKENS][MAX_TOKEN_LENGTH];
  // The number of the line
  int line_number;
};

// Scene loading
// -------------

/**
 * Indicates if the line is a valid "begin scene" line
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_begin_scene_line(const char* line) {
  return strcmp(line, "begin scene") == 0;
}

/**
 * Indicates if the line is a valid "end scene" line
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_end_scene_line(const char* line) {
  return strcmp(line, "end scene") == 0;
}

/**
 * Parses a line
 *
 * @param line         The line to parse
 * @param parsed_line  The resulting parsed line
 * @param line_number  The number of the line
 */
void parse_line(const char* line,
                struct ParsedLine* parsed_line,
                int line_number) {
  char line_copy[MAX_LENGTH + 1];
  strncpy(line_copy, line, MAX_LENGTH);
  char *token = strtok(line_copy, " ");
  int t = 0;
  while (token != NULL) {
    strncpy(parsed_line->tokens[t], token, MAX_TOKEN_LENGTH);
    ++t;
    token = strtok(NULL, " ");
  }
  parsed_line->num_tokens = t;
  parsed_line->line_number = line_number;
}

/**
 * Loads a building from a parsed line
 *
 * @param parsed_line  The parsed line
 * @param scene        The scene in which the building is loaded
 * @return             true if and only if the loading was successful
 */
bool load_building_from_parsed_line(const struct ParsedLine* parsed_line,
                                    struct Scene* scene) {
  if (strcmp(parsed_line->tokens[0], "building") != 0)
    return false;
  if (parsed_line->num_tokens != 6)
    report_error_line_wrong_arguments_number("building",
                                             parsed_line->line_number);
  if (!is_valid_id(parsed_line->tokens[1]))
      report_error_invalid_identifier(parsed_line->tokens[1],
                                      parsed_line->line_number);
  if (!is_valid_integer(parsed_line->tokens[2]))
      report_error_invalid_int(parsed_line->tokens[2],
                               parsed_line->line_number);
  if (!is_valid_integer(parsed_line->tokens[3]))
      report_error_invalid_int(parsed_line->tokens[3],
                               parsed_line->line_number);
  if (!is_valid_positive_integer(parsed_line->tokens[4]))
      report_error_invalid_positive_int(parsed_line->tokens[4],
                                        parsed_line->line_number);
  if (!is_valid_positive_integer(parsed_line->tokens[5]))
      report_error_invalid_positive_int(parsed_line->tokens[5],
                                        parsed_line->line_number);
  struct Building building;
  strncpy(building.id, parsed_line->tokens[1], MAX_LENGTH_ID);
  building.x = atoi(parsed_line->tokens[2]);
  building.y = atoi(parsed_line->tokens[3]);
  building.w = atoi(parsed_line->tokens[4]);
  building.h = atoi(parsed_line->tokens[5]);
  add_building(scene, &building);
  return true;
}

/**
 * Loads an antenna from a parsed line
 *
 * @param parsed_line  The parsed line
 * @param scene        The scene in which the antenna is loaded
 * @return             true if and only if the loading was successful
 */
bool load_antenna_from_parsed_line(const struct ParsedLine* parsed_line,
                                   struct Scene* scene) {
  if (strcmp(parsed_line->tokens[0], "antenna") != 0)
    return false;
  if (parsed_line->num_tokens != 5)
    report_error_line_wrong_arguments_number("antenna",
                                             parsed_line->line_number);
  if (!is_valid_id(parsed_line->tokens[1]))
      report_error_invalid_identifier(parsed_line->tokens[1],
                                      parsed_line->line_number);
  if (!is_valid_integer(parsed_line->tokens[2]))
      report_error_invalid_int(parsed_line->tokens[2],
                               parsed_line->line_number);
  if (!is_valid_integer(parsed_line->tokens[3]))
      report_error_invalid_int(parsed_line->tokens[3],
                               parsed_line->line_number);
  if (!is_valid_positive_integer(parsed_line->tokens[4]))
      report_error_invalid_positive_int(parsed_line->tokens[4],
                                        parsed_line->line_number);
  struct Antenna antenna;
  strncpy(antenna.id, parsed_line->tokens[1], MAX_LENGTH_ID);
  antenna.x = atoi(parsed_line->tokens[2]);
  antenna.y = atoi(parsed_line->tokens[3]);
  antenna.r = atoi(parsed_line->tokens[4]);
  add_antenna(scene, &antenna);
  return true;
}

/**
 * Loads a scene from the standard input
 *
 * @param scene  The resulting scene
 */
void load_scene_from_stdin(struct Scene* scene) {
  initialize_empty_scene(scene);
  char line[MAX_LENGTH + 1];
  bool first_line = true, last_line = false;
  int line_number = 1;
  while (fgets(line, MAX_LENGTH, stdin) != NULL) {
    last_line = false;
    line[strcspn(line, "\n")] = '\0';
    if (first_line) {
      if (!is_begin_scene_line(line))
        report_error_scene_first_line();
      first_line = false;
    } else if (is_end_scene_line(line)) {
      last_line = true;
    } else {
      struct ParsedLine parsed_line;
      parse_line(line, &parsed_line, line_number);
      if (parsed_line.num_tokens == 0) {
        fprintf(stderr, "error: line has no token\n");
        exit(1);
      }
      if (!load_building_from_parsed_line(&parsed_line, scene) &&
          !load_antenna_from_parsed_line(&parsed_line, scene))
        report_error_unrecognized_line(line_number);
    }
    ++line_number;
  }
  if (!last_line)
    report_error_scene_last_line();
}

// Subcommands processing
// ----------------------

/**
 * Runs the bounding-box subcommand
 */
void run_bounding_box_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  validate_scene(&scene);
  print_scene_bounding_box(&scene);
}

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  validate_scene(&scene);
  print_scene_summary(&scene);
  print_scene_buildings(&scene);
  print_scene_antennas(&scene);
}

/**
 * Prints the help on stdout
 */
void print_help(void) {
  printf("%s", HELP);
}

/**
 * Runs the summarize subcommand
 */
void run_summarize_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  validate_scene(&scene);
  print_scene_summary(&scene);
}

// Main function
// -------------

/**
 * Main function
 *
 * @param argc  The number of arguments
 * @param argv  The arguments
 */
int main(int argc, char* argv[]) {
  if (argc < 2)
    report_error_mandatory_subcommand();
  const char* subcommand = argv[1];

  if (strcmp(subcommand, "bounding-box") == 0) {
    run_bounding_box_subcommand();
  } else if (strcmp(subcommand, "describe") == 0) {
    run_describe_subcommand();
  } else if (strcmp(subcommand, "help") == 0) {
    print_help();
  } else if (strcmp(subcommand, "summarize") == 0) {
    run_summarize_subcommand();
  } else {
    report_error_unrecognized_subcommand(subcommand);
  }
  return 0;
}
