#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp;
    int c;

    if (argc < 2)
        fp = stdin;
    else {
        fp = fopen(argv[1], "r");
        if (fp == NULL) {
            fprintf(stderr, "File Open Error\n");
            return 1;
        }
    }

    while ((c = getc(fp)) != EOF)
        putc(c, stdout);

    if (fp != stdin)
        fclose(fp);

    return 0;
}
