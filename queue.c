/* ============================= queue.c ============================== */
#include <stdlib.h>
#include <string.h>
#include "queue.h"

void queueInit(Queue *q, size_t elsize) {
    q->front = q->rear = NULL;
    q->elsize = elsize;
}

int enqueue(Queue *q, const void *elem) {
    QNode *n = malloc(sizeof(QNode));
    if (!n) return 0;
    n->data = malloc(q->elsize);
    if (!n->data) { free(n); return 0; }
    memcpy(n->data, elem, q->elsize);
    n->next = NULL;
    if (q->rear) q->rear->next = n; else q->front = n;
    q->rear = n;
    return 1;
}

int dequeue(Queue *q, void *out) {
    if (!q->front) return 0;
    QNode *n = q->front;
    if (out) memcpy(out, n->data, q->elsize);
    q->front = n->next;
    if (!q->front) q->rear = NULL;
    free(n->data);
    free(n);
    return 1;
}

int queueIsEmpty(const Queue *q) {
    return q->front == NULL;
}

void queueDestroy(Queue *q) {
    void *tmp = malloc(q->elsize);
    while (!queueIsEmpty(q)) dequeue(q, tmp);
    free(tmp);
}
