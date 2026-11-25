#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

struct node {
    int data;
    struct node *next;
};

void push(struct node *top, int data)
{
    struct node *p = malloc(sizeof(*p));

    if (p == NULL) {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(EXIT_FAILURE);
    }

    p->data = data;
    p->next = top->next;
    top->next = p;
}

int pop(struct node *top)
{
    struct node *p;
    int data;

    if (top->next == NULL)
        return -1;

    p = top->next;
    data = p->data;
    top->next = p->next;

    free(p);
    return data;
}

int main(void)
{
    struct node top = {0, NULL};
    char input[100];
    char *end;
    long data;

    while (scanf("%99s", input) == 1) {
        errno = 0;
        data = strtol(input, &end, 10);

        if (errno != 0 || end == input ||
            *end != '\0' || data <= 0 || data > INT_MAX)
            break;

        push(&top, (int)data);
    }

    printf("Print stack\n");

    while (top.next != NULL)
        printf("%d\n", pop(&top));

    return 0;
}
