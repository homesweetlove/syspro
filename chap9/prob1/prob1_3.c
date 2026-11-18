#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid1, pid2;

    printf("[PARENT] : Hello, world! pid=%ld\n", (long)getpid());
    fflush(stdout);

    pid1 = fork();
    if (pid1 < 0) {
        perror("fork 1");
        return 1;
    }
    if (pid1 == 0) {
        printf("[Child 1] : Hello, world! pid=%ld\n", (long)getpid());
        return 0;
    }

    pid2 = fork();
    if (pid2 < 0) {
        perror("fork 2");
        waitpid(pid1, NULL, 0);
        return 1;
    }
    if (pid2 == 0) {
        printf("[Child 2] : Hello, world! pid=%ld\n", (long)getpid());
        return 0;
    }

    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
    return 0;
}
