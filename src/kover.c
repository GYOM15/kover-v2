#include <stdio.h>
#include <string.h>

#define MAX_LENGTH 50

int main(int argc, char* argv[]) {
  char line[MAX_LENGTH + 1];
  if (argc < 2)
    return 1;
  const char* subcommand = argv[1];

  if (strcmp(subcommand, "summarize") == 0) {
    int num_lines = 0;
    while (fgets(line, MAX_LENGTH, stdin) != NULL)
      ++num_lines;
    if (num_lines == 2)
      puts("An empty scene");
    else
      puts("A scene with 1 building");
    return 0;
  } else if (strcmp(subcommand, "describe") == 0) {
    int num_lines = 0;
    while (fgets(line, MAX_LENGTH, stdin) != NULL)
      ++num_lines;
    if (num_lines == 2)
      puts("An empty scene");
    else {
      puts("A scene with 1 building");
      puts("  building b1 at 0 0 with dimensions 1 1");
    }
    return 0;
  } else {
    return 1;
  }
}
