#include <errno.h>
#include <fcntl.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>
#define BUFSIZE 512

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일명\n", argv[0]); return 1;
    }
    int fd = open(argv[1], O_RDONLY);
    if (fd == -1) { perror(argv[1]); return 1; }
    char buffer[BUFSIZE];
    uintmax_t total = 0;
    ssize_t count;
    while ((count = read(fd, buffer, sizeof buffer)) != 0) {
        if (count == -1) {
            if (errno == EINTR) continue;
            perror("read"); close(fd); return 1;
        }
        if (UINTMAX_MAX - total < (uintmax_t)count) {
            fprintf(stderr, "파일 크기 범위 초과\n"); close(fd); return 1;
        }
        total += (uintmax_t)count;
    }
    if (close(fd) == -1) { perror("close"); return 1; }
    printf("파일명: %s, 파일 크기: %" PRIuMAX " 바이트\n", argv[1], total);
    return 0;
}
