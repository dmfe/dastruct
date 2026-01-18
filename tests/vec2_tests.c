#include <criterion/criterion.h>
#include <criterion/internal/assert.h>
#include <math.h>
#include <stdbool.h>

#include "../src/vec2.h"
#include "../src/common.h"

Test(vec2_tests, vec2_length) {
    Vec2 vec = {2.0f, 3.0f};
    float expected_length = sqrt(vec.x * vec.x + vec.y * vec.y);

    float res = vec2_length(vec);

    cr_expect(float_equals(expected_length, res), "vec2_lenght should return correct vector lenght");
}

Test(vec2_tests, vec2_add) {
    Vec2 vec1 = {2.0f, 3.0f};
    Vec2 vec2 = {3.0f, 2.0f};
    Vec2 expected_vec = {5.0f, 5.0f};

    Vec2 res = vec2_add(vec1, vec2);

    cr_expect(vec2_equals(expected_vec, res), "vec2_add should return correct result vector");
}

Test(vec2_tests, vec2_add_mut) {
    Vec2 vec1 = {2.0f, 3.0f};
    Vec2 vec2 = {3.0f, 2.0f};
    Vec2 expected_vec = {5.0f, 5.0f};

    vec2_add_mut(&vec1, vec2);

    cr_expect(vec2_equals(expected_vec, vec1), "vec2_add_mut should correctly update vector");
}

Test(vec2_tests, vec2_sub) {
    Vec2 vec1 = {2.0f, 3.0f};
    Vec2 vec2 = {3.0f, 2.0f};
    Vec2 expected_vec = {-1.0f, 1.0f};

    Vec2 res = vec2_sub(vec1, vec2);

    cr_expect(vec2_equals(expected_vec, res), "vec2_sub should return correct result vector");
}

Test(vec2_tests, vec2_sub_mut) {
    Vec2 vec1 = {2.0f, 3.0f};
    Vec2 vec2 = {3.0f, 2.0f};
    Vec2 expected_vec = {-1.0f, 1.0f};

    vec2_sub_mut(&vec1, vec2);

    cr_expect(vec2_equals(expected_vec, vec1), "vec2_sub_mut should correctly update vector");
}

Test(vec2_tests, vec2_mult) {
    Vec2 vec = {2.0f, 2.0f};
    float scalar = 3.0f;
    Vec2 expected = {6.0f, 6.0f};

    Vec2 res = vec2_mult(vec, scalar);

    cr_expect(vec2_equals(expected, res), "vec2_mult should return correct result vector");
}

Test(vec2_tests, vec2_mult_mut) {
    Vec2 vec = {2.0f, 2.0f};
    float scalar = 3.0f;
    Vec2 expected = {6.0f, 6.0f};

    vec2_mult_mut(&vec, scalar);

    cr_expect(vec2_equals(expected, vec), "vec2_mult_mut should correctly update vector");
}

Test(vec2_tests, vec2_div) {
    Vec2 vec = {2.0f, 2.0f};
    float scalar = 2.0f;
    Vec2 expected = {1.0f, 1.0f};

    Vec2 res = vec2_div(vec, scalar);

    cr_expect(vec2_equals(expected, res), "vec2_div should return correct result vector");
}

Test(vec2_tests, vec2_div_mut) {
    Vec2 vec = {2.0f, 2.0f};
    float scalar = 2.0f;
    Vec2 expected = {1.0f, 1.0f};

    vec2_div_mut(&vec, scalar);

    cr_expect(vec2_equals(expected, vec), "vec2_div_mut should correctly update vector");
}

Test(vec2_tests, vec2_normalize) {
    Vec2 vec = {2.0f, 2.0f};
    float length = vec2_length(vec);
    Vec2 expected = vec2_div(vec, length);

    vec2_normalize(&vec);

    cr_expect(vec2_equals(expected, vec));
}

Test(vec2_tests, vec2_equals) {
    Vec2 vec1 = {2.0f, 2.0f};
    Vec2 vec2 = {2.0f, 2.0f};

    bool res = vec2_equals(vec1, vec2);

    cr_expect(res, "vec2_equals should return true for equal vectors");
}
