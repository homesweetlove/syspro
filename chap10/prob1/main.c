#include <stdio.h>
#include <stdlib.h>

struct student {
    int id;
    char name[100];
};

int main(void)
{
    int n;
    struct student *s;

    printf("몇 명의 학생을 입력하겠습니까? ");

    if (scanf("%d", &n) != 1 || n <= 0 ||
        (size_t)n > (size_t)-1 / sizeof(*s)) {
        fprintf(stderr, "잘못된 학생 수입니다.\n");
        return 1;
    }

    s = malloc((size_t)n * sizeof(*s));

    if (s == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        return 1;
    }

    printf("%d 명의 학번과 이름을 입력하세요.\n", n);

    for (int i = 0; i < n; i++) {
        if (scanf("%d %99s", &s[i].id, s[i].name) != 2) {
            fprintf(stderr, "잘못된 학생 정보입니다.\n");
            free(s);
            return 1;
        }
    }

    printf("\n* 학생 정보(역순) *\n");

    for (int i = n - 1; i >= 0; i--)
        printf("%d %s\n", s[i].id, s[i].name);

    free(s);
    return 0;
}
