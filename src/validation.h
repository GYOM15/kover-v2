#ifndef VALIDATION_H_
#define VALIDATION_H_

#include <stdbool.h>

// Forward declaration of struct Scene
struct Scene;

// Validation error structure to store error information
struct ValidationError {
    bool has_error;
    char message[256];
};

// Validations
// -----------

/**
 * Initialize validation error structure
 * 
 * @param error  The validation error structure to initialize
 */
void init_validation_error(struct ValidationError* error);

/**
 * Validates a scene without exiting on error
 *
 * @param scene  The scene to validate
 * @param error  Structure to store error information if validation fails
 * @return       true if the scene is valid, false otherwise
 */
bool is_scene_valid(const struct Scene* scene, struct ValidationError* error);

/**
 * Gets the last validation error message
 * 
 * @param error  The validation error structure
 * @return       The error message from the validation
 */
const char* get_validation_error(const struct ValidationError* error);

/**
 * Indicates if a string is a valid identifier
 *
 * An identifier is valid if it completely matches the BRE
 *
 *   [a-zA-Z_][a-zA-Z0-1_]*
 *
 * @param s  The string to verify
 * @return   true if and only if the string is a valid identifier
 */
bool is_valid_id(const char* s);

/**
 * Indicates if a string is a valid integer
 *
 * An string is a valid integer if it completely matches the BRE
 *
 *   0|([-]?[1-9][0-9]*)
 *
 * @param s  The string to verify
 * @return   true if and only if the string is a valid identifier
 */
bool is_valid_integer(const char* s);

/**
 * Indicates if a string is a valid positive integer
 *
 * An string is a valid positive integer if it completely matches the BRE
 *
 *   [1-9][0-9]*
 *
 * @param s  The string to verify
 * @return   true if and only if the string is a valid identifier
 */
bool is_valid_positive_integer(const char* s);

// Error reporting
// ---------------

/**
 * Report an error and exit with error code
 * 
 * This function prints "not ok" to stdout, the error message to stderr,
 * and exits with status code 1.
 * 
 * @param format   The format string for the error message
 * @param ...      The arguments for the format string
 */
void report_error(const char* format, ...);

/**
 * Reports on stderr that a given object identifier is not unique
 *
 * @param object  The object
 * @param id      The identifier
 */
void report_error_non_unique_identifiers(const char* object, const char* id);

/**
 * Reports on stderr that an identifier is invalid
 *
 * @param id           The identifier
 * @param line_number  The line number
 */
void report_error_invalid_identifier(const char* id, int line_number);

/**
 * Reports on stderr that a string is not a valid integer
 *
 * @param s            The string
 * @param line_number  The line number
 */
void report_error_invalid_int(const char* s, int line_number);

/**
 * Reports on stderr that a string is not a valid positive integer
 *
 * @param s            The string
 * @param line_number  The line number
 */
void report_error_invalid_positive_int(const char* s, int line_number);

/**
 * Reports on stderr that the first line of a scene is invalid
 */
void report_error_scene_first_line(void);

/**
 * Reports on stderr that a scene line is not recognized
 *
 * @param line_number  The number of the unrecognized line
 */
void report_error_unrecognized_line(int line_number);

/**
 * Reports on stderr that a building line has the wrong number of arguments
 *
 * @param object       The object on the line
 * @param line_number  The number of the unrecognized line
 */
void report_error_line_wrong_arguments_number(const char* object, int line_number);

/**
 * Reports on stderr that the last line of a scene is invalid
 */
void report_error_scene_last_line(void);

/**
 * Reports on stderr that two buildings are overlapping
 *
 * @param id1  The identifier of the first building
 * @param id2  The identifier of the second building
 */
void report_error_overlapping_buildings(const char* id1, const char* id2);

/**
 * Reports on stderr that two antennas have the same position
 *
 * @param id1  The identifier of the first antenna
 * @param id2  The identifier of the second antenna
 */
void report_error_same_position_antennas(const char* id1, const char* id2);

/**
 * Reports on stderr that the subcommand is mandatory
 */
void report_error_mandatory_subcommand(void);

/**
 * Reports on stderr that the subcommand is unrecognized
 */
void report_error_unrecognized_subcommand(const char* subcommand);

#endif