/* ============================= list.c ============================== */
#include <stdlib.h>
#include <string.h>
#include "list.h"

void listInit(List *l, size_t elsize) {
    l->head = NULL;
    l->elsize = elsize;
}

int listInsertFront(List *l, const void *elem) {
    LNode *n = malloc(sizeof(LNode));
    if (!n) return 0;
    n->data = malloc(l->elsize);
    if (!n->data) { free(n); return 0; }
    memcpy(n->data, elem, l->elsize);
    n->next = l->head;
    l->head = n;
    return 1;
}

void listTraverse(const List *l, void (*visit)(void *)) {
    for (LNode *p = l->head; p; p = p->next) visit(p->data);
}

void listDestroy(List *l) {
    void *tmp = malloc(l->elsize);
    LNode *p = l->head;
    while (p) {
        LNode *next = p->next;
        memcpy(tmp, p->data, l->elsize);
        free(p->data);
        free(p);
        p = next;
    }
    free(tmp);
}