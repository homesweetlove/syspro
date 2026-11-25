#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARG 64
#define MAXLINE 1024

int main(void)
{
    char line[MAXLINE];

    while (1) {
        char *args[MAXARG + 1];
        char *token, *saveptr;
        int argc = 0, background = 0, status;
        pid_t pid;

        while (waitpid(-1, NULL, WNOHANG) > 0) { }

        printf("[Shell] ");
        fflush(stdout);
        if (fgets(line, sizeof(line), stdin) == NULL)
            break;

        token = strtok_r(line, " \t\r\n", &saveptr);
        while (token != NULL && argc < MAXARG) {
            args[argc++] = token;
            token = strtok_r(NULL, " \t\r\n", &saveptr);
        }
        if (token != NULL) {
            fprintf(stderr, "too many arguments\n");
            continue;
        }
        if (argc == 0)
            continue;
        if (strcmp(args[0], "exit") == 0)
            break;

        if (strcmp(args[argc - 1], "&") == 0) {
            background = 1;
            argc--;
        }
        if (argc == 0)
            continue;
        args[argc] = NULL;

        pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }
        if (pid == 0) {
            execvp(args[0], args);
            perror(args[0]);
            _exit(127);
        }

        printf("[%ld] child process start\n", (long)pid);
        if (!background) {
            while (waitpid(pid, &status, 0) < 0) {
                if (errno != EINTR) {
                    perror("waitpid");
                    break;
                }
            }
            printf("[%ld] child process end\n", (long)pid);
        }
    }

    putchar('\n');
    return 0;
}
