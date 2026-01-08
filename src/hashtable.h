#ifndef __HASHTABLE_H__
#define __HASHTABLE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

typedef uint64_t hash_function (const char*, size_t);
typedef void cleanupfunction (void *);
typedef struct _hash_table Hash_table;

Hash_table *hash_table_create(hash_function *hf, cleanupfunction *clf);
void hash_table_destroy(Hash_table *ht);
bool hash_table_insert(Hash_table *ht, const char *key, void *value);
void *hash_table_find(Hash_table *ht, const char *key);
void *hash_table_retrieve(Hash_table *ht, const char *key);
uint64_t hash_table_collisions(Hash_table *ht);
uint32_t hash_table_capacity(Hash_table *ht);
uint32_t hash_table_size(Hash_table *ht);

void hash_table_print(Hash_table *ht);

#endif
