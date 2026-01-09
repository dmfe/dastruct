#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <stdint.h>

typedef void cleanupfunction (void *);
typedef struct _vector Vector;

Vector *vector_create(cleanupfunction *clf);
void vector_destroy(Vector *v);
void vector_push(Vector *v, void *element);
void vector_insert(Vector *v, uint32_t index, void *element);
void *vector_pop(Vector *v);
void *vector_get(Vector *v, uint32_t index);
void vector_delete(Vector *v, uint32_t index);
void vector_delete_nosh(Vector *v, uint32_t index);
uint32_t vector_get_next_null_pos(Vector *v, uint32_t start_pos);
uint32_t vector_size(Vector *v);
uint32_t vector_capacity(Vector *v);

void vector_print(Vector *v);

#endif
