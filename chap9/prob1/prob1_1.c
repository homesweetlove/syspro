#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("[%ld] Process start\n", (long)getpid());
    fflush(stdout);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }

    printf("[%ld] Process : return value %ld\n",
           (long)getpid(), (long)pid);
    return 0;
}
