#include <stdio.h>
#include <unistd.h>

int main(void)
{
    printf("Process ID: %ld\n", (long)getpid());
    printf("Parent Process ID: %ld\n", (long)getppid());
    return 0;
}
