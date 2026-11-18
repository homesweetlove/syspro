#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid1, pid2, child;
    int status;

    printf("[%ld] parent process start\n", (long)getpid());
    fflush(stdout);

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork 1");
        return 1;
    }
    if (pid1 == 0) {
        printf("[%ld] child process[1] start\n", (long)getpid());
        fflush(stdout);
        sleep(1);
        printf("[%ld] child process[1] end\n", (long)getpid());
        exit(1);
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork 2");
        waitpid(pid1, NULL, 0);
        return 1;
    }
    if (pid2 == 0) {
        printf("[%ld] child process[2] start\n", (long)getpid());
        fflush(stdout);
        sleep(2);
        printf("[%ld] child process[2] end\n", (long)getpid());
        exit(2);
    }

    for (int i = 0; i < 2; i++) {
        child = wait(&status);
        if (child < 0) {
            perror("wait");
            return 1;
        }
        printf("[%ld] child process %ld end\n", (long)getpid(), (long)child);
        if (WIFEXITED(status))
            printf("end code %d\n", WEXITSTATUS(status));
    }

    return 0;
}
