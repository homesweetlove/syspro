#include <stdio.h>
#include <sys/stat.h>
#include <utime.h>

int main(int argc, char **argv)
{
    struct stat st;
    struct utimbuf times;
    if (argc != 3) {
        fprintf(stderr, "Usage: %s source-file target-file\n", argv[0]);
        return 1;
    }
    if (stat(argv[1], &st) == -1) {
        perror("stat()");
        return 1;
    }
    times.actime = st.st_atime;
    times.modtime = st.st_mtime;
    if (utime(argv[2], &times) == -1) {
        perror("utime()");
        return 1;
    }
    return 0;
}
