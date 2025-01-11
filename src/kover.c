#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
// ---------

// The help to display
#define HELP "Usage: kover SUBCOMMAND\n\
Handles positioning of communication antennas by reading a scene on stdin.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
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
  4. A building line can start with space characters, followed by an\n\
     instruction of the form 'building ID X Y RX RY', where\n\
       ID is the building identifier\n\
       X is the x-coordinate of the building\n\
       Y is the y-coordinate of the building\n\
       RX is the x-radius of the building\n\
       RY is the y-radius of the building\n\
  5. An antenna line can start with space characters, followed by an\n\
     instruction of the form 'antenna ID X Y R', where\n\
       ID is the building identifier\n\
       X is the x-coordinate of the antenna\n\
       Y is the y-coordinate of the antenna\n\
       R is the radius scope of the antenna\n"

// The maximum length of a line in a scene stream
#define MAX_LENGTH 50
// The maximum length of an identifier
#define MAX_LENGTH_ID 10
// The maximum number of buildings in a scene
#define NUM_MAX_BUILDINGS 100
// The maximum number of antennas in a scene
#define NUM_MAX_ANTENNAS 100

// Types
// -----

// A building
struct Building {
  // The identifier of the building
  char id[MAX_LENGTH_ID + 1];
  // The x coordinate of the building
  int x;
  // The y coordinate of the building
  int y;
  // The x radius of the building (half length in x direction)
  int rx;
  // The y radius of the building (half length in y direction)
  int ry;
};

// An antenna
struct Antenna {
  // The identifier of the antenna
  char id[MAX_LENGTH_ID + 1];
  // The x coordinate of the antenna
  int x;
  // The y coordinate of the antenna
  int y;
  // The radius of the antenna
  int r;
};

// A scene
struct Scene {
  // The number of building in the scene
  unsigned int num_buildings;
  // The buildings of the scene
  struct Building buildings[NUM_MAX_BUILDINGS];
  // The number of antennas in the scene
  unsigned int num_antennas;
  // The antennas of the scene
  struct Antenna antennas[NUM_MAX_ANTENNAS];
};

// Error reporting
// ---------------

/**
 * Reports on stderr that a given object identifier is not unique
 *
 * @param object  The object
 * @param id      The identifier
 */
void report_error_non_unique_identifiers(const char* object, const char* id) {
  fprintf(stderr, "error: %s identifier %s is non unique\n", object, id);
  exit(1);
}

/**
 * Reports on stderr that the first line of a scene is invalid
 */
void report_error_scene_first_line(void) {
  fprintf(stderr, "error: first line must be exactly 'begin scene'\n");
  exit(1);
}

/**
 * Reports on stderr that a scene line is not recognized
 *
 * @param line_number  The number of the unrecognized line
 */
void report_error_unrecognized_line(int line_number) {
  fprintf(stderr, "error: unrecognized line (line #%d)\n", line_number);
  exit(1);
}

/**
 * Reports on stderr that the last line of a scene is invalid
 */
void report_error_scene_last_line(void) {
  fprintf(stderr, "error: last line must be exactly 'end scene'\n");
  exit(1);
}

/**
 * Reports on stderr that two buildings are overlapping
 *
 * @param id1  The identifier of the first building
 * @param id2  The identifier of the second building
 */
void report_error_overlapping_buildings(const char* id1, const char* id2) {
  fprintf(stderr, "error: buildings %s and %s are overlapping\n", id1, id2);
  exit(1);
}

/**
 * Reports on stderr that the subcommand is mandatory
 */
void report_error_mandatory_subcommand(void) {
  fprintf(stderr, "error: subcommand is mandatory\n");
  exit(1);
}

/**
 * Reports on stderr that the subcommand is unrecognized
 */
void report_error_unrecognized_subcommand(const char* subcommand) {
  fprintf(stderr, "error: subcommand '%s' is not recognized\n", subcommand);
  exit(1);
}

// Scene construction
// ------------------

/**
 * Initializes an empty scene
 *
 * @param scene  The scene to initialize
 */
void initialize_empty_scene(struct Scene* scene) {
  scene->num_buildings = 0;
}

/**
 * Adds a building to a scene
 *
 * @param scene     The scene to which the building is added
 * @param building  The building to add
 */
void add_building(struct Scene* scene, const struct Building* building) {
  unsigned int b = 0;
  while (b < scene->num_buildings &&
         strcmp(building->id, scene->buildings[b].id) > 0)
    ++b;
  if (strcmp(building->id, scene->buildings[b].id) == 0)
    report_error_non_unique_identifiers("building", building->id);
  for (int b2 = scene->num_buildings; b2 > b; --b2)
    scene->buildings[b2] = scene->buildings[b2 - 1];
  struct Building* scene_building = scene->buildings + b;
  strncpy(scene_building->id, building->id, MAX_LENGTH_ID);
  scene_building->x = building->x;
  scene_building->y = building->y;
  scene_building->rx = building->rx;
  scene_building->ry = building->ry;
  ++scene->num_buildings;
}

/**
 * Adds an antenna to a scene
 *
 * @param scene    The scene to which the antenna is added
 * @param antenna  The antenna to add
 */
void add_antenna(struct Scene* scene, const struct Antenna* antenna) {
  unsigned int a = 0;
  while (a < scene->num_antennas &&
         strcmp(antenna->id, scene->antennas[a].id) > 0)
    ++a;
  if (strcmp(antenna->id, scene->antennas[a].id) == 0)
    report_error_non_unique_identifiers("antenna", antenna->id);
  for (int a2 = scene->num_buildings; a2 > a; --a2)
    scene->antennas[a2] = scene->antennas[a2 - 1];
  struct Antenna* scene_antenna = scene->antennas + a;
  strncpy(scene_antenna->id, antenna->id, MAX_LENGTH_ID);
  scene_antenna->x = antenna->x;
  scene_antenna->y = antenna->y;
  scene_antenna->r = antenna->r;
  ++scene->num_antennas;
}

/**
 * Prints a summary of the scene on stdout
 *
 * @param scene  The scene to print
 */
void print_scene_summary(const struct Scene* scene) {
  if (scene->num_buildings == 0 && scene->num_antennas == 0)
    puts("An empty scene");
  else if (scene->num_antennas == 0)
    printf("A scene with %d building%s\n", scene->num_buildings,
           scene->num_buildings > 1 ? "s" : "");
  else if (scene->num_buildings == 0)
    printf("A scene with %d antenna%s\n", scene->num_antennas,
           scene->num_antennas > 1 ? "s" : "");
}

/**
 * Prints the building of the scene to stdout
 *
 * @param scene  The scene whose building are printed
 */
void print_scene_buildings(const struct Scene* scene) {
  for (int b = 0; b < scene->num_buildings; ++b) {
    const struct Building* building = scene->buildings + b;
    printf("  building %s at %d %d with dimensions %d %d\n",
           building->id, building->x, building->y, building->rx, building->ry);
  }
}

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
 * Indicates if the line is a building line
 *
 * A building line is a line that matches the BRE regex
 *
 *   ^[:blank:]*building
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_building_line(const char* line) {
  while (isblank(*line))
    ++line;
  return line != NULL && strncmp(line, "building", 8) == 0;
}

/**
 * Indicates if the line is an antenna line
 *
 * An antenna line is a line that matches the BRE regex
 *
 *   ^[:blank:]*antenna
 *
 * @param line  The line to check
 * @return      true if and only if the line is valid
 */
bool is_antenna_line(const char* line) {
  while (isblank(*line))
    ++line;
  return line != NULL && strncmp(line, "antenna", 7) == 0;
}

/**
 * Loads a building from a line
 *
 * @param building  The loaded building
 * @param line      The line
 */
void load_building_from_line(struct Building* building, const char* line) {
  char line_copy[MAX_LENGTH + 1];
  strncpy(line_copy, line, MAX_LENGTH);
  char *token;
  // 1st token should be building
  token = strtok(line_copy, " ");
  if (token == NULL || strcmp(token, "building") != 0) return;
  // 2nd token should be the identifier
  token = strtok(NULL, " ");
  if (token == NULL) return;
  strncpy(building->id, token, MAX_LENGTH_ID);
  // 3rd token should be x
  token = strtok(NULL, " ");
  if (token == NULL) return;
  building->x = atoi(token);
  // 4th token should be y
  token = strtok(NULL, " ");
  if (token == NULL) return;
  building->y = atoi(token);
  // 5th token should be rx
  token = strtok(NULL, " ");
  if (token == NULL) return;
  building->rx = atoi(token);
  // 6th token should be ry
  token = strtok(NULL, " ");
  if (token == NULL) return;
  building->ry = atoi(token);
}

/**
 * Loads an antenna from a line
 *
 * @param antenna  The loaded antenna
 * @param line     The line
 */
void load_antenna_from_line(struct Antenna* antenna, const char* line) {
  char line_copy[MAX_LENGTH + 1];
  strncpy(line_copy, line, MAX_LENGTH);
  char *token;
  // 1st token should be antenna
  token = strtok(line_copy, " ");
  if (token == NULL || strcmp(token, "antenna") != 0) return;
  // 2nd token should be the identifier
  token = strtok(NULL, " ");
  if (token == NULL) return;
  strncpy(antenna->id, token, MAX_LENGTH_ID);
  // 3rd token should be x
  token = strtok(NULL, " ");
  if (token == NULL) return;
  antenna->x = atoi(token);
  // 4th token should be y
  token = strtok(NULL, " ");
  if (token == NULL) return;
  antenna->y = atoi(token);
  // 5th token should be r
  token = strtok(NULL, " ");
  if (token == NULL) return;
  antenna->r = atoi(token);
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
  struct Building building;
  struct Antenna antenna;
  int line_number = 1;
  while (fgets(line, MAX_LENGTH, stdin) != NULL) {
    last_line = false;
    line[strcspn(line, "\n")] = '\0';
    if (first_line) {
      if (!is_begin_scene_line(line))
        report_error_scene_first_line();
      first_line = false;
    } else if (is_building_line(line)) {
      load_building_from_line(&building, line);
      add_building(scene, &building);
    } else if (is_antenna_line(line)) {
      load_antenna_from_line(&antenna, line);
      add_antenna(scene, &antenna);
    } else if (is_end_scene_line(line)) {
      last_line = true;
    } else {
      report_error_unrecognized_line(line_number);
    }
    ++line_number;
  }
  if (!last_line)
    report_error_scene_last_line();
}

// Scene validation
// ----------------

/**
 * Indicates if two intervals are overlapping
 *
 * @param a1  The start of the first interval
 * @param b1  The end of the first interval
 * @param a2  The start of the second interval
 * @param b2  The end of the second interval
 */
bool are_intervals_overlapping(int a1, int b1, int a2, int b2) {
  return (a1 <= a2 && a2 < b1 && b1 <= b2) ||
         (a2 <= a1 && a1 < b2 && b2 <= b1);
}

/**
 * Indicates if two buildings are overlapping
 *
 * Two building are overlapping if their intersection has a strictly positive
 * area.
 *
 * @param building1  The first building
 * @param building2  The second building
 */
bool are_building_overlapping(const struct Building* building1,
                              const struct Building* building2) {
  return are_intervals_overlapping(building1->x - building1->rx,
                                   building1->x + building1->rx,
                                   building2->x - building2->rx,
                                   building2->x + building2->rx) &&
         are_intervals_overlapping(building1->y - building1->ry,
                                   building1->y + building1->ry,
                                   building2->y - building2->ry,
                                   building2->y + building2->ry);
}

/**
 * Checks if a scene is valid
 *
 * If the scene is invalid, an error is printed on stdout and the program exits
 * with 1.
 *
 * @param scene  The scene to validate
 */
void validate_scene(const struct Scene* scene) {
  for (int b1 = 0; b1 < scene->num_buildings; ++b1)
    for (int b2 = b1 + 1; b2 < scene->num_buildings; ++b2) {
      const struct Building* building1 = scene->buildings + b1,
                           * building2 = scene->buildings + b2;
      if (are_building_overlapping(building1, building2))
        report_error_overlapping_buildings(building1->id, building2->id);
    }
}

// Subcommands processing
// ----------------------

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

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand(void) {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  validate_scene(&scene);
  print_scene_summary(&scene);
  print_scene_buildings(&scene);
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

  if (strcmp(subcommand, "help") == 0) {
    print_help();
  } else if (strcmp(subcommand, "summarize") == 0) {
    run_summarize_subcommand();
  } else if (strcmp(subcommand, "describe") == 0) {
    run_describe_subcommand();
  } else {
    report_error_unrecognized_subcommand(subcommand);
  }
  return 0;
}
