#ifndef SCENE_H_
#define SCENE_H_

#include <stdbool.h>
#include "validation.h"

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
 * Checks if there are any overlapping structures in the scene
 *
 * @param scene  The scene to validate
 * @param error  Structure to store error information
 * @return       true if an overlap is found, false otherwise
 */
bool validate_structures_overlaps(const struct Scene* scene, struct ValidationError* error);

/**
 * Checks if there are antennas with the same position
 *
 * @param scene  The scene to validate
 * @param error  Structure to store error information
 * @return       true if same position antennas are found, false otherwise
 */
bool validate_antennas(const struct Scene* scene, struct ValidationError* error);

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
 * Indicates if two houses are overlapping
 *
 * @param house1  The first house
 * @param house2  The second house
 * @return        true if and only if they overlap
 */
bool are_houses_overlapping(const struct House* house1,
  const struct House* house2);

/**
 * Indicates if a building and a house are overlapping
 *
 * @param building  The building
 * @param house     The house
 * @return          true if and only if they overlap
 */
bool are_building_house_overlapping(const struct Building* building,
  const struct House* house);

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
 * Prints the houses of the scene to stdout
 *
 * @param scene  The scene whose houses are printed
 */
void print_scene_houses(const struct Scene* scene);

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
 * Adds a house to a scene
 *
 * @param scene  The scene to which the house is added
 * @param house  The house to add
 */
void add_house(struct Scene* scene, const struct House* house);

/**
 * Adds an antenna to a scene
 *
 * @param scene    The scene to which the antenna is added
 * @param antenna  The antenna to add
 */
void add_antenna(struct Scene* scene, const struct Antenna* antenna);

// Quality
// -------

/**
 * Checks if a point is covered by any antenna
 *
 * @param x      The x-coordinate of the point
 * @param y      The y-coordinate of the point
 * @param scene  The scene with antennas
 * @return       true if the point is covered, false otherwise
 */
bool is_point_covered(int x, int y, const struct Scene* scene);

/**
 * Counts covered corners of a building
 *
 * @param building  The building
 * @param scene     The scene with antennas
 * @return          The number of corners covered (0-4)
 */
int count_building_covered_corners(const struct Building* building, const struct Scene* scene);

/**
 * Counts covered corners of a house
 *
 * @param house  The house
 * @param scene  The scene with antennas
 * @return       The number of corners covered (0-4)
 */
int count_house_covered_corners(const struct House* house, const struct Scene* scene);

/**
 * Gets the quality grade based on covered corners
 *
 * @param covered_corners  The number of covered corners (0-4)
 * @return                 The quality grade ('A' to 'E')
 */
char get_coverage_quality(int covered_corners);

#endif