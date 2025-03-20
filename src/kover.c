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
 * Runs the help subcommand
 */
void run_help_subcommand(void) {
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

/**
 * Runs the validate subcommand
 */
void run_validate_subcommand(void) {
  struct Scene scene;
  struct ValidationError error;
  
  load_scene_from_stdin(&scene);
  init_validation_error(&error);
  
  if (is_scene_valid(&scene, &error)) {
    printf("ok\n");
  } else {
    printf("not ok\n");
    fprintf(stderr, "error: %s\n", error.message);
    exit(1);
  }
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
  if (strcmp(subcommand, "bounding-box") == 0)
    run_bounding_box_subcommand();
  else if (strcmp(subcommand, "describe") == 0)
    run_describe_subcommand();
  else if (strcmp(subcommand, "help") == 0)
    run_help_subcommand();
  else if (strcmp(subcommand, "summarize") == 0)
    run_summarize_subcommand();
  else if (strcmp(subcommand, "validate") == 0)
    run_validate_subcommand();
  else
    report_error_unrecognized_subcommand(subcommand);
  return 0;
}
