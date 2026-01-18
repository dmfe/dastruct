#include <math.h>

#include "common.h"

#define FLT_EPSILON 1e-6f

bool float_equals(float a, float b) {
    float diff = fabsf(a - b);
    if (diff <= FLT_EPSILON) return true;
    return diff <= FLT_EPSILON * fmaxf(fabsf(a), fabsf(b));
}
