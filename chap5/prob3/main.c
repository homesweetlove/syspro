#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#define BUFSIZE 512

int main(int argc, char *argv[])
{
    if (argc != 3) {
        fprintf(stderr, "사용법: %s 원본파일 새파일\n", argv[0]); return 1;
    }
    int source = open(argv[1], O_RDONLY);
    if (source == -1) { perror(argv[1]); return 1; }
    struct stat a, b;
    if (fstat(source, &a) == -1) { perror("fstat source"); close(source); return 1; }
    if (!S_ISREG(a.st_mode)) {
        fprintf(stderr, "원본은 일반 파일이어야 합니다.\n"); close(source); return 1;
    }
    /* Truncate only after checking that both paths are different files. */
    int target = open(argv[2], O_WRONLY | O_CREAT, 0600);
    if (target == -1) { perror(argv[2]); close(source); return 1; }
    if (fstat(target, &b) == -1) {
        perror("fstat"); close(source); close(target); return 1;
    }
    if (a.st_dev == b.st_dev && a.st_ino == b.st_ino) {
        fprintf(stderr, "원본과 대상이 같은 파일입니다.\n");
        close(source); close(target); return 1;
    }
    /* Same truncation as O_TRUNC, using the already checked descriptor. */
    if (ftruncate(target, 0) == -1) {
        perror("ftruncate"); close(source); close(target); return 1;
    }
    char buffer[BUFSIZE];
    ssize_t count;
    while ((count = read(source, buffer, sizeof buffer)) != 0) {
        if (count == -1) {
            if (errno == EINTR) continue;
            perror("read"); close(source); close(target); return 1;
        }
        ssize_t done = 0;
        while (done < count) {
            ssize_t written = write(target, buffer + done, (size_t)(count - done));
            if (written == -1 && errno == EINTR) continue;
            if (written <= 0) {
                if (written == 0) errno = EIO;
                perror("write"); close(source); close(target); return 1;
            }
            done += written;
        }
    }
    int failed = 0;
    if (close(source) == -1) { perror("close source"); failed = 1; }
    if (close(target) == -1) { perror("close target"); failed = 1; }
    if (!failed) printf("복사 완료: %s -> %s\n", argv[1], argv[2]);
    return failed;
}
