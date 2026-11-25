#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("PARENT: PID = %ld  GID = %ld\n",
           (long)getpid(), (long)getpgrp());
    fflush(stdout);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        printf("CHILD:  PID = %ld  GID = %ld\n",
               (long)getpid(), (long)getpgrp());
        return 0;
    }

    waitpid(pid, NULL, 0);
    return 0;
}
