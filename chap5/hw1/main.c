#define _POSIX_C_SOURCE 200809L
#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Chapter 5, slides 28-31: select stored lines by number, list, range or *. */
struct line {
    size_t start;
    size_t length;
};

/* Read in blocks, growing the buffer so lines have no fixed length limit. */
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

/* A final newline ends its line; it does not create an extra empty line. */
static struct line *index_lines(const char *data, size_t length, size_t *count)
{
    size_t start = 0, index = 0;
    for (size_t i = 0; i < length; ++i)
        if (data[i] == '\n')
            ++*count;
    if (length > 0 && data[length - 1] != '\n')
        ++*count;
    if (*count == 0)
        return NULL;
    if (*count > SIZE_MAX / sizeof(struct line)) {
        errno = ENOMEM;
        return NULL;
    }
    struct line *lines = malloc(*count * sizeof *lines);
    if (lines == NULL)
        return NULL;
    for (size_t i = 0; i < length; ++i) {
        if (data[i] == '\n') {
            lines[index++] = (struct line){start, i - start};
            start = i + 1;
        }
    }
    if (start < length)
        lines[index] = (struct line){start, length - start};
    return lines;
}

static void skip_space(const char **p)
{
    while (isspace((unsigned char)**p))
        ++*p;
}

/* Parse decimal digits explicitly, rejecting signs and integer overflow. */
static int number(const char **p, size_t *value)
{
    *value = 0;
    if (**p < '0' || **p > '9')
        return -1;
    while (**p >= '0' && **p <= '9') {
        size_t digit = (size_t)(**p - '0');
        if (*value > (SIZE_MAX - digit) / 10)
            return -1;
        *value = *value * 10 + digit;
        ++*p;
    }
    return *value == 0 ? -1 : 0;
}

static int print_line(const char *data, const struct line *lines, size_t n)
{
    const struct line *line = &lines[n - 1];
    if (printf("%zu: ", n) < 0 ||
        fwrite(data + line->start, 1, line->length, stdout) != line->length ||
        putchar('\n') == EOF)
        return -1;
    return 0;
}

/* Validate first, then call again to print: bad lists never print partially. */
static const char *select_lines(const char *p, const char *data,
                               const struct line *lines, size_t count,
                               int print)
{
    skip_space(&p);
    if (*p == '*') {
        ++p;
        skip_space(&p);
        if (*p != '\0')
            return "'*' must be used alone.";
        if (print)
            for (size_t i = 0; i < count; ++i)
                if (print_line(data, lines, i + 1) == -1)
                    return "Output error.";
        return NULL;
    }
    for (;;) {
        size_t first, last;
        if (number(&p, &first) == -1)
            return "Expected a positive line number.";
        skip_space(&p);
        last = first;
        if (*p == '-') {
            ++p;
            skip_space(&p);
            if (number(&p, &last) == -1)
                return "Expected a positive range endpoint.";
            skip_space(&p);
        }
        if (first > last)
            return "Range start must not exceed its end.";
        if (last > count)
            return "Line number is out of range.";
        if (*p != ',' && *p != '\0')
            return "Expected a comma or the end of the selection.";
        if (print) {
            size_t i = first;
            for (;;) {
                if (print_line(data, lines, i) == -1)
                    return "Output error.";
                if (i == last)
                    break;
                ++i;
            }
        }
        if (*p == '\0')
            return NULL;
        ++p;
        skip_space(&p);
    }
}

int main(int argc, char **argv)
{
    char *data = NULL, *input = NULL;
    size_t length = 0, count = 0, input_capacity = 0;
    int status = EXIT_SUCCESS;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s text-file\n", argv[0]);
        return EXIT_FAILURE;
    }
    if (load_file(argv[1], &data, &length) == -1) {
        free(data);
        return EXIT_FAILURE;
    }
    struct line *lines = index_lines(data, length, &count);
    if (count > 0 && lines == NULL) {
        perror("malloc");
        free(data);
        return EXIT_FAILURE;
    }
    printf("Loaded %zu lines. Select n, n,m, n-m, or *; EOF to quit.\n", count);
    for (;;) {
        if (fputs("Input: ", stdout) == EOF || fflush(stdout) == EOF) {
            perror("stdout");
            status = EXIT_FAILURE;
            break;
        }
        errno = 0;
        ssize_t n = getline(&input, &input_capacity, stdin);
        if (n == -1) {
            if (errno == EINTR) {
                clearerr(stdin);
                continue;
            }
            if (!feof(stdin)) {
                perror("stdin");
                status = EXIT_FAILURE;
            }
            break;
        }
        const char *error = memchr(input, '\0', (size_t)n) != NULL
            ? "NUL bytes are not allowed in a selection."
            : select_lines(input, data, lines, count, 0);
        if (error != NULL) {
            fprintf(stderr, "Invalid selection: %s\n", error);
            continue;
        }
        error = select_lines(input, data, lines, count, 1);
        if (error != NULL) {
            fprintf(stderr, "%s\n", error);
            status = EXIT_FAILURE;
            break;
        }
    }
    if (fflush(stdout) == EOF) {
        perror("stdout");
        status = EXIT_FAILURE;
    }
    free(input);
    free(lines);
    free(data);
    return status;
}
