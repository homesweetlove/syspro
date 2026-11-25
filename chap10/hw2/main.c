#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

struct node {
    int data;
    struct node *next;
};

void addq(struct node *head, struct node *tail, int data)
{
    struct node *p = malloc(sizeof(*p));

    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    p->data = data;
    p->next = NULL;

    if (head->next == NULL)
        head->next = p;
    else
        tail->next->next = p;

    tail->next = p;
}

int delete(struct node *head, struct node *tail)
{
    struct node *p;
    int data;

    if (head->next == NULL)
        return -1;

    p = head->next;
    data = p->data;
    head->next = p->next;

    if (head->next == NULL)
        tail->next = NULL;

    free(p);
    return data;
}

int main(void)
{
    struct node head = {0, NULL};
    struct node tail = {0, NULL};
    char input[100];
    char *end;
    long data;

    while (scanf("%99s", input) == 1) {
        errno = 0;
        data = strtol(input, &end, 10);

        if (errno != 0 || end == input ||
            *end != '\0' || data <= 0 || data > INT_MAX)
            break;

        addq(&head, &tail, (int)data);
    }

    printf("Print queue\n");

    while (head.next != NULL)
        printf("%d\n", delete(&head, &tail));

    return 0;
}
