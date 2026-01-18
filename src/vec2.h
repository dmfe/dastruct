#ifndef __VEC2_H__
#define __VEC2_H__

#include <stdbool.h>

typedef struct Vec2 {
    float x;
    float y;
} Vec2;

float vec2_length(Vec2 vec);
Vec2 vec2_add(Vec2 vec1, Vec2 vec2);
void vec2_add_mut(Vec2 *vec1, Vec2 vec2);
Vec2 vec2_sub(Vec2 vec1, Vec2 vec2);
void vec2_sub_mut(Vec2 *vec1, Vec2 vec2);
Vec2 vec2_mult(Vec2 vec, float scalar);
void vec2_mult_mut(Vec2 *vec, float scalar);
Vec2 vec2_div(Vec2 vec, float scalar);
void vec2_div_mut(Vec2 *vec, float scalar);
void vec2_normalize(Vec2 *vec);
bool vec2_equals(Vec2 vec1, Vec2 vec2);

#endif
