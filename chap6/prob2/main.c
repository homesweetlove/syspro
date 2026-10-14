#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    char *end;
    long mode;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s octal-mode file\n", argv[0]);
        return 1;
    }
    for (const char *p = argv[1]; *p; ++p) {
        if (*p < '0' || *p > '7') {
            fprintf(stderr, "Mode must contain octal digits (0-7).\n");
            return 1;
        }
    }
    errno = 0;
    mode = strtol(argv[1], &end, 8);
    if (errno || end == argv[1] || *end || mode > 07777) {
        fprintf(stderr, "Mode must be between 0000 and 7777.\n");
        return 1;
    }
    if (chmod(argv[2], (mode_t)mode) == -1) {
        perror("chmod()");
        return 1;
    }
    return 0;
}
