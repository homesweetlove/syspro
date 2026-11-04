#include <stdio.h>
#include <stdlib.h>

static void exit_handler(void)
{
    printf("Exit handler called\n");
}

int main(void)
{
    if (atexit(exit_handler) != 0)
        return 1;

    printf("Main function executed\n");
    return 0;
}
