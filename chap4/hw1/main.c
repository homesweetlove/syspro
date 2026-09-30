#include <stdio.h>

int main(int argc, char *argv[])
{
    FILE *fp1, *fp2;
    int c;

    if (argc != 3) {
        fprintf(stderr, "How to use: %s File1 File2\n", argv[0]);
        return 1;
    }

    fp1 = fopen(argv[1], "a");
    if (fp1 == NULL) {
        fprintf(stderr, "File %s Open Error\n", argv[1]);
        return 2;
    }

    fp2 = fopen(argv[2], "r");
    if (fp2 == NULL) {
        fprintf(stderr, "File %s Open Error\n", argv[2]);
        fclose(fp1);
        return 3;
    }

    while ((c = fgetc(fp2)) != EOF)
        fputc(c, fp1);

    fclose(fp1);
    fclose(fp2);
    return 0;
}
