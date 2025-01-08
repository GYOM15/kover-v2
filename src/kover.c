#include <stdio.h>

#define MAX_LENGTH 50

int main(void) {
  char line[MAX_LENGTH + 1];
  int num_lines = 0;
  while (fgets(line, MAX_LENGTH, stdin) != NULL)
    ++num_lines;
  if (num_lines == 2)
    puts("An empty scene");
  else
    puts("A scene with 1 building");
  return 0;
}
