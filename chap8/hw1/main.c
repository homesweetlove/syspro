#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

extern char **environ;

static void print_environment(const char *name)
{
    if (name == NULL) {
        for (int i = 0; environ[i] != NULL; i++)
            printf("%s\n", environ[i]);
    } else {
        const char *value = getenv(name);
        if (value != NULL)
            printf("%s\n", value);
        else
            printf("Environment variable not found: %s\n", name);
    }
}

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s [-e [NAME]] [-u] [-g] [-i] [-p]\n", argv[0]);
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-e") == 0) {
            const char *name = NULL;
            if (i + 1 < argc && argv[i + 1][0] != '-')
                name = argv[++i];
            print_environment(name);
        }
        else if (strncmp(argv[i], "-e", 2) == 0) {
            print_environment(argv[i] + 2);
        }
        else if (strcmp(argv[i], "-u") == 0) {
            printf("Real User ID: %ld\n", (long)getuid());
            printf("Effective User ID: %ld\n", (long)geteuid());
        }
        else if (strcmp(argv[i], "-g") == 0) {
            printf("Real Group ID: %ld\n", (long)getgid());
            printf("Effective Group ID: %ld\n", (long)getegid());
        }
        else if (strcmp(argv[i], "-i") == 0) {
            printf("Process ID: %ld\n", (long)getpid());
        }
        else if (strcmp(argv[i], "-p") == 0) {
            printf("Parent Process ID: %ld\n", (long)getppid());
        }
        else {
            fprintf(stderr, "Unknown option: %s\n", argv[i]);
            return 1;
        }
    }
    return 0;
}
