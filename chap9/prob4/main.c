#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/* system()의 동작을 fork(), exec(), waitpid()로 구현 */
int mysystem(const char *command)
{
    pid_t pid;
    int status;

    if (command == NULL)
        return 1;

    pid = fork();
    if (pid < 0)
        return -1;

    if (pid == 0) {
        execl("/bin/sh", "sh", "-c", command, (char *)NULL);
        _exit(127);
    }

    while (waitpid(pid, &status, 0) < 0) {
        if (errno != EINTR)
            return -1;
    }

    return status;
}

int main(void)
{
    const char *commands[] = {"date", "hello", "who; exit 44"};

    for (int i = 0; i < 3; i++) {
        int status = mysystem(commands[i]);

        if (status == -1) {
            perror("mysystem");
            return 1;
        }

        if (WIFEXITED(status))
            printf("end code %d\n", WEXITSTATUS(status));
        else
            printf("command did not exit normally\n");
    }

    return 0;
}
