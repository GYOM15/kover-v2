#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 50

// Scenes
// ------

// A scene
struct Scene {
  // The number of building in the scene
  unsigned int num_buildings;
};

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
  scene->num_buildings = num_lines == 2 ? 0 : 1;
}

// Subcommands processing
// ----------------------

/**
 * Runs the summarize subcommand
 */
void run_summarize_subcommand() {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  if (scene.num_buildings == 0)
    puts("An empty scene");
  else
    printf("A scene with %d building%s\n", scene.num_buildings,
           scene.num_buildings > 1 ? "s" : "");
}

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand() {
  struct Scene scene;
  load_scene_from_stdin(&scene);
  if (scene.num_buildings == 0)
    puts("An empty scene");
  else {
    printf("A scene with %d building%s\n", scene.num_buildings,
           scene.num_buildings > 1 ? "s" : "");
    puts("  building b1 at 0 0 with dimensions 1 1");
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
