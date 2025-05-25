/* ============================= queue.h ============================== */
#ifndef QUEUE_H
#define QUEUE_H
#include <stddef.h>

typedef struct QNode {
    void             *data;
    struct QNode     *next;
} QNode;

typedef struct {
    QNode   *front;
    QNode   *rear;
    size_t   elsize;
} Queue;

void queueInit(Queue *q, size_t elsize);
int  enqueue(Queue *q, const void *elem);
int  dequeue(Queue *q, void *out);
int  queueIsEmpty(const Queue *q);
void queueDestroy(Queue *q);

#endif /* QUEUE_H */