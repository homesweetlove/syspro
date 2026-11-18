#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(void)
{
    pid_t pid;

    printf("[%ld] Parent process start\n", (long)getpid());
    fflush(stdout);

    for (int i = 0; i < 3; i++) {
        pid = fork();
        if (pid < 0) {
            perror("fork");
            while (wait(NULL) > 0) { }
            return 1;
        }
        if (pid == 0) {
            if (i == 0)
                execl("/bin/echo", "echo", "hello", (char *)NULL);
            else if (i == 1)
                execl("/bin/date", "date", (char *)NULL);
            else
                execl("/bin/ls", "ls", "-l", (char *)NULL);

            fprintf(stderr, "execl failed\n");
            _exit(127);
        }
    }

    for (int i = 0; i < 3; i++)
        wait(NULL);

    printf("[%ld] Parent process end\n", (long)getpid());
    return 0;
}
