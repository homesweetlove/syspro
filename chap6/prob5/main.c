#define _DEFAULT_SOURCE
#include <dirent.h>
#include <errno.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

static void permissions(mode_t mode, char text[11])
{
    const mode_t bits[] = {S_IRUSR,S_IWUSR,S_IXUSR,S_IRGRP,S_IWGRP,
                          S_IXGRP,S_IROTH,S_IWOTH,S_IXOTH};
    const char letters[] = "rwxrwxrwx";
    text[0] = S_ISDIR(mode) ? 'd' : S_ISLNK(mode) ? 'l' :
              S_ISCHR(mode) ? 'c' : S_ISBLK(mode) ? 'b' :
              S_ISFIFO(mode) ? 'p' : S_ISSOCK(mode) ? 's' : '-';
    for (int i = 0; i < 9; ++i)
        text[i + 1] = (mode & bits[i]) ? letters[i] : '-';
    if (mode & S_ISUID) text[3] = (mode & S_IXUSR) ? 's' : 'S';
    if (mode & S_ISGID) text[6] = (mode & S_IXGRP) ? 's' : 'S';
    if (mode & S_ISVTX) text[9] = (mode & S_IXOTH) ? 't' : 'T';
    text[10] = '\0';
}

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
    if (dir == NULL) { perror("opendir()"); return 1; }
    for (;;) {
        errno = 0;
        entry = readdir(dir);
        if (entry == NULL) {
            if (errno) { perror("readdir()"); failed = 1; }
            break;
        }
        size_t a = strlen(argv[1]), b = strlen(entry->d_name);
        if (a > SIZE_MAX - b - 2) {
            fprintf(stderr, "Path is too long.\n"); failed = 1; break;
        }
        char *path = malloc(a + b + 2);
        if (path == NULL) { perror("malloc()"); failed = 1; break; }
        snprintf(path, a + b + 2, "%s/%s", argv[1], entry->d_name);
        struct stat st;
        if (lstat(path, &st) == -1) {
            fprintf(stderr, "%s: ", path);
            perror("lstat()"); free(path); failed = 1; continue;
        }
        free(path);
        char mode[11];
        permissions(st.st_mode, mode);
        const char *modified = ctime(&st.st_mtime);
        if (modified == NULL) { perror("ctime()"); failed = 1; continue; }
        printf("%ju %s %ju %ju %ju %jd %.24s %s\n",
               (uintmax_t)st.st_ino, mode, (uintmax_t)st.st_nlink,
               (uintmax_t)st.st_uid, (uintmax_t)st.st_gid,
               (intmax_t)st.st_size, modified, entry->d_name);
    }
    if (closedir(dir) == -1) { perror("closedir()"); failed = 1; }
    return failed;
}
