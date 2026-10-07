#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "사용법: %s 파일명\n", argv[0]);
        return 1;
    }
    int fd = open(argv[1], O_RDWR);
    if (fd == -1) {
        fprintf(stderr, "파일 열기 오류\n");
        perror(argv[1]);
        return 1;
    }
    printf("파일명: %s, 파일 디스크립터: %d\n", argv[1], fd);
    if (close(fd) == -1) { perror("close"); return 1; }
    return 0;
}
