#include <stdio.h>
#include <unistd.h>

int main(int argc, char **argv)
{
    if (argc != 3) {
        fprintf(stderr, "Usage: %s original-file hard-link\n", argv[0]);
        return 1;
    }
    if (link(argv[1], argv[2]) == -1) {
        perror("link()");
        return 1;
    }
    return 0;
}
