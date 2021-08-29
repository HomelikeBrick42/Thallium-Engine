#include "Math.h"

#include <math.h>

f32 f32sin(f32 x) {
    return cast(f32) sin(cast(double) x);
}

f32 f32cos(f32 x) {
    return cast(f32) cos(cast(double) x);
}

f32 f32tan(f32 x) {
    return cast(f32) tan(cast(double) x);
}

f32 f32acos(f32 x) {
    return cast(f32) acos(cast(double) x);
}

f32 f32sqrt(f32 x) {
    return cast(f32) sqrt(cast(double) x);
}

f32 f32abs(f32 x) {
    if (x < 0.0f) {
        return -x;
    }
    return x;
}
