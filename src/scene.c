#include "scene.h"

#include <limits.h>
#include <stdio.h>
#include <string.h>

#include "validation.h"

// Help functions
// --------------

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
  return are_intervals_overlapping(building1->x - building1->w,
                                   building1->x + building1->w,
                                   building2->x - building2->w,
                                   building2->x + building2->w) &&
         are_intervals_overlapping(building1->y - building1->h,
                                   building1->y + building1->h,
                                   building2->y - building2->h,
                                   building2->y + building2->h);
}

/**
 * Indicates if two antennas have the same position
 *
 * @param antenna1  The first antenna
 * @param antenna2  The second antenna
 */
bool have_antennas_same_position(const struct Antenna* antenna1,
                                 const struct Antenna* antenna2) {
  return antenna1->x == antenna2->x && antenna1->y == antenna2->y;
}

/**
 * Checks if the buildings of a scene are valid.
 *
 * @param scene  The scene to validate
 */
void validate_buildings(const struct Scene* scene) {
  for (unsigned int b1 = 0; b1 < scene->num_buildings; ++b1)
    for (unsigned int b2 = b1 + 1; b2 < scene->num_buildings; ++b2) {
      const struct Building* building1 = scene->buildings + b1,
                           * building2 = scene->buildings + b2;
      if (are_building_overlapping(building1, building2))
        report_error_overlapping_buildings(building1->id, building2->id);
    }
}

/**
 * Checks if the antennas of a scene are valid.
 *
 * @param scene  The scene to validate
 */
void validate_antennas(const struct Scene* scene) {
  for (unsigned int a1 = 0; a1 < scene->num_antennas; ++a1)
    for (unsigned int a2 = a1 + 1; a2 < scene->num_antennas; ++a2) {
      const struct Antenna* antenna1 = scene->antennas + a1,
                          * antenna2 = scene->antennas + a2;
      if (have_antennas_same_position(antenna1, antenna2))
        report_error_same_position_antennas(antenna1->id, antenna2->id);
    }
}

// Construction
// ------------

void initialize_empty_scene(struct Scene* scene) {
  scene->num_buildings = 0;
  scene->num_antennas = 0;
}

// Validation
// ----------

void validate_scene(const struct Scene* scene) {
  validate_buildings(scene);
  validate_antennas(scene);
}

// Accessors
// ---------

bool scene_is_empty(const struct Scene* scene) {
  return scene->num_buildings == 0 && scene->num_antennas == 0;
}

void print_scene_summary(const struct Scene* scene) {
  if (scene->num_buildings == 0 && scene->num_antennas == 0) {
    puts("An empty scene");
    return;
  }
  printf("A scene with ");
  if (scene->num_buildings > 0)
    printf("%d building%s", scene->num_buildings,
           scene->num_buildings > 1 ? "s" : "");
  if (scene->num_buildings > 0 && scene->num_antennas > 0)
    printf(" and ");
  if (scene->num_antennas > 0)
    printf("%d antenna%s", scene->num_antennas,
           scene->num_antennas > 1 ? "s" : "");
  printf("\n");
}

void print_scene_buildings(const struct Scene* scene) {
  for (unsigned int b = 0; b < scene->num_buildings; ++b) {
    const struct Building* building = scene->buildings + b;
    printf("  building %s at %d %d with dimensions %d %d\n",
           building->id, building->x, building->y, building->w, building->h);
  }
}

void print_scene_antennas(const struct Scene* scene) {
  for (unsigned int a = 0; a < scene->num_antennas; ++a) {
    const struct Antenna* antenna = scene->antennas + a;
    printf("  antenna %s at %d %d with range %d\n",
           antenna->id, antenna->x, antenna->y, antenna->r);
  }
}

void print_scene_bounding_box(const struct Scene* scene) {
  if (scene_is_empty(scene)) {
    printf("undefined (empty scene)\n");
    return;
  }
  int xmin = INT_MAX, xmax = INT_MIN,
      ymin = INT_MAX, ymax = INT_MIN;
  for (unsigned int b = 0; b < scene->num_buildings; ++b) {
    int x = scene->buildings[b].x,
        y = scene->buildings[b].y,
        w = scene->buildings[b].w,
        h = scene->buildings[b].h;
    xmin = x - w < xmin ? x - w : xmin;
    xmax = x + w > xmax ? x + w : xmax;
    ymin = y - h < ymin ? y - h : ymin;
    ymax = y + h > ymax ? y + h : ymax;
  }
  for (unsigned int a = 0; a < scene->num_antennas; ++a) {
    int x = scene->antennas[a].x,
        y = scene->antennas[a].y,
        r = scene->antennas[a].r;
    xmin = x - r < xmin ? x - r : xmin;
    xmax = x + r > xmax ? x + r : xmax;
    ymin = y - r < ymin ? y - r : ymin;
    ymax = y + r > ymax ? y + r : ymax;
  }
  printf("bounding box [%d, %d] x [%d, %d]\n", xmin, xmax, ymin, ymax);
}

// Modifiers
// ---------

void add_building(struct Scene* scene, const struct Building* building) {
  unsigned int b = 0;
  while (b < scene->num_buildings &&
         strcmp(building->id, scene->buildings[b].id) > 0)
    ++b;
  if (b < scene->num_buildings &&
      strcmp(building->id, scene->buildings[b].id) == 0)
    report_error_non_unique_identifiers("building", building->id);
  for (unsigned int b2 = scene->num_buildings; b2 > b; --b2)
    scene->buildings[b2] = scene->buildings[b2 - 1];
  struct Building* scene_building = scene->buildings + b;
  strncpy(scene_building->id, building->id, MAX_LENGTH_ID);
  scene_building->x = building->x;
  scene_building->y = building->y;
  scene_building->w = building->w;
  scene_building->h = building->h;
  ++scene->num_buildings;
}

void add_antenna(struct Scene* scene, const struct Antenna* antenna) {
  unsigned int a = 0;
  while (a < scene->num_antennas &&
         strcmp(antenna->id, scene->antennas[a].id) > 0)
    ++a;
  if (a < scene->num_antennas &&
      strcmp(antenna->id, scene->antennas[a].id) == 0)
    report_error_non_unique_identifiers("antenna", antenna->id);
  for (unsigned int a2 = scene->num_antennas; a2 > a; --a2)
    scene->antennas[a2] = scene->antennas[a2 - 1];
  struct Antenna* scene_antenna = scene->antennas + a;
  strncpy(scene_antenna->id, antenna->id, MAX_LENGTH_ID);
  scene_antenna->x = antenna->x;
  scene_antenna->y = antenna->y;
  scene_antenna->r = antenna->r;
  ++scene->num_antennas;
}
