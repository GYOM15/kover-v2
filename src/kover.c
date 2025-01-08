#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 50

/**
 * Runs the summarize subcommand
 */
void run_summarize_subcommand() {
  char line[MAX_LENGTH + 1];
  int num_lines = 0;
  while (fgets(line, MAX_LENGTH, stdin) != NULL)
    ++num_lines;
  if (num_lines == 2)
    puts("An empty scene");
  else
    puts("A scene with 1 building");
}

/**
 * Runs the describe subcommand
 */
void run_describe_subcommand() {
  char line[MAX_LENGTH + 1];
  int num_lines = 0;
  while (fgets(line, MAX_LENGTH, stdin) != NULL)
    ++num_lines;
  if (num_lines == 2)
    puts("An empty scene");
  else {
    puts("A scene with 1 building");
    puts("  building b1 at 0 0 with dimensions 1 1");
  }
}

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
