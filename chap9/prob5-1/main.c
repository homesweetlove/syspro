#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int fd;

    if (argc != 2) {
        fprintf(stderr, "Usage: %s output_file\n", argv[0]);
        return 1;
    }

    fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0660);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    if (dup2(fd, STDOUT_FILENO) < 0) {
        perror("dup2");
        close(fd);
        return 1;
    }
    close(fd);

    printf("Hello stdout !\n");
    fprintf(stderr, "Hello stderr !\n");
    return 0;
}
