#pragma once

#include "Core/Defines.h"
#include "Math/VectorTypes.h"
#include "Math/Math.h"

THALLIUM_INLINE Vector2f Vector2f_Create(f32 x, f32 y) {
    Vector2f result;
    result.x = x;
    result.y = y;
    return result;
}

THALLIUM_INLINE Vector2f Vector2f_FromScalar(f32 s) {
    return Vector2f_Create(s, s);
}

THALLIUM_INLINE Vector2f Vector2f_Zero() {
    return Vector2f_FromScalar(0.0f);
}

THALLIUM_INLINE Vector2f Vector2f_Up() {
    return Vector2f_Create(0.0f, 1.0f);
}

THALLIUM_INLINE Vector2f Vector2f_Down() {
    return Vector2f_Create(0.0f, -1.0f);
}

THALLIUM_INLINE Vector2f Vector2f_Left() {
    return Vector2f_Create(-1.0f, 0.0f);
}

THALLIUM_INLINE Vector2f Vector2f_Right() {
    return Vector2f_Create(1.0f, 0.0f);
}

THALLIUM_INLINE Vector2f Vector2f_Add(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

THALLIUM_INLINE Vector2f Vector2f_Sub(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

THALLIUM_INLINE Vector2f Vector2f_Mul(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    return result;
}

THALLIUM_INLINE Vector2f Vector2f_Div(Vector2f a, Vector2f b) {
    Vector2f result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    return result;
}

THALLIUM_INLINE f32 Vector2f_SqrLength(Vector2f v) {
    return (v.x * v.x) + (v.y * v.y);
}

THALLIUM_INLINE f32 Vector2f_Length(Vector2f v) {
    return f32sqrt(Vector2f_SqrLength(v));
}

THALLIUM_INLINE Vector2f Vector2f_Normalized(Vector2f v) {
    return Vector2f_Div(v, Vector2f_FromScalar(Vector2f_Length(v)));
}

THALLIUM_INLINE b8 Vector2f_Equal(Vector2f a, Vector2f b, f32 tolerance) {
    if (f32abs(a.x - b.x) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.y - b.y) > tolerance) {
        return FALSE;
    }

    return TRUE;
}

THALLIUM_INLINE f32 Vector2f_SqrDistance(Vector2f a, Vector2f b) {
    return Vector2f_SqrLength(Vector2f_Sub(a, b));
}

THALLIUM_INLINE f32 Vector2f_Distance(Vector2f a, Vector2f b) {
    return f32sqrt(Vector2f_SqrDistance(a, b));
}

THALLIUM_INLINE f32 Vector2f_Dot(Vector2f a, Vector2f b) {
    return (a.x * b.x) + (a.y * b.y);
}

THALLIUM_INLINE Vector3f Vector3f_Create(f32 x, f32 y, f32 z) {
    Vector3f result;
    result.x = x;
    result.y = y;
    result.z = z;
    return result;
}

THALLIUM_INLINE Vector3f Vector3f_FromScalar(f32 s) {
    return Vector3f_Create(s, s, s);
}

THALLIUM_INLINE Vector3f Vector3f_Zero() {
    return Vector3f_FromScalar(0.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Up() {
    return Vector3f_Create(0.0f, 1.0f, 0.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Down() {
    return Vector3f_Create(0.0f, -1.0f, 0.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Left() {
    return Vector3f_Create(-1.0f, 0.0f, 0.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Right() {
    return Vector3f_Create(1.0f, 0.0f, 0.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Forward() {
    return Vector3f_Create(0.0f, 0.0f, 1.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Back() {
    return Vector3f_Create(0.0f, 0.0f, -1.0f);
}

THALLIUM_INLINE Vector3f Vector3f_Add(Vector3f a, Vector3f b) {
    Vector3f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

THALLIUM_INLINE Vector3f Vector3f_Sub(Vector3f a, Vector3f b) {
    Vector3f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

THALLIUM_INLINE Vector3f Vector3f_Mul(Vector3f a, Vector3f b) {
    Vector3f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    return result;
}

THALLIUM_INLINE Vector3f Vector3f_Div(Vector3f a, Vector3f b) {
    Vector3f result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    return result;
}

THALLIUM_INLINE f32 Vector3f_SqrLength(Vector3f v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z);
}

THALLIUM_INLINE f32 Vector3f_Length(Vector3f v) {
    return f32sqrt(Vector3f_SqrLength(v));
}

THALLIUM_INLINE Vector3f Vector3f_Normalized(Vector3f v) {
    return Vector3f_Div(v, Vector3f_FromScalar(Vector3f_Length(v)));
}

THALLIUM_INLINE b8 Vector3f_Equal(Vector3f a, Vector3f b, f32 tolerance) {
    if (f32abs(a.x - b.x) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.y - b.y) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.z - b.z) > tolerance) {
        return FALSE;
    }

    return TRUE;
}

THALLIUM_INLINE f32 Vector3f_SqrDistance(Vector3f a, Vector3f b) {
    return Vector3f_SqrLength(Vector3f_Sub(a, b));
}

THALLIUM_INLINE f32 Vector3f_Distance(Vector3f a, Vector3f b) {
    return f32sqrt(Vector3f_SqrDistance(a, b));
}

THALLIUM_INLINE f32 Vector3f_Dot(Vector3f a, Vector3f b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
}

THALLIUM_INLINE Vector3f Vector3f_Cross(Vector3f a, Vector3f b) {
    Vector3f result;
    result.x = (a.y * b.z) - (a.z * b.y);
    result.y = (a.z * b.x) - (a.x * b.z);
    result.z = (a.x * b.y) - (a.y * b.x);
    return result;
}

THALLIUM_INLINE Vector4f Vector4f_Create(f32 x, f32 y, f32 z, f32 w) {
    Vector4f result;
    result.x = x;
    result.y = y;
    result.z = z;
    result.w = w;
    return result;
}

THALLIUM_INLINE Vector4f Vector4f_FromScalar(f32 s) {
    return Vector4f_Create(s, s, s, s);
}

THALLIUM_INLINE Vector4f Vector4f_Zero() {
    return Vector4f_FromScalar(0.0f);
}

THALLIUM_INLINE Vector4f Vector4f_Add(Vector4f a, Vector4f b) {
    Vector4f result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    result.w = a.w + b.w;
    return result;
}

THALLIUM_INLINE Vector4f Vector4f_Sub(Vector4f a, Vector4f b) {
    Vector4f result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    result.w = a.w - b.w;
    return result;
}

THALLIUM_INLINE Vector4f Vector4f_Mul(Vector4f a, Vector4f b) {
    Vector4f result;
    result.x = a.x * b.x;
    result.y = a.y * b.y;
    result.z = a.z * b.z;
    result.w = a.w * b.w;
    return result;
}

THALLIUM_INLINE Vector4f Vector4f_Div(Vector4f a, Vector4f b) {
    Vector4f result;
    result.x = a.x / b.x;
    result.y = a.y / b.y;
    result.z = a.z / b.z;
    result.w = a.w / b.w;
    return result;
}

THALLIUM_INLINE f32 Vector4f_SqrLength(Vector4f v) {
    return (v.x * v.x) + (v.y * v.y) + (v.z * v.z) + (v.w * v.w);
}

THALLIUM_INLINE f32 Vector4f_Length(Vector4f v) {
    return f32sqrt(Vector4f_SqrLength(v));
}

THALLIUM_INLINE Vector4f Vector4f_Normalized(Vector4f v) {
    return Vector4f_Div(v, Vector4f_FromScalar(Vector4f_Length(v)));
}

THALLIUM_INLINE b8 Vector4f_Equal(Vector4f a, Vector4f b, f32 tolerance) {
    if (f32abs(a.x - b.x) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.y - b.y) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.z - b.z) > tolerance) {
        return FALSE;
    }

    if (f32abs(a.w - b.w) > tolerance) {
        return FALSE;
    }

    return TRUE;
}

THALLIUM_INLINE f32 Vector4f_SqrDistance(Vector4f a, Vector4f b) {
    return Vector4f_SqrLength(Vector4f_Sub(a, b));
}

THALLIUM_INLINE f32 Vector4f_Distance(Vector4f a, Vector4f b) {
    return f32sqrt(Vector4f_SqrDistance(a, b));
}

THALLIUM_INLINE f32 Vector4f_Dot(Vector4f a, Vector4f b) {
    return (a.x * b.x) + (a.y * b.y) + (a.z * b.z) + (a.w * b.w);
}
