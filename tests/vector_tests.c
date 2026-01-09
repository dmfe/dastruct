#include <criterion/criterion.h>
#include <criterion/parameterized.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include "../src/vector.h"

Vector *v = NULL;

void suite_setup() {
    v = vector_create(NULL);
}

void suite_teardown() {
    vector_destroy(v);
}

TestSuite(vector_tests, .init=suite_setup, .fini=suite_teardown);

Test(vector_tests, vector_create) {
    cr_expect_not_null(v, "vector_create should return not NULL");
}

Test(vector_tests, vector_push_null_element) {
    vector_push(v, NULL);

    cr_expect_eq(0, vector_size(v), "after pushing null element vector size should be equal to zero");
}

Test(vector_tests, vector_push_successful) {
    char *element = "element";

    vector_push(v, element);

    cr_expect_eq(1, vector_size(v), "after pushing one element vector size should be equal to one");
    char *element_from_vector = vector_pop(v);
    cr_expect(strcmp(element_from_vector, element) == 0, "pushed and popped elements should be equal");
}

Test(vector_tests, vector_insert_null_element) {
    vector_insert(v, 0, NULL);

    cr_expect_eq(0, vector_size(v), "after inserting null element vector size should be equal to zero");
}

Test(vector_tests, vector_insert_replace_existing_element) {
    char *element_one = strdup("element_one");
    char *element_two = strdup("element_two");
    vector_push(v, element_one);

    vector_insert(v, 0, element_two);

    char *element_from_vector = vector_get(v, 0);
    cr_expect_eq(1, vector_size(v), "after replacing existing element size of the vector should stay the same");
    cr_expect(strcmp(element_two, element_from_vector) == 0, "after replacing existing element vector should contain new element");
}

int vector_insert_adding_element_params[] = {0, 1000};
ParameterizedTestParameters(vector_tests, vector_insert_adding_element) {
    size_t nb_params = sizeof(vector_insert_adding_element_params) / sizeof(int);
    return cr_make_param_array(int, vector_insert_adding_element_params, nb_params);
}

ParameterizedTest(int *index, vector_tests, vector_insert_adding_element) {
    char *element = strdup("element");

    cr_log_info("inserting element with index %d", *index);
    vector_insert(v, *index, element);

    char *element_from_vector = vector_get(v, *index);
    cr_expect_eq(1, vector_size(v), "after adding new element size of the vector should be increased by one");
    cr_expect(strcmp(element, element_from_vector) == 0, "after adding new element vector should contain this element");
}

Test(vector_tests, vector_pop_from_null_vector) {
    void *element = vector_pop(NULL);

    cr_expect_null(element, "vector_pop should return null when trying to call it on null vector");
}

Test(vector_tests, vector_pop_from_empty_vector) {
    void *result = vector_pop(v);

    cr_expect_null(result, "vector_pop should return null when trying to call it on empty vector");
}

Test(vector_tests, vector_pop_success) {
    char *element = "element";
    vector_push(v, element);

    char *result = vector_pop(v);
    cr_expect_eq(0, vector_size(v), "after popping the element size of the vector should be decreased by one");
    cr_expect(strcmp(element, result) == 0, "vector_pop should return pointer to popped element");
}

Test(vector_tests, vector_get_from_null_vector) {
    void *result = vector_get(NULL, 0);

    cr_expect_null(result, "vector_get should return null when trying to call it on empty vector");
}

Test(vector_tests, vector_get_index_out_of_bounds) {
    void *result = vector_get(v, 1000000000);

    cr_expect_null(result, "vector_get should return null when trying get out of bounds element");
}

Test(vector_tests, vector_get_succeess) {
    char *element = strdup("element");
    vector_push(v, element);

    char *result = vector_get(v, 0);
    cr_expect_eq(1, vector_size(v), "size of the vector should stay the same after getting the element");
    cr_expect(strcmp(element, result) == 0, "vector_get should return pointer to requested element");
}

Test(vector_tests, vector_delete_index_out_of_bounds) {
    char *element = strdup("element");
    vector_push(v, element);

    vector_delete(v, 1000000000);

    cr_expect_eq(1, vector_size(v), "vector_delete shouldn't do anything when trying to delete out of bounds element");
}

Test(vector_tests, vector_delete_zero_element) {
    char *element = strdup("element");
    vector_push(v, element);

    vector_delete(v, 1);

    cr_expect_eq(1, vector_size(v), "vector_delete shouldn't do anything when trying to delete zero element");
}

Test(vector_tests, vector_delete_non_empty_element) {
    char *element = strdup("element");
    vector_push(v, element);

    vector_delete(v, 0);

    void *result = vector_get(v, 0);
    cr_expect_eq(0, vector_size(v), "vector_delete shouldn't do anything when trying to delete zero element");
    cr_expect_null(result, "vector shouldn't contain the element after deletion");
}

Test(vector_tests, vector_delete_shift_elements) {
    char *e1 = strdup("e1");
    char *e2 = strdup("e2");
    char *e3 = strdup("e3");
    vector_push(v, e1);
    vector_push(v, e2);
    vector_push(v, e3);

    vector_delete(v, 1);

    vector_print(v);

    char *res1 = vector_get(v, 0);
    char *res2 = vector_get(v, 1);
    char *res3 = vector_get(v, 2);

    cr_expect(strcmp(e1, res1) == 0, "first element should stay at the same position after deletion of the middle element");
    cr_expect(strcmp(e3, res2) == 0, "last element should be shifted to the left after deletion of the middle element");
    cr_expect_null(res3, "there should be empty element after deletion of the middle element and shifting of the last element");
}

Test(vector_tests, vector_delete_nosh_index_out_of_bounds) {
    char *element = strdup("element");
    vector_push(v, element);

    vector_delete_nosh(v, 1000000000);

    cr_expect_eq(1, vector_size(v), "vector_delete_nosh shouldn't do anything when trying to delete out of bounds element");
}

Test(vector_tests, vector_delete_nosh_zero_element) {
    char *element = strdup("element");
    vector_push(v, element);

    vector_delete_nosh(v, 1);

    cr_expect_eq(1, vector_size(v), "vector_delete_nosh shouldn't do anything when trying to delete zero element");
}

Test(vector_tests, vector_delete_nosh_elements_doesnt_shifted) {
    char *e1 = strdup("e1");
    char *e2 = strdup("e2");
    char *e3 = strdup("e3");
    vector_push(v, e1);
    vector_push(v, e2);
    vector_push(v, e3);

    vector_delete_nosh(v, 1);

    vector_print(v);

    char *res1 = vector_get(v, 0);
    char *res2 = vector_get(v, 1);
    char *res3 = vector_get(v, 2);

    cr_expect_eq(2, vector_size(v), "size should be decreased by one after deletion of the middle element");
    cr_expect(strcmp(e1, res1) == 0, "first element should stay at the same position after deletion of the middle element");
    cr_expect(strcmp(e3, res3) == 0, "last element should stay at the same position after deletion of the middle element");
    cr_expect_null(res2, "there should be empty element after deletion of the middle element");
}

Test(vector_tests, vector_get_next_null_pos) {
    char *e1 = strdup("e1");
    char *e2 = strdup("e2");
    char *e3 = strdup("e3");
    vector_push(v, e1);
    vector_push(v, e2);
    vector_push(v, e3);

    uint32_t result = vector_get_next_null_pos(v, 0);

    cr_expect_eq(3, result, "Next NULL position of three element vector should be three");
}

Test(vector_tests, vector_size) {
    char *e = strdup("e");
    vector_push(v, e);

    uint32_t result = vector_size(v);

    cr_expect_eq(1, result, "Size of the one element vector should be equal to one");
}

Test(vector_tests, vector_capacity_expand) {
    uint32_t initial_capacity = vector_capacity(v);
    cr_log_info("initial vector capacity is: %u", initial_capacity);
    for (uint32_t i = 0; i < initial_capacity; i++) {
        char tmp[10];
        snprintf(tmp, sizeof(tmp), "e%d", i);
        char *e = strdup(tmp);
        vector_push(v, e);
    }
    char *e = strdup("ee");
    vector_push(v, e);

    uint32_t result = vector_capacity(v);

    cr_log_info("vector capacity after extension is: %u", result);
    cr_expect(result > initial_capacity, "extended vector capacity should be greater than initial capacity");
}
