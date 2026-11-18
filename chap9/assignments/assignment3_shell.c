#define _POSIX_C_SOURCE 200809L
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAXARG 64

static void check_background(void)
{
    while (waitpid(-1, NULL, WNOHANG) > 0) { }
}

/* 한 명령의 인수와 입출력 파일을 나누어 실행 */
static int run_command(char *command, int background)
{
    char *args[MAXARG + 1];
    char *infile = NULL, *outfile = NULL;
    char *token, *saveptr;
    int argc = 0, status;
    pid_t pid;

    token = strtok_r(command, " \t\r\n", &saveptr);
    while (token != NULL) {
        if (strcmp(token, "<") == 0 || strcmp(token, ">") == 0) {
            int input = (token[0] == '<');
            char *file = strtok_r(NULL, " \t\r\n", &saveptr);
            if (file == NULL) {
                fprintf(stderr, "redirection: file name missing\n");
                return 0;
            }
            if (input)
                infile = file;
            else
                outfile = file;
        } else {
            if (argc == MAXARG) {
                fprintf(stderr, "too many arguments\n");
                return 0;
            }
            args[argc++] = token;
        }
        token = strtok_r(NULL, " \t\r\n", &saveptr);
    }
    args[argc] = NULL;

    if (argc == 0)
        return 0;
    if (strcmp(args[0], "exit") == 0)
        return 1;

    pid = fork();
    if (pid < 0) {
        perror("fork");
        return 0;
    }

    if (pid == 0) {
        int fd;
        if (infile != NULL) {
            fd = open(infile, O_RDONLY);
            if (fd < 0) {
                perror(infile);
                _exit(1);
            }
            if (dup2(fd, STDIN_FILENO) < 0) {
                perror("dup2 input");
                _exit(1);
            }
            close(fd);
        }
        if (outfile != NULL) {
            fd = open(outfile, O_CREAT | O_TRUNC | O_WRONLY, 0660);
            if (fd < 0) {
                perror(outfile);
                _exit(1);
            }
            if (dup2(fd, STDOUT_FILENO) < 0) {
                perror("dup2 output");
                _exit(1);
            }
            close(fd);
        }

        execvp(args[0], args);
        perror(args[0]);
        _exit(127);
    }

    if (background) {
        printf("[background] pid=%ld\n", (long)pid);
    } else {
        while (waitpid(pid, &status, 0) < 0) {
            if (errno != EINTR) {
                perror("waitpid");
                break;
            }
        }
    }
    return 0;
}

int main(void)
{
    char *line = NULL;
    size_t size = 0;

    while (1) {
        char *start, *p;
        int stop = 0;

        check_background();
        printf("[shell] ");
        fflush(stdout);
        if (getline(&line, &size, stdin) == -1)
            break;

        start = line;
        for (p = line; ; p++) {
            if (*p == ';' || *p == '&' || *p == '\0') {
                char separator = *p;
                *p = '\0';
                if (run_command(start, separator == '&')) {
                    stop = 1;
                    break;
                }
                if (separator == '\0')
                    break;
                start = p + 1;
            }
        }

        if (stop)
            break;
    }

    free(line);
    putchar('\n');
    return 0;
}
