#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    struct stat st;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s existing-target symbolic-link\n", argv[0]);
        return 1;
    }
    /* A relative target is resolved from the new link's parent directory. */
    const char *slash = strrchr(argv[2], '/');
    char *check = NULL;
    const char *target = argv[1];
    if (argv[1][0] != '/' && slash != NULL) {
        size_t parent = (size_t)(slash - argv[2]) + 1;
        size_t n = strlen(argv[1]);
        if (parent > (size_t)-1 - n - 1) {
            fprintf(stderr, "Path is too long.\n"); return 1;
        }
        check = malloc(parent + n + 1);
        if (check == NULL) { perror("malloc()"); return 1; }
        memcpy(check, argv[2], parent);
        memcpy(check + parent, argv[1], n + 1);
        target = check;
    }
    if (stat(target, &st) == -1) {
        int error = errno;
        free(check); errno = error; perror("stat()"); return 1;
    }
    free(check);
    if (symlink(argv[1], argv[2]) == -1) {
        perror("symlink()"); return 1;
    }
    return 0;
}
