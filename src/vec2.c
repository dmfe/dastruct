#include <math.h>
#include <stddef.h>
#include <stdbool.h>

#include "vec2.h"
#include "common.h"

float vec2_length(Vec2 vec) {
    return sqrt(vec.x * vec.x + vec.y * vec.y);
}

Vec2 vec2_add(Vec2 vec1, Vec2 vec2) {
    return (Vec2) {
        vec1.x + vec2.x,
        vec1.y + vec2.y
    };
}

void vec2_add_mut(Vec2 *vec1, Vec2 vec2) {
    if (vec1 == NULL) return;

    vec1->x += vec2.x;
    vec1->y += vec2.y;
}

Vec2 vec2_sub(Vec2 vec1, Vec2 vec2) {
    return (Vec2) {
        vec1.x - vec2.x,
        vec1.y - vec2.y
    };
}

void vec2_sub_mut(Vec2 *vec1, Vec2 vec2) {
    if (vec1 == NULL) return;

    vec1->x -= vec2.x;
    vec1->y -= vec2.y;
}

Vec2 vec2_mult(Vec2 vec, float scalar) {
    return (Vec2) {
        vec.x * scalar,
        vec.y * scalar
    };
}

void vec2_mult_mut(Vec2 *vec, float scalar) {
    if (vec == NULL) return;

    vec->x *= scalar;
    vec->y *= scalar;
}

Vec2 vec2_div(Vec2 vec, float scalar) {
    return (Vec2) {
        vec.x / scalar,
        vec.y / scalar
    };
}

void vec2_div_mut(Vec2 *vec, float scalar) {
    if (vec == NULL) return;

    vec->x /= scalar;
    vec->y /= scalar;
}

void vec2_normalize(Vec2 *vec) {
    float length = vec2_length(*vec);
    if (length > 0) {
        vec2_mult_mut(vec, 1 / length);
    }
}

bool vec2_equals(Vec2 vec1, Vec2 vec2) {
    return float_equals(vec1.x, vec2.x) && float_equals(vec1.y, vec2.y);
}
