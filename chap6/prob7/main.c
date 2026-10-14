#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s link-file\n", argv[0]);
        return 1;
    }
    /* link() creates a hard link; unlink() removes its directory entry. */
    if (unlink(argv[1]) == -1) {
        perror("unlink()");
        return 1;
    }
    return 0;
}
