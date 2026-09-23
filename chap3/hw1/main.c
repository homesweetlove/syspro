#include <stdio.h>
#include <string.h>
#include "copy.h"

#define N 5

char lines[N][MAXLINE];

int main() {
  char line[MAXLINE];
  char temp[MAXLINE];
  int n = 0;
  int i, j;

  while (n < N && fgets(line, MAXLINE, stdin) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    copy(line, lines[n]);
    n++;
  }
  for (i = 0; i < n - 1; i++) {
    for (j = 0; j < n - 1 - i; j++) {
      if (strlen(lines[j]) < strlen(lines[j + 1])) {
        copy(lines[j], temp);
        copy(lines[j + 1], lines[j]);
        copy(temp, lines[j + 1]);
      }
    }
  }

  printf("\n");
  for (i = 0; i < n; i++)
    printf("%s\n", lines[i]);

  return 0;
}
