#include "scene.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "validation.h"

// Private constants, types and functions
// ======================================

// Constants
// ---------

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

// Validation
// ----------

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
 * Vérifie si un bâtiment et une maison se chevauchent
 */
bool are_building_house_overlapping(const struct Building *building,
                                    const struct House *house)
{
  return are_intervals_overlapping(building->x - building->w,
                                   building->x + building->w,
                                   house->x - house->w,
                                   house->x + house->w) &&
         are_intervals_overlapping(building->y - building->h,
                                   building->y + building->h,
                                   house->y - house->h,
                                   house->y + house->h);
}

/**
 * Vérifie si deux maisons se chevauchent
 */
bool are_houses_overlapping(const struct House *house1,
                            const struct House *house2)
{
  return are_intervals_overlapping(house1->x - house1->w,
                                   house1->x + house1->w,
                                   house2->x - house2->w,
                                   house2->x + house2->w) &&
         are_intervals_overlapping(house1->y - house1->h,
                                   house1->y + house1->h,
                                   house2->y - house2->h,
                                   house2->y + house2->h);
}

/**
 * Checks if there are overlapping buildings in the scene
 *
 * @param scene  The scene to validate
 * @return       true if an overlap is found, false otherwise
 */
bool validate_buildings(const struct Scene* scene, struct ValidationError* error) {
  for (unsigned int b1 = 0; b1 < scene->num_buildings; ++b1) {
    for (unsigned int b2 = b1 + 1; b2 < scene->num_buildings; ++b2) {
      const struct Building* building1 = scene->buildings + b1;
      const struct Building* building2 = scene->buildings + b2;
      if (are_building_overlapping(building1, building2)) {
        snprintf(error->message, sizeof(error->message), 
                "buildings %s and %s are overlapping", 
                building1->id, building2->id);
        error->has_error = true;
        return true;
      }
    }
  }
  return false;
}

/**
 * Checks if there are overlapping houses in the scene
 *
 * @param scene  The scene to validate
 * @return       true if an overlap is found, false otherwise
 */
bool validate_houses(const struct Scene* scene, struct ValidationError* error) {
  for (unsigned int h1 = 0; h1 < scene->num_houses; ++h1) {
    for (unsigned int h2 = h1 + 1; h2 < scene->num_houses; ++h2) {
      const struct House* house1 = scene->houses + h1;
      const struct House* house2 = scene->houses + h2;
      if (are_houses_overlapping(house1, house2)) {
        snprintf(error->message, sizeof(error->message), 
                "houses %s and %s are overlapping", 
                house1->id, house2->id);
        error->has_error = true;
        return true;
      }
    }
  }
  return false;
}

/**
 * Checks if there are overlapping buildings and houses in the scene
 *
 * @param scene  The scene to validate
 * @return       true if an overlap is found, false otherwise
 */
bool validate_buildings_houses_overlap(const struct Scene* scene, struct ValidationError* error) {
  for (unsigned int b = 0; b < scene->num_buildings; ++b) {
    for (unsigned int h = 0; h < scene->num_houses; ++h) {
      const struct Building* building = scene->buildings + b;
      const struct House* house = scene->houses + h;
      if (are_building_house_overlapping(building, house)) {
        snprintf(error->message, sizeof(error->message), 
                "buildings %s and %s are overlapping", 
                building->id, house->id);
        error->has_error = true;
        return true;
      }
    }
  }
  return false;
}

/**
 * Checks for any overlapping structures in the scene
 *
 * @param scene  The scene to validate
 * @return       true if an overlap is found, false otherwise
 */
bool validate_structures_overlaps(const struct Scene* scene, struct ValidationError* error) {
  return validate_buildings(scene, error) ||
         validate_houses(scene, error) ||
         validate_buildings_houses_overlap(scene, error);
}

/**
 * Checks if the antennas of a scene are valid.
 *
 * @param scene  The scene to validate
 */
bool validate_antennas(const struct Scene* scene, struct ValidationError* error) {
  for (unsigned int a1 = 0; a1 < scene->num_antennas; ++a1) {
    for (unsigned int a2 = a1 + 1; a2 < scene->num_antennas; ++a2) {
      const struct Antenna* antenna1 = scene->antennas + a1;
      const struct Antenna* antenna2 = scene->antennas + a2;
      
      if (have_antennas_same_position(antenna1, antenna2)) {
        snprintf(error->message, sizeof(error->message), 
                "antennas %s and %s have the same position", 
                antenna1->id, antenna2->id);
        error->has_error = true;
        return true;
      }
    }
  }
  return false;
}

// Loading
// -------

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
 * Loads a house from a parsed line
 *
 * @param parsed_line  The parsed line
 * @param scene        The scene in which the house is loaded
 * @return             true if and only if the loading was successful
 */
bool load_house_from_parsed_line(const struct ParsedLine *parsed_line,
                                 struct Scene *scene)
{
  if (strcmp(parsed_line->tokens[0], "house") != 0)
    return false;
  if (parsed_line->num_tokens != 6)
    report_error_line_wrong_arguments_number("house",
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
  struct House house;
  strncpy(house.id, parsed_line->tokens[1], MAX_LENGTH_ID);
  house.x = atoi(parsed_line->tokens[2]);
  house.y = atoi(parsed_line->tokens[3]);
  house.w = atoi(parsed_line->tokens[4]);
  house.h = atoi(parsed_line->tokens[5]);
  add_house(scene, &house);
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

// Public functions definition
// ===========================

// Construction
// ------------

void initialize_empty_scene(struct Scene* scene) {
  scene->num_buildings = 0;
  scene->num_houses = 0;
  scene->num_antennas = 0;
}

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
        report_error("line has no token\n");
      }
      if (!load_building_from_parsed_line(&parsed_line, scene) &&
          !load_house_from_parsed_line(&parsed_line, scene) &&
          !load_antenna_from_parsed_line(&parsed_line, scene)){
            report_error_unrecognized_line(line_number);
          }  
    }
    ++line_number;
  }
  if (!last_line)
    report_error_scene_last_line();
}

// Validation
// ----------

void validate_scene(const struct Scene* scene) {
  struct ValidationError error;
  init_validation_error(&error);
  
  if (!is_scene_valid(scene, &error)) {
    printf("not ok\n");
    fprintf(stderr, "error: %s\n", error.message);
    exit(1);
  }
}

// Accessors
// ---------

bool scene_is_empty(const struct Scene* scene) {
  return scene->num_buildings == 0 && 
         scene->num_houses == 0 && 
         scene->num_antennas == 0;
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