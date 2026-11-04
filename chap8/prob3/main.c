#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    const char *names[] = {"HOME", "SHELL", "PATH"};

    for (int i = 0; i < 3; i++) {
        const char *value = getenv(names[i]);
        printf("%s = %s\n", names[i],
               value != NULL ? value : "(null)");
    }
    return 0;
}
