#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Chapter 5, slides 32-33: last line first, first line last. */
static int load_file(const char *path, char **data, size_t *length)
{
    char block[4096];
    size_t capacity = 0;
    int fd = open(path, O_RDONLY);

    if (fd == -1) {
        perror(path);
        return -1;
    }
    for (;;) {
        ssize_t n = read(fd, block, sizeof block);
        if (n == -1) {
            if (errno == EINTR)
                continue;
            perror("read");
            close(fd);
            return -1;
        }
        if (n == 0)
            break;
        if ((size_t)n > SIZE_MAX - *length) {
            fprintf(stderr, "File is too large.\n");
            close(fd);
            return -1;
        }
        size_t needed = *length + (size_t)n;
        if (needed > capacity) {
            size_t next = capacity ? capacity : sizeof block;
            while (next < needed) {
                if (next > SIZE_MAX / 2) {
                    next = needed;
                    break;
                }
                next *= 2;
            }
            char *grown = realloc(*data, next);
            if (grown == NULL) {
                perror("realloc");
                close(fd);
                return -1;
            }
            *data = grown;
            capacity = next;
        }
        memcpy(*data + *length, block, (size_t)n);
        *length = needed;
    }
    if (close(fd) == -1) {
        perror("close");
        return -1;
    }
    return 0;
}

/* Put separators between lines and retain a final newline when present.
   A final blank output line needs a newline of its own to remain visible.
   Example: "first\nlast" becomes "last\nfirst", not "lastfirst\n". */
static int reverse_lines(const char *data, size_t length)
{
    if (length == 0)
        return 0;
    int final_newline = data[length - 1] == '\n';
    size_t end = length - (size_t)final_newline;
    for (;;) {
        size_t start = end;
        while (start > 0 && data[start - 1] != '\n')
            --start;
        size_t size = end - start;
        if (fwrite(data + start, 1, size, stdout) != size)
            return -1;
        if ((start > 0 || final_newline || size == 0) && putchar('\n') == EOF)
            return -1;
        if (start == 0)
            break;
        end = start - 1;
    }
    return 0;
}

int main(int argc, char **argv)
{
    char *data = NULL;
    size_t length = 0;
    int status = EXIT_SUCCESS;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s text-file\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (load_file(argv[1], &data, &length) == -1) {
        free(data);
        return EXIT_FAILURE;
    }
    if (reverse_lines(data, length) == -1 || fflush(stdout) == EOF) {
        perror("stdout");
        status = EXIT_FAILURE;
    }
    free(data);
    return status;
}
