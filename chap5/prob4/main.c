#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

static int write_text(int fd, const char *text)
{
    size_t left = strlen(text);
    while (left > 0) {
        ssize_t n = write(fd, text, left);
        if (n == -1 && errno == EINTR) continue;
        if (n <= 0) { if (n == 0) errno = EIO; return -1; }
        text += n; left -= (size_t)n;
    }
    return 0;
}

int main(void)
{
    int fd = creat("myfile", 0600);
    if (fd == -1) { perror("myfile"); return 1; }
    if (write_text(fd, "Hello! Linux\n") == -1) {
        perror("write"); close(fd); return 1;
    }
    int copy = dup(fd);
    if (copy == -1) { perror("dup"); close(fd); return 1; }
    if (write_text(copy, "Bye! Linux\n") == -1) {
        perror("write"); close(fd); close(copy); return 1;
    }
    printf("원본 디스크립터: %d, 복제 디스크립터: %d\n", fd, copy);
    int failed = 0;
    if (close(fd) == -1) { perror("close"); failed = 1; }
    if (close(copy) == -1) { perror("close"); failed = 1; }
    return failed;
}
