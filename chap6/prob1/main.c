#define _DEFAULT_SOURCE
#include <stdio.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    int failed = 0;
    if (argc < 2) {
        fprintf(stderr, "Usage: %s file...\n", argv[0]);
        return 1;
    }
    for (int i = 1; i < argc; ++i) {
        struct stat st;
        if (lstat(argv[i], &st) == -1) {
            fprintf(stderr, "%s: ", argv[i]);
            perror("lstat()");
            failed = 1;
            continue;
        }
        const char *type = "unknown";
        if (S_ISREG(st.st_mode)) type = "regular file";
        else if (S_ISDIR(st.st_mode)) type = "directory";
        else if (S_ISCHR(st.st_mode)) type = "character device";
        else if (S_ISBLK(st.st_mode)) type = "block device";
        else if (S_ISFIFO(st.st_mode)) type = "FIFO";
        else if (S_ISLNK(st.st_mode)) type = "symbolic link";
        else if (S_ISSOCK(st.st_mode)) type = "socket";
        printf("%s: %s\n", argv[i], type);
    }
    return failed;
}
