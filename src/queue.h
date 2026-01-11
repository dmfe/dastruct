#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdbool.h>

typedef void cleanupfunction (void *);
typedef struct _queue Queue;

Queue *queue_create(cleanupfunction *clf);
void queue_destroy(Queue *q);
bool queue_enqueue(Queue *q, void *element);
void *queue_dequeue(Queue *q);

void queue_print(Queue *q);

#endif
