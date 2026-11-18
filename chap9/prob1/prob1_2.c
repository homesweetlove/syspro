#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    }

    if (pid == 0) {
        printf("[Child] Hello, world! Pid=%ld\n", (long)getpid());
    } else {
        printf("[Parent] Hello, world! Pid=%ld\n", (long)getpid());
        waitpid(pid, NULL, 0);
    }

    return 0;
}
