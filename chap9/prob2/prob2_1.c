#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid, child;
    int status;

    printf("[%ld] parent process start\n", (long)getpid());
    fflush(stdout);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        printf("[%ld] child process start\n", (long)getpid());
        exit(1);
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
