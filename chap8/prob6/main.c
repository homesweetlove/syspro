#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Real User ID: %ld\n", (long)getuid());
    printf("Effective User ID: %ld\n", (long)geteuid());
    printf("Real Group ID: %ld\n", (long)getgid());
    printf("Effective Group ID: %ld\n", (long)getegid());
    return 0;
}
