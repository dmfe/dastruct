#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

#define MAX_LINE 4096
#define FNV_PRIME 0x100000001b3
#define FNV_OFFSET 0xcbf29ce4842232UL

uint64_t hash(const char* data, size_t lenght) {
    uint64_t hash_value = 0;
    for(int i=0; i<lenght; i++) {
        hash_value += data[i];
        hash_value *= data[i];
    }
    return hash_value;
}

uint64_t hash_fnv1(const char* data, size_t lenght) {
    uint64_t hash_value = FNV_OFFSET;
    for(int i=0; i<lenght; i++) {
        hash_value *= FNV_PRIME;
        hash_value ^= data[i];
    }
    return hash_value;
}

uint64_t hash_fnv1a(const char* data, size_t lenght) {
    uint64_t hash_value = FNV_OFFSET;
    for(int i=0; i<lenght; i++) {
        hash_value ^= data[i];
        hash_value *= FNV_PRIME;
    }
    return hash_value;
}

 void my_cleanupfunction(void *value) {
    printf("freeing mem: %p\n", value);
    free(value);
 }

void generate_random_word(char *buffer, size_t lenght) {
    for (size_t i = 0; i < lenght; i++) {
        buffer[i] = 'a' + (rand() % 26);
    }
    buffer[lenght] = 0;
}

int main(int argc, char **argv) {

    if (argc != 3) {
        printf("usage: %s <wordlist filename> <num guesses>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *filename = argv[1];
    uint32_t num_guesses = atol(argv[2]);

    Hash_table *hash_table = hash_table_create(NULL, NULL);

    FILE *fp = fopen(filename, "r");
    char buffer[MAX_LINE];
    uint32_t words_num = 0;
    while (!feof(fp) && fgets(buffer, MAX_LINE, fp) != NULL) {
        buffer[strcspn(buffer, "\n\r")] = 0;
        if (hash_table_find(hash_table, buffer) == NULL) {
            char *new_value = strdup(buffer);
            if (hash_table_insert(hash_table, buffer, new_value)) words_num++;
        }
    }
    fclose(fp);

    printf("Loaded %u words into the hash table\n", words_num);
    printf("\t...with %lu collisions\n", hash_table_collisions(hash_table));
    printf("\t...Capacity: %u\n", hash_table_capacity(hash_table));
    printf("\t...Size: %u\n", hash_table_size(hash_table));

    // Debug
    //hash_table_print(hash_table);

    // Make guesses
    uint32_t good_guesses = 0;
    const int shortest_guess = 2;
    const int longest_guess = 15;
    for (uint32_t i = 0; i < num_guesses; i++) {
        generate_random_word(buffer, shortest_guess + (rand() % (longest_guess - shortest_guess)));
        if (hash_table_find(hash_table, buffer)) {
            good_guesses++;
        }
    }
    printf("%u of %u guesses were in the hash tabled\n", good_guesses, num_guesses);

    hash_table_destroy(hash_table);

    return 0;
}
