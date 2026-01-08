#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define FNV_PRIME 1099511628211UL
#define FNV_OFFSET 14695981039346656037UL
#define INITIAL_CAPACITY 256

typedef struct entry {
    char *key;
    void *value;
    struct entry *next;
} entry;

struct _hash_table {
    uint32_t capacity;
    uint32_t size;
    hash_function *hash;
    cleanupfunction *cleanup;
    entry **buckets;
    uint64_t collisions;
};

static size_t hash_table_index(Hash_table *ht, const char *key) {
    return ht->hash(key, strlen(key)) % ht->capacity;
}

static uint64_t hash_fnv1(const char* data, size_t lenght) {
    uint64_t hash_value = FNV_OFFSET;
    for(int i=0; i<lenght; i++) {
        hash_value *= FNV_PRIME;
        hash_value ^= data[i];
    }
    return hash_value;
}

static void hash_table_expand(Hash_table *ht) {
    uint32_t new_capacity = ht->capacity * 2;

    // Overflow. Capacity would be too big.
    if (new_capacity < ht->capacity) return;

    entry **new_buckets = calloc(new_capacity, sizeof(entry*));
    if (new_buckets == NULL) return; 

    // Iterate through buckets and move non-empty ones to the new buckets
    for (uint32_t i = 0; i < ht->capacity; i++) {
        if (ht->buckets[i]) {
            new_buckets[i] = ht->buckets[i];
        }
    }

    free(ht->buckets);
    ht->buckets = new_buckets;
    ht->capacity = new_capacity;
}

Hash_table *hash_table_create(hash_function *hf, cleanupfunction clf) {
    Hash_table *ht = malloc(sizeof(*ht));
    ht->capacity = INITIAL_CAPACITY;
    ht->size = 0;
    ht->collisions = 0;
    ht->hash = hf != NULL ? hf : hash_fnv1;
    ht->cleanup = clf != NULL ? clf: free;
    // calloc zeroes out the memory
    ht->buckets = calloc(ht->capacity, sizeof(entry*));
    return ht;
}

void hash_table_destroy(Hash_table *ht) {
    // Clean up elements in buckets
    for (uint32_t i = 0; i < ht->capacity; i++) {
        while (ht->buckets[i]) {
            entry *tmp = ht->buckets[i];
            ht->buckets[i] = ht->buckets[i]->next;
            free(tmp->key);
            ht->cleanup(tmp->value);
            free(tmp);
        }
    }

    free(ht->buckets);
    free(ht);
}

bool hash_table_insert(Hash_table *ht, const char *key, void *value) {
    if (ht == NULL || key == NULL || value == NULL) return false;

    if (ht->size >= ht->capacity / 2) {
        hash_table_expand(ht);
    }

    size_t index = hash_table_index(ht, key);

    if (hash_table_find(ht, key) != NULL) return false;

    // Create new entry
    entry *e = malloc(sizeof(*e));
    e->value = value;
    e->key = strdup(key);

    // Insert entry
    if (ht->buckets[index]) {
        ht->collisions++;
    } else {
        ht->size++;
    }
    e->next = ht->buckets[index];
    ht->buckets[index] = e;

    return true;
}

void *hash_table_find(Hash_table *ht, const char *key) {
    if (ht == NULL || key == NULL) return NULL;
    size_t index = hash_table_index(ht, key);

    entry *tmp = ht->buckets[index];
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        tmp = tmp->next;
    }

    if (tmp == NULL) return NULL;
    return tmp->value;
}

void *hash_table_retrieve(Hash_table *ht, const char *key) {
    if (ht == NULL || key == NULL) return NULL;
    size_t index = hash_table_index(ht, key);

    entry *tmp = ht->buckets[index];
    entry *prev = NULL;
    while (tmp != NULL && strcmp(tmp->key, key) != 0) {
        prev = tmp;
        tmp = tmp->next;
    }

    if (tmp == NULL) return NULL;
    if (prev == NULL) {
        // Deleting the head of the list
        ht->buckets[index] = tmp->next;
    } else {
        // Deleting from somewhere not the head of the list
        prev->next = tmp->next;
    }
    void *result = tmp->value;
    free(tmp->key);
    free(tmp);

    return result;
}

uint64_t hash_table_collisions(Hash_table *ht) {
    return ht->collisions;
}

uint32_t hash_table_capacity(Hash_table *ht) {
    return ht->capacity;
}

uint32_t hash_table_size(Hash_table *ht) {
    return ht->size;
}

void hash_table_print(Hash_table *ht) {
    printf("---Start Hash Table---\n");
    for (uint32_t i = 0; i < ht->capacity; i++) {
        if (ht->buckets[i] == NULL) {
            //printf("\t%i\t---\n", i);
        } else {
            printf("\t%i\t", i);
            entry *tmp = ht->buckets[i];
            while(tmp != NULL) {
                printf("\"%s\"(%p) - ", tmp->key, tmp->value);
                tmp = tmp->next;
            }
            printf("\n");
        }
    }
    printf("---End Hash Table---\n");
}
