#include <stdio.h>
#include <string.h>
#include "copy.h"

char line[MAXLINE];
char lines[5][MAXLINE];
char temp[MAXLINE];

int main() {
  int len[5];
  int n, i, j, t;
  n = 0;

  while (n < 5 && fgets(line, MAXLINE, stdin) != NULL) {
    line[strcspn(line, "\n")] = '\0';
    len[n] = strlen(line);
    copy(line, lines[n]);
    n++;
  }

  for (i = 0; i < n - 1; i++) {
    for (j = i + 1; j < n; j++) {
      if (len[i] < len[j]) {
        copy(lines[i], temp);
        copy(lines[j], lines[i]);
        copy(temp, lines[j]);
        t = len[i];
        len[i] = len[j];
        len[j] = t;
      }
    }
  }

  printf("\n");
  for (i = 0; i < n; i++)
    printf("%s\n", lines[i]);

  return 0;
}
