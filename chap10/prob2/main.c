#include <stdio.h>
#include <stdlib.h>

struct student {
    int id;
    char name[100];
    struct student *next;
};

int main(void)
{
    struct student *head = NULL;
    struct student *p;
    int id, count = 0;

    printf("학번과 이름을 입력하세요\n");

    while (scanf("%d", &id) == 1) {
        p = malloc(sizeof(*p));

        if (p == NULL) {
            fprintf(stderr, "Memory allocation failed.\n");
            while (head != NULL) {
                p = head;
                head = head->next;
                free(p);
            }
            return 1;
        }

        p->id = id;

        if (scanf("%99s", p->name) != 1) {
            free(p);
            break;
        }

        p->next = head;
        head = p;
        count++;
    }

    printf("\n* 학생 정보(역순) *\n");

    while (head != NULL) {
        p = head;
        printf("학번: %d 이름: %s\n", p->id, p->name);
        head = head->next;
        free(p);
    }

    printf("총 %d 명입니다.\n", count);

    return 0;
}
