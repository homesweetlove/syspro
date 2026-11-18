#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid, child;
    int status;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s output_file command [args...]\n", argv[0]);
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        int fd = open(argv[1], O_CREAT | O_TRUNC | O_WRONLY, 0660);

        if (fd < 0) {
            perror("open");
            _exit(1);
        }
        if (dup2(fd, STDOUT_FILENO) < 0) {
            perror("dup2");
            close(fd);
            _exit(1);
        }
        close(fd);

        execvp(argv[2], &argv[2]);
        perror("execvp");
        _exit(127);
    }

    child = waitpid(pid, &status, 0);
    if (child < 0) {
        perror("waitpid");
        return 1;
    }

    printf("[%ld] child process %ld end\n", (long)getpid(), (long)child);
    if (WIFEXITED(status))
        printf("end code %d\n", WEXITSTATUS(status));

    return 0;
}
