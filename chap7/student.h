#ifndef STUDENT_H
#define STUDENT_H

#include <errno.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#define MAX 24
#define START 1401001

struct student {
    char name[MAX];
    int id;
    int score;
};

static inline int record_offset(int id, off_t *offset)
{
    if (id < START)
        return -1;

    uintmax_t position =
        (uintmax_t)(id - START) * sizeof(struct student);

    *offset = (off_t)position;

    if (*offset < 0 || (uintmax_t)*offset != position)
        return -1;

    return 0;
}

static inline ssize_t read_record(int fd, struct student *s)
{
    size_t done = 0;

    while (done < sizeof(*s)) {
        ssize_t n = read(fd, (char *)s + done,
                         sizeof(*s) - done);

        if (n == -1 && errno == EINTR)
            continue;

        if (n == -1)
            return -1;

        if (n == 0)
            break;

        done += (size_t)n;
    }

    return (ssize_t)done;
}

static inline int write_record(int fd, const struct student *s)
{
    size_t done = 0;

    while (done < sizeof(*s)) {
        ssize_t n = write(fd, (const char *)s + done,
                          sizeof(*s) - done);

        if (n == -1 && errno == EINTR)
            continue;

        if (n <= 0)
            return -1;

        done += (size_t)n;
    }

    return 0;
}

#endif
