#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

static int write_all(const char *text, size_t length)
{
    while (length) {
        ssize_t n = write(STDOUT_FILENO, text, length);
        if (n < 0) {
            if (errno == EINTR) continue;
            perror("write()"); return 1;
        }
        if (n == 0) { errno = EIO; perror("write()"); return 1; }
        text += n;
        length -= (size_t)n;
    }
    return 0;
}

int main(int argc, char **argv)
{
    size_t capacity = 128;
    char *buffer = NULL;
    ssize_t length;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s symbolic-link\n", argv[0]); return 1;
    }
    for (;;) {
        char *grown = realloc(buffer, capacity);
        if (grown == NULL) { perror("realloc()"); free(buffer); return 1; }
        buffer = grown;
        length = readlink(argv[1], buffer, capacity);
        if (length < 0) { perror("readlink()"); free(buffer); return 1; }
        if ((size_t)length < capacity) break;
        if (capacity > (size_t)-1 / 2) {
            fprintf(stderr, "Link target is too long.\n"); free(buffer); return 1;
        }
        capacity *= 2;
    }
    /* readlink() does not append NUL: write exactly the returned length. */
    int failed = write_all(buffer, (size_t)length);
    free(buffer);
    if (!failed) failed = write_all("\n", 1);
    return failed;
}
