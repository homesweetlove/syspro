#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    pid_t pid, child;
    int status;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]\n", argv[0]);
        return 1;
    }

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        execvp(argv[1], &argv[1]);
        perror("execvp");
        _exit(127);
    }

    child = wait(&status);
    if (child < 0) {
        perror("wait");
        return 1;
    }

    printf("[%ld] child process %ld end\n", (long)getpid(), (long)child);
    if (WIFEXITED(status))
        printf("end code %d\n", WEXITSTATUS(status));
    return 0;
}
