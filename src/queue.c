#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "queue.h"

typedef struct node node;
struct node {
    void *element;
    node *next;
};

struct _queue {
    node *head;
    node *tail;
    cleanupfunction *cleanup;
};

Queue *queue_create(cleanupfunction *clf) {
    Queue *q = malloc(sizeof(*q));
    if (q == NULL) return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->cleanup = clf != NULL ? clf : free;
    return q;
}

void queue_destroy(Queue *q) {
    if (q == NULL) return;

    node *cur_node = q->head;
    while (cur_node != NULL) {
        q->head = q->head->next;
        q->cleanup(cur_node->element);
        free(cur_node);
        cur_node = q->head;
    }
    free(q);
}

bool queue_enqueue(Queue *q, void *element) {
    if (q == NULL || element == NULL) return false;

    // Create node
    node *node = malloc(sizeof(*node));
    if (node == NULL) return false;
    node->element = element;
    node->next = NULL;

    if (q->tail != NULL) {
        q->tail->next = node;
    }
    q->tail = node;
    if (q->head == NULL) {
        q->head = node;
    }

    return true;
}

void *queue_dequeue(Queue *q) {
    if (q == NULL || q->head == NULL) return NULL;

    node *head_node = q->head;
    void *element = head_node->element;
    q->head = q->head->next;
    if (q->head == NULL) q->tail = NULL;
    free(head_node);

    return element;
}

void queue_print(Queue *q) {
    if (q == NULL) return;

    node *node = q->head;
    while (node != NULL) {
        printf("(%p) - ", node->element);
        node = node->next;
    }
    printf("\n");
}
