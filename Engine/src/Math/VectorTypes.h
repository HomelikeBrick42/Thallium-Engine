#pragma once

#include "Core/Defines.h"

typedef union Vector2f {
    f32 Elements[2];
    struct {
        union {
            f32 x, r, s, u;
        };
        union {
            f32 y, g, t, v;
        };
    };
} Vector2f;

typedef union Vector3f {
    f32 Elements[3];
    struct {
        union {
            f32 x, r, s, u;
        };
        union {
            f32 y, g, t, v;
        };
        union {
            f32 z, b, p, w;
        };
    };
} Vector3f;


typedef union Vector4f {
    alignas(16) f32 Elements[4];
    struct {
        union {
            f32 x, r, s;
        };
        union {
            f32 y, g, t;
        };
        union {
            f32 z, b, p;
        };
        union {
            f32 w, a, q;
        };
    };
} Vector4f;
