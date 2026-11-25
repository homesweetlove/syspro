#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("[%ld] Parent process start\n", (long)getpid());
    fflush(stdout);

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 1;
    }
    if (pid == 0) {
        execl("/bin/echo", "echo", "hello", (char *)NULL);
        fprintf(stderr, "execl failed\n");
        _exit(127);
    }

    waitpid(pid, NULL, 0);
    printf("[%ld] Parent process end\n", (long)getpid());
    return 0;
}
