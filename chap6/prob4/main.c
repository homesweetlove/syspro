#include <dirent.h>
#include <errno.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    DIR *dir;
    struct dirent *entry;
    int failed = 0;
    if (argc != 2) {
        fprintf(stderr, "Usage: %s directory\n", argv[0]);
        return 1;
    }
    dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("opendir()");
        return 1;
    }
    for (;;) {
        errno = 0;
        entry = readdir(dir);
        if (entry == NULL) {
            if (errno) { perror("readdir()"); failed = 1; }
            break;
        }
        puts(entry->d_name);
    }
    if (closedir(dir) == -1) { perror("closedir()"); failed = 1; }
    return failed;
}
