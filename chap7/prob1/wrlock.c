#define _FILE_OFFSET_BITS 64

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include "../student.h"

int main(int argc, char *argv[])
{
    int fd, id, score, result = 0;
    off_t offset;
    struct student record;
    struct flock lock = {0};

    if (argc != 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_RDWR);

    if (fd == -1) {
        perror(argv[1]);
        return 1;
    }

    printf("Enter StudentID you want to modify : ");
    fflush(stdout);

    if (scanf("%d", &id) != 1 ||
        record_offset(id, &offset) == -1) {
        fprintf(stderr, "Invalid StudentID\n");
        close(fd);
        return 1;
    }

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = offset;
    lock.l_len = sizeof(struct student);

    if (fcntl(fd, F_SETLKW, &lock) == -1) {
        perror(argv[1]);
        close(fd);
        return 1;
    }

    if (lseek(fd, offset, SEEK_SET) == (off_t)-1 ||
        read_record(fd, &record) != sizeof(record)) {
        fprintf(stderr, "Failed to read record\n");
        result = 1;
    } else if (record.id != id) {
        printf("Student not found\n");
        result = 1;
    } else {
        printf("Name:%s  StuID:%d  Score:%d\n",
               record.name, record.id, record.score);

        printf("Enter new score: ");
        fflush(stdout);

        if (scanf("%d", &score) != 1) {
            fprintf(stderr, "Invalid score\n");
            result = 1;
        } else {
            record.score = score;

            if (lseek(fd, offset, SEEK_SET) == (off_t)-1 ||
                write_record(fd, &record) == -1) {
                perror(argv[1]);
                result = 1;
            } else {
                printf("Updated: %d %s %d\n",
                       record.id, record.name, record.score);
            }
        }
    }

    lock.l_type = F_UNLCK;

    if (fcntl(fd, F_SETLK, &lock) == -1) {
        perror(argv[1]);
        result = 1;
    }

    if (close(fd) == -1) {
        perror(argv[1]);
        result = 1;
    }

    return result;
}
