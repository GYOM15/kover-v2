#ifndef SCENE_H_
#define SCENE_H_

#include <stdbool.h>

// Constants
// ---------

// The maximum length of an identifier
#define MAX_LENGTH_ID 10
// The maximum number of buildings in a scene
#define NUM_MAX_BUILDINGS 100
// The maximum number of antennas in a scene
#define NUM_MAX_ANTENNAS 100
// The maximum number of houses in a scene
#define NUM_MAX_HOUSES 100

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
  // The half width of the building (half length in x direction)
  int w;
  // The half height of the building (half length in y direction)
  int h;
};

// A house (similar to a building)
struct House {
  // The identifier of the house
  char id[MAX_LENGTH_ID + 1];
  // The x coordinate of the house
  int x;
  // The y coordinate of the house
  int y;
  // The half width of the house (half length in x direction)
  int w;
  // The half height of the house (half length in y direction)
  int h;
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
  // The number of houses in the scene
  unsigned int num_houses;
  // The houses of the scene
  struct House houses[NUM_MAX_HOUSES];
  // The number of antennas in the scene
  unsigned int num_antennas;
  // The antennas of the scene
  struct Antenna antennas[NUM_MAX_ANTENNAS];
};

// Construction
// ------------

/**
 * Initializes an empty scene
 *
 * @param scene  The scene to initialize
 */
void initialize_empty_scene(struct Scene* scene);

/**
 * Loads a scene from the standard input
 *
 * @param scene  The resulting scene
 */
void load_scene_from_stdin(struct Scene* scene);

// Validation
// ----------

/**
 * Checks if a scene is valid
 *
 * If the scene is invalid, an error is printed on stdout and the program exits
 * with 1.
 *
 * @param scene  The scene to validate
 */
void validate_scene(const struct Scene* scene);

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
                             const struct Building* building2);

/**
* Indicates if two antennas have the same position
*
* @param antenna1  The first antenna
* @param antenna2  The second antenna
*/
bool have_antennas_same_position(const struct Antenna* antenna1,
                                const struct Antenna* antenna2);

// Accessors
// ---------

/**
 * Indicates if a scene is empty
 *
 * @param scene  The scene to check
 * @return       true if and only if the given scene is empty
 */
bool scene_is_empty(const struct Scene* scene);

/**
 * Prints a summary of the scene on stdout
 *
 * @param scene  The scene to print
 */
void print_scene_summary(const struct Scene* scene);

/**
 * Prints the building of the scene to stdout
 *
 * @param scene  The scene whose buildings are printed
 */
void print_scene_buildings(const struct Scene* scene);

/**
 * Prints the antenna of the scene to stdout
 *
 * @param scene  The scene whose antennas are printed
 */
void print_scene_antennas(const struct Scene* scene);

/**
 * Prints the bounding box of the scene to stdout
 *
 * @param scene  The scene whose bounding box is printed
 */
void print_scene_bounding_box(const struct Scene* scene);

// Modifiers
// ---------

/**
 * Adds a building to a scene
 *
 * @param scene     The scene to which the building is added
 * @param building  The building to add
 */
void add_building(struct Scene* scene, const struct Building* building);

/**
 * Adds an antenna to a scene
 *
 * @param scene    The scene to which the antenna is added
 * @param antenna  The antenna to add
 */
void add_antenna(struct Scene* scene, const struct Antenna* antenna);

#endif