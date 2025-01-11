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

// A scene
struct Scene {
  // The number of building in the scene
  unsigned int num_buildings;
  // The buildings of the scene
  struct Building buildings[NUM_MAX_BUILDINGS];
};

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
  if (strcmp(building->id, scene->buildings[b].id) == 0) {
    fprintf(stderr, "error: building identifier %s is non unique\n",
            building->id);
    exit(1);
  }
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
 * Prints a summary of the scene on stdout
 *
 * @param scene  The scene to print
 */
void print_scene_summary(const struct Scene* scene) {
  if (scene->num_buildings == 0)
    puts("An empty scene");
  else
    printf("A scene with %d building%s\n", scene->num_buildings,
           scene->num_buildings > 1 ? "s" : "");
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
 * A building line is a line that verifies the BRE regex
 *
 *   [:blank:]*building
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
 * Loads a scene from the standard input
 *
 * @param scene  The resulting scene
 */
void load_scene_from_stdin(struct Scene* scene) {
  initialize_empty_scene(scene);
  char line[MAX_LENGTH + 1];
  bool first_line = true, last_line = false;
  struct Building building;
  while (fgets(line, MAX_LENGTH, stdin) != NULL) {
    last_line = false;
    line[strcspn(line, "\n")] = '\0';
    if (first_line) {
      if (!is_begin_scene_line(line)) {
        fprintf(stderr, "error: first line must be exactly 'begin scene'");
        exit(1);
      }
      first_line = false;
    } else if (is_building_line(line)) {
      load_building_from_line(&building, line);
      add_building(scene, &building);
    } else if (is_end_scene_line(line)) {
      last_line = true;
    } else {
      exit(1);
    }
  }
  if (!last_line) {
    fprintf(stderr, "error: last line must be exactly 'end scene'");
    exit(1);
  }
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
      if (are_building_overlapping(building1, building2)) {
        fprintf(stderr, "error: buildings %s and %s are overlapping\n",
                building1->id, building2->id);
        exit(1);
      }
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
    return 1;
  const char* subcommand = argv[1];

  if (strcmp(subcommand, "help") == 0) {
    print_help();
    return 0;
  } else if (strcmp(subcommand, "summarize") == 0) {
    run_summarize_subcommand();
    return 0;
  } else if (strcmp(subcommand, "describe") == 0) {
    run_describe_subcommand();
    return 0;
  } else {
    return 1;
  }
}
