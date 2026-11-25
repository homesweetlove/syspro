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
#include <unistd.h>

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

static void print_name(const char *name, int quote, int directory)
{
    if (quote) putchar('"');
    for (const unsigned char *p = (const unsigned char *)name; *p; ++p) {
        if (quote && (*p == '"' || *p == '\\')) printf("\\%c", *p);
        else if (quote && *p == '\n') fputs("\\n", stdout);
        else if (quote && *p == '\r') fputs("\\r", stdout);
        else if (quote && *p == '\t') fputs("\\t", stdout);
        else if (quote && (*p < 32 || *p == 127)) printf("\\%03o", *p);
        else putchar(*p);
    }
    if (quote) putchar('"');
    if (directory) putchar('/');
    putchar('\n');
}

int main(int argc, char **argv)
{
    int all = 0, inode = 0, slash = 0, quote = 0, details = 0;
    int option, failed = 0;
    while ((option = getopt(argc, argv, "aipQl")) != -1) {
        switch (option) {
        case 'a': all = 1; break;
        case 'i': inode = 1; break;
        case 'p': slash = 1; break;
        case 'Q': quote = 1; break;
        case 'l': details = 1; break;
        default:
            fprintf(stderr, "Usage: %s [-aipQl] [directory]\n", argv[0]); return 1;
        }
    }
    if (argc - optind > 1) {
        fprintf(stderr, "Usage: %s [-aipQl] [directory]\n", argv[0]); return 1;
    }
    const char *directory = optind < argc ? argv[optind] : ".";
    DIR *dir = opendir(directory);
    if (dir == NULL) { perror("opendir()"); return 1; }
    for (;;) {
        errno = 0;
        struct dirent *entry = readdir(dir);
        if (entry == NULL) {
            if (errno) { perror("readdir()"); failed = 1; }
            break;
        }
        if (!all && entry->d_name[0] == '.') continue;
        size_t a = strlen(directory), b = strlen(entry->d_name);
        if (a > SIZE_MAX - b - 2) {
            fprintf(stderr, "Path is too long.\n"); failed = 1; break;
        }
        char *path = malloc(a + b + 2);
        if (path == NULL) { perror("malloc()"); failed = 1; break; }
        snprintf(path, a + b + 2, "%s/%s", directory, entry->d_name);
        struct stat st;
        if (lstat(path, &st) == -1) {
            fprintf(stderr, "%s: ", path);
            perror("lstat()"); free(path); failed = 1; continue;
        }
        free(path);
        if (inode) printf("%ju ", (uintmax_t)st.st_ino);
        if (details) {
            char mode[11];
            permissions(st.st_mode, mode);
            const char *modified = ctime(&st.st_mtime);
            if (modified == NULL) { perror("ctime()"); failed = 1; continue; }
            printf("%s %ju %ju %ju %jd %.24s ", mode,
                   (uintmax_t)st.st_nlink, (uintmax_t)st.st_uid,
                   (uintmax_t)st.st_gid, (intmax_t)st.st_size, modified);
        }
        print_name(entry->d_name, quote, slash && S_ISDIR(st.st_mode));
    }
    if (closedir(dir) == -1) { perror("closedir()"); failed = 1; }
    if (fflush(stdout) == EOF) { perror("stdout"); failed = 1; }
    return failed;
}
