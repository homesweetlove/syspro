#ifndef STUDENT_H
#define STUDENT_H
#include <errno.h>
#include <ctype.h>
#include <limits.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 24
#define START 1401001
struct student { char name[MAX]; int id; int score; };

/* Consume a whole token, including excess characters. 0 means too long. */
static inline int scan_token(char *buffer, size_t capacity)
{
    char ch;
    size_t length = 0;
    int oversized = 0;
    if (scanf(" %c", &ch) != 1) return EOF;
    do {
        if (length + 1 < capacity) buffer[length++] = ch;
        else oversized = 1;
        if (scanf("%c", &ch) != 1) break;
    } while (!isspace((unsigned char)ch));
    buffer[length] = '\0';
    return oversized ? 0 : 1;
}

static inline int parse_int(const char *text, int *value)
{
    char *end;
    errno = 0;
    long n = strtol(text, &end, 10);
    if (errno || end == text || *end || n < INT_MIN || n > INT_MAX) return -1;
    *value = (int)n; return 0;
}

static inline int record_offset(int id, off_t *offset)
{
    if (id < START) return -1;
    uintmax_t position = (uintmax_t)(id - START) * sizeof(struct student);
    *offset = (off_t)position;
    if (*offset < 0 || (uintmax_t)*offset != position) return -1;
    return 0;
}

/* read() can return only part of a record or be interrupted by a signal. */
static inline ssize_t read_record(int fd, struct student *s)
{
    size_t done = 0;
    while (done < sizeof *s) {
        ssize_t n = read(fd, (char *)s + done, sizeof *s - done);
        if (n == -1 && errno == EINTR) continue;
        if (n == -1) return -1;
        if (n == 0) break;
        done += (size_t)n;
    }
    return (ssize_t)done;
}

static inline int write_record(int fd, const struct student *s)
{
    size_t done = 0;
    while (done < sizeof *s) {
        ssize_t n = write(fd, (const char *)s + done, sizeof *s - done);
        if (n == -1 && errno == EINTR) continue;
        if (n <= 0) { if (n == 0) errno = EIO; return -1; }
        done += (size_t)n;
    }
    return 0;
}
#endif
