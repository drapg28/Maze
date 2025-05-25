/* ============================= stack.c ============================== */
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void stackInit(Stack *s, size_t elsize) {
    s->top = NULL;
    s->elsize = elsize;
}

int stackPush(Stack *s, const void *elem) {
    SNode *n = malloc(sizeof(SNode));
    if (!n) return 0;
    n->data = malloc(s->elsize);
    if (!n->data) { free(n); return 0; }
    memcpy(n->data, elem, s->elsize);
    n->next = s->top;
    s->top = n;
    return 1;
}

int stackPop(Stack *s, void *out) {
    if (!s->top) return 0;
    SNode *n = s->top;
    if (out) memcpy(out, n->data, s->elsize);
    s->top = n->next;
    free(n->data);
    free(n);
    return 1;
}

int stackIsEmpty(const Stack *s) {
    return s->top == NULL;
}

void stackDestroy(Stack *s) {
    void *tmp = malloc(s->elsize);
    while (!stackIsEmpty(s)) stackPop(s, tmp);
    free(tmp);
}