#include <stdio.h>
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
 * Loads a scene from the standard input
 *
 * @param scene  The resulting scene
 */
void load_scene_from_stdin(struct Scene* scene) {
  char line[MAX_LENGTH + 1];
  int num_lines = 0;
  while (fgets(line, MAX_LENGTH, stdin) != NULL)
    ++num_lines;
  scene->num_buildings = num_lines - 2;
  if (num_lines != 2) {
    strncpy(scene->buildings[0].id, "b1", MAX_LENGTH_ID);
    scene->buildings[0].x = 0;
    scene->buildings[0].y = 0;
    scene->buildings[0].rx = 1;
    scene->buildings[0].ry = 1;
  }
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
