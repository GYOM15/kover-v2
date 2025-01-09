#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// Scenes
// ------

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
  struct Building* scene_building = scene->buildings + scene->num_buildings;
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
  bool first_line = true;
  struct Building building;
  while (fgets(line, MAX_LENGTH, stdin) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    if (first_line) {
      if (!is_begin_scene_line(line))
        exit(1);
      first_line = false;
    } else if (is_building_line(line)) {
      load_building_from_line(&building, line);
      add_building(scene, &building);
    } else if (is_end_scene_line(line)) {
      break;
    } else {
      exit(1);
    }
  }
}

// Subcommands processing
// ----------------------

/**
 * Runs the summarize subcommand
 */
void run_summarize_subcommand() {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  print_scene_summary(&scene);
}

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand() {
  struct Scene scene;
  load_scene_from_stdin(&scene);
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

  if (strcmp(subcommand, "summarize") == 0) {
    run_summarize_subcommand();
    return 0;
  } else if (strcmp(subcommand, "describe") == 0) {
    run_describe_subcommand();
    return 0;
  } else {
    return 1;
  }
}
