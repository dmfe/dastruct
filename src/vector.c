#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include "vector.h"

#define INITIAL_CAPACITY 32

struct _vector {
    uint32_t size;
    uint32_t capacity;
    cleanupfunction *cleanup;
    void **data;
};

static bool vector_expand(Vector *v, uint32_t new_size) {
    uint32_t new_capacity = new_size != 0 ? new_size : v->capacity * 2;

    // Overflow. Capacity would be too big.
    if (new_capacity < v->capacity) return false;

    void **new_data = calloc(new_capacity, sizeof(*new_data));
    if (new_data == NULL) return false; 

    for (uint32_t i = 0; i < v->capacity; i++) {
        if (v->data[i]) {
            new_data[i] = v->data[i];
        }
    }

    free(v->data);
    v->data = new_data;
    v->capacity = new_capacity;

    return true;
}

Vector *vector_create(cleanupfunction *clf) {
    Vector *v = malloc(sizeof(*v));
    if (v == NULL) return NULL;

    v->size = 0;
    v->capacity = INITIAL_CAPACITY;
    v->cleanup = clf != NULL ? clf : free;
    v->data = calloc(v->capacity, sizeof(*v->data));

    return v;
}

void vector_destroy(Vector *v) {
    if (v == NULL) return;

    // Cleanup elements in data
    for (uint32_t i = 0; i < v->capacity; i++) {
        if (v->data[i]) {
            v->cleanup(v->data[i]);
        }
    }

    free(v->data);
    free(v);
}

void vector_push(Vector *v, void *element) {
    if (v == NULL || element == NULL) return;

    if (v->size >= v->capacity) {
        // If unable to expand, then do nothing
        if (!vector_expand(v, 0)) return;
    }

    v->data[v->size] = element;
    v->size++;
}

void vector_insert(Vector *v, uint32_t index, void *element) {
    if (v == NULL || element == NULL) return;

    if (index >= v->capacity) {
        // If unable to expand, then do nothing
        if (!vector_expand(v, index + INITIAL_CAPACITY)) return;
    }

    void *prev_element = v->data[index];
    v->data[index] = element;
    if (prev_element) {
        v->cleanup(prev_element);
    } else {
        v->size++;
    }
}

void *vector_pop(Vector *v) {
    if (v == NULL || v->size == 0) return NULL;

    void *result = NULL;
    for (uint32_t i = v->capacity - 1; i >= 0; i--) {
        if (v->data[i]) {
            result = v->data[i];
            v->data[i] = 0;
            v->size--;
            break;
        }
    }

    return result;
}

void *vector_get(Vector *v, uint32_t index) {
    if (v == NULL || index >= v->capacity) return NULL;
    return v->data[index];
}

void vector_delete(Vector *v, uint32_t index) {
    if (v == NULL || index >= v->capacity) return;
    if (v->data[index] == NULL) return;

    // Cleanup element and shift left all elements starting from index + 1
    v->cleanup(v->data[index]);
    for (uint32_t i = index; i < v->capacity - 1; i++) {
        v->data[i] = v->data[i + 1];
    }
    v->size--;
}

void vector_delete_nosh(Vector *v, uint32_t index) {
    if (v == NULL || index >= v->capacity) return;
    if (v->data[index] == NULL) return;

    // Don't do letf shifting, just cleanup and zeroing element
    v->cleanup(v->data[index]);
    v->data[index] = 0;
    v->size--;
}

uint32_t vector_get_next_null_pos(Vector *v, uint32_t start_pos) {
    uint32_t next_pos = start_pos + 1;
    while (next_pos < v->capacity) {
        if (v->data[next_pos] == NULL) break;
        next_pos++;
    }
    return next_pos;
}

uint32_t vector_size(Vector *v) {
    return v->size;
}

uint32_t vector_capacity(Vector *v) {
    return v->capacity;
}

void vector_print(Vector *v) {
    printf("---Start Vector---\n");
    for (uint32_t i = 0; i < v->capacity; i++) {
        if (v->data[i]) {
            printf("%u(%p) - ", i, v->data[i]);
        }
    }
    printf("\n");
    printf("---End Vector---\n");
}
