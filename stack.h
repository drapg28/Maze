#ifndef STACK_H
#define STACK_H
#include <stddef.h>

typedef struct SNode {
    void               *data;
    struct SNode       *next;
} SNode;

typedef struct {
    SNode   *top;
    size_t   elsize;
} Stack;

void stackInit(Stack *s, size_t elsize);
int  stackPush(Stack *s, const void *elem);
int  stackPop(Stack *s, void *out);
int  stackIsEmpty(const Stack *s);
void stackDestroy(Stack *s);

#endif