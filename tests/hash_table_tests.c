#include <criterion/criterion.h>
#include <string.h>
#include <stdbool.h>

#include "../src/hashtable.h"

Hash_table *ht;

void suite_setup() {
    ht = hash_table_create(NULL, NULL);
}

void suite_teardown() {
    hash_table_destroy(ht);
}

TestSuite(hash_table_tests, .init=suite_setup, .fini=suite_teardown);

Test(hash_table_tests, hash_table_create) {
    cr_expect_not_null(ht, "hash_table_create should return not NULL");
}

Test(hash_table_tests, hash_table_insert_into_null_hash_table) {
    bool result = hash_table_insert(NULL, NULL, NULL);

    cr_expect(result == false, "hash_table_insert should return false, when trying to insert into NULL hash table");
}

Test(hash_table_tests, hash_table_insert_null_key) {
    bool result = hash_table_insert(ht, NULL, NULL);

    cr_expect(result == false, "hash_table_insert should return false, when trying to insert NULL key");
}

Test(hash_table_tests, hash_table_insert_null_value) {
    char *key = "key";

    bool result = hash_table_insert(ht, key, NULL);

    cr_expect(result == false, "hash_table_insert should return false, when trying to insert NULL value");
}

Test(hash_table_tests, hash_table_insert_existing_key) {
    char *key = "key";
    char *value = strdup("value");
    hash_table_insert(ht, key, value);

    bool result = hash_table_insert(ht, key, value);

    cr_expect(result == false, "hash_table_insert should return false, when trying to insert existing key");
}

Test(hash_table_tests, hash_table_insert_not_existing_key) {
    char *key = "key";
    char *value = strdup("value");

    bool result = hash_table_insert(ht, key, value);

    cr_expect(result == true, "hash_table_insert should return true, when trying to insert not existing key");
}

Test(hash_table_tests, hash_table_find_by_not_existing_key) {
    char *key = "key";

    void *result = hash_table_find(ht, key);

    cr_expect_null(result, "hash_table_find should return NULL, when trying to find value by not existing key");
}

Test(hash_table_tests, hash_table_find_by_existing_key) {
    char *key = "key";
    char *value = strdup("value");
    hash_table_insert(ht, key, value);

    void *result = hash_table_find(ht, key);

    cr_expect_eq(value, result, "hash_table_find should return pointer to the value, when trying to find value by existing key");
}

Test(hash_table_tests, hash_table_retrieve_by_not_existing_key) {
    char *key = "key";

    void *result = hash_table_retrieve(ht, key);

    cr_expect_null(result, "hash_table_retrieve should return NULL, when trying to retrieve value by not existing key");
}

Test(hash_table_tests, hash_table_retrieve_by_existing_key) {
    char *key = "key";
    char *value = strdup("value");
    hash_table_insert(ht, key, value);

    void *result = hash_table_retrieve(ht, key);

    cr_expect_eq(value, result, "hash_table_retrieve should return pointer to the value, when trying to retrieve value by existing key");
}

Test(hash_table_tests, hash_table_retrieve_by_already_retrieved_key) {
    char *key = "key";
    char *value = strdup("value");
    hash_table_insert(ht, key, value);
    hash_table_retrieve(ht, key);

    void *result = hash_table_retrieve(ht, key);

    cr_expect_null(result, "hash_table_retrieve should return NULL, when trying to retrieve value by the key, which was already retrieved");
}
