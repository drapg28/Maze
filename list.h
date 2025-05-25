/* ============================= list.h ============================== */
#ifndef LIST_H
#define LIST_H
#include <stddef.h>

typedef struct LNode {
    void           *data;
    struct LNode   *next;
} LNode;

typedef struct {
    LNode    *head;
    size_t    elsize;
} List;

void listInit(List *l, size_t elsize);
int  listInsertFront(List *l, const void *elem);
void listTraverse(const List *l, void (*visit)(void *));
void listDestroy(List *l);

#endif /* LIST_H */
