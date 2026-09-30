#include <stdio.h>
#include <string.h>
#define MAXLINE 80

int main(int argc, char *argv[])
{
    FILE *fp;
    int c, i, line = 0, number = 0, start = 1;
    char buffer[MAXLINE];

    if (argc > 1 && !strcmp(argv[1], "-n")) {
        number = 1;
        start = 2;
    }

    if (argc <= start) {
        fprintf(stderr, "How to use: %s [-n] File1 [File2 ...]\n", argv[0]);
        return 1;
    }

    for (i = start; i < argc; i++) {
        if ((fp = fopen(argv[i], "r")) == NULL) {
            fprintf(stderr, "File %s Open Error\n", argv[i]);
            continue;
        }

        if (number) {
            while (fgets(buffer, MAXLINE, fp) != NULL) {
                line++;
                printf("%3d %s", line, buffer);
            }
        } else {
            while ((c = fgetc(fp)) != EOF)
                fputc(c, stdout);
        }

        fclose(fp);
    }

    return 0;
}
