#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#include "../src/queue.h"

Queue *q = NULL;

void suite_setup() {
    q = queue_create(NULL);
}

void suite_teardown() {
    queue_destroy(q);
}

TestSuite(queue_tests, .init=suite_setup, .fini=suite_teardown);

Test(queue_tests, queue_create) {
    cr_expect_not_null(q, "queue_create should return not NULL pointer to queue struct");
}

Test(queue_tests, queue_enqueue_null_queue) {
    bool result = queue_enqueue(NULL, NULL);

    cr_expect_not(result, "queue_enqueue should return false, when trying to call it on NULL queue");
}

Test(queue_tests, queue_enqueue_null_element) {
    bool result = queue_enqueue(q, NULL);

    cr_expect_not(result, "queue_enqueue should return false, when trying to insert NULL element");
}

Test(queue_tests, queue_enqueue) {
    char *e = strdup("e");

    bool result = queue_enqueue(q, e);

    queue_print(q);
    cr_expect(result, "queue_enqueue should successfully added element at the end of the queue");
}

Test(queue_tests, queue_ordering_test) {
    char *e1 = strdup("e1");
    char *e2 = strdup("e2");
    char *e3 = strdup("e3");
    queue_enqueue(q, e1);
    queue_enqueue(q, e2);
    queue_enqueue(q, e3);
    queue_print(q);

    void *res1 = queue_dequeue(q);
    void *res2 = queue_dequeue(q);
    void *res3 = queue_dequeue(q);

    cr_expect_eq(e1, res1, "first added element should be dequeued first");
    cr_expect_eq(e2, res2, "second added element should be dequeued second");
    cr_expect_eq(e3, res3, "third added element should be dequeued third");
}

Test(queue_tests, queue_dequeue_null_queue) {
    void *result = queue_dequeue(NULL);

    cr_expect_not(result, "queue_dequeue should return NULL pointer, when trying to call it on NULL queue");
}

Test(queue_tests, queue_dequeue_empty_queue) {
    void *result = queue_dequeue(q);

    cr_expect_not(result, "queue_dequeue should return NULL pointer, when trying to call it on an empty queue");
}

Test(queue_tests, queue_dequeue) {
    char *e = strdup("e");
    queue_enqueue(q, e);

    void *result = queue_dequeue(q);

    cr_expect_eq(e, result, "queue_dequeue should return the same element, which was previously enqueued");
}
