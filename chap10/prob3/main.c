#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    const char *src = "Do you like Linux?";
    char *str = malloc(100);
    char *p;

    if (str == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    /* memcpy: 지정된 크기의 메모리를 복사한다. */
    memcpy(str, src, strlen(src) + 1);
    printf("%s\n", str);

    /* memset: 지정된 메모리를 특정 값으로 채운다. */
    memset(str + 12, 'l', 1);
    printf("%s\n", str);

    /* memchr: 지정된 값이 처음 나타나는 주소를 찾는다. */
    p = memchr(str, 'l', strlen(str));

    if (p != NULL)
        printf("%s\n", p);

    /* 원본 문자열 복원 */
    memcpy(str, src, strlen(src) + 1);

    /* memmove: 겹치는 메모리 영역도 안전하게 이동한다. */
    memmove(str + 12, str + 7, 10);
    str[22] = '\0';

    printf("%s\n", str);

    free(str);
    return 0;
}
