#pragma once

#include "Core/Defines.h"
#include "Math/MatrixTypes.h"
#include "Math/Vector.h"

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Identity() {
    Matrix4x4f result = {};
    result.Data[0] = 1.0f;
    result.Data[5] = 1.0f;
    result.Data[10] = 1.0f;
    result.Data[16] = 1.0f;
    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Mul(Matrix4x4f a, Matrix4x4f b) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32* aPtr = a.Data;
    f32* bPtr = b.Data;
    f32* dstPtr = result.Data;

    for (u64 i = 0; i < 4; i++) {
        for (u64 j = 0; j < 4; j++) {
            *dstPtr =
                aPtr[0] * bPtr[0 + j] +
                aPtr[1] * bPtr[4 + j] +
                aPtr[2] * bPtr[8 + j] +
                aPtr[3] * bPtr[12 + j];
            dstPtr++;
        }
        aPtr += 4;
    }

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Orthographic(f32 left, f32 right, f32 bottom, f32 top, f32 nearClip, f32 farClip) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32 lr = 1.0f / (left - right);
    f32 bt = 1.0f / (bottom - top);
    f32 nf = 1.0f / (nearClip - farClip);

    result.Data[0] = -2.0 * lr;
    result.Data[5] = -2.0 * bt;
    result.Data[10] = -2.0 * nf;

    result.Data[12] = (left + right) * lr;
    result.Data[13] = (top + bottom) * bt;
    result.Data[14] = (nearClip + farClip) * nf;

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Perspective(f32 fovRadians, f32 aspectRatio, f32 nearClip, f32 farClip) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32 halfTanFov = f32tan(fovRadians * 0.5f);

    result.Data[0] = 1.0f / (aspectRatio * halfTanFov);
    result.Data[5] = 1.0f / halfTanFov;
    result.Data[10] = -((farClip + nearClip) / (farClip - nearClip));
    result.Data[11] = 1.0f;
    result.Data[14] = -((2.0f * farClip * nearClip) / (farClip - nearClip));

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_LookAt(Vector3f position, Vector3f target, Vector3f up) {
    Matrix4x4f result = Matrix4x4f_Identity();

    Vector3f zAxis = Vector3f_Normalized(Vector3f_Sub(target, position));
    Vector3f xAxis = Vector3f_Normalized(Vector3f_Cross(zAxis, up));
    Vector3f yAxis = Vector3f_Cross(xAxis, zAxis);

    result.Data[0] = xAxis.x;
    result.Data[1] = yAxis.x;
    result.Data[2] = zAxis.x;
    result.Data[3] = 0.0f;
    
    result.Data[4] = xAxis.y;
    result.Data[5] = yAxis.y;
    result.Data[6] = zAxis.y;
    result.Data[7] = 0.0f;
    
    result.Data[8] = xAxis.y;
    result.Data[9] = yAxis.y;
    result.Data[10] = zAxis.y;
    result.Data[11] = 0.0f;

    result.Data[12] = -Vector3f_Dot(xAxis, position);
    result.Data[13] = -Vector3f_Dot(yAxis, position);
    result.Data[14] = -Vector3f_Dot(zAxis, position);
    result.Data[15] = 1.0f;

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Transpose(Matrix4x4f matrix) {
    Matrix4x4f result = Matrix4x4f_Identity();

    result.Data[0] = matrix.Data[0];
    result.Data[1] = matrix.Data[4];
    result.Data[2] = matrix.Data[8];
    result.Data[3] = matrix.Data[12];

    result.Data[4] = matrix.Data[1];
    result.Data[5] = matrix.Data[5];
    result.Data[6] = matrix.Data[9];
    result.Data[7] = matrix.Data[13];

    result.Data[8] = matrix.Data[2];
    result.Data[9] = matrix.Data[6];
    result.Data[10] = matrix.Data[10];
    result.Data[11] = matrix.Data[14];

    result.Data[12] = matrix.Data[3];
    result.Data[13] = matrix.Data[7];
    result.Data[14] = matrix.Data[11];
    result.Data[15] = matrix.Data[15];

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Inverse(Matrix4x4f matrix) {
    f32* m = matrix.Data;

    f32 t0 = m[10] * m[15];
    f32 t1 = m[14] * m[11];
    f32 t2 = m[6] * m[15];
    f32 t3 = m[14] * m[7];
    f32 t4 = m[6] * m[11];
    f32 t5 = m[10] * m[7];
    f32 t6 = m[2] * m[15];
    f32 t7 = m[14] * m[3];
    f32 t8 = m[2] * m[11];
    f32 t9 = m[10] * m[3];
    f32 t10 = m[2] * m[7];
    f32 t11 = m[6] * m[3];
    f32 t12 = m[8] * m[13];
    f32 t13 = m[12] * m[9];
    f32 t14 = m[4] * m[13];
    f32 t15 = m[12] * m[5];
    f32 t16 = m[4] * m[9];
    f32 t17 = m[8] * m[5];
    f32 t18 = m[0] * m[13];
    f32 t19 = m[12] * m[1];
    f32 t20 = m[0] * m[9];
    f32 t21 = m[8] * m[1];
    f32 t22 = m[0] * m[5];
    f32 t23 = m[4] * m[1];

    Matrix4x4f result;
    f32* o = result.Data;

    o[0] = (t0 * m[5] + t3 * m[9] + t4 * m[13]) - (t1 * m[5] + t2 * m[9] + t5 * m[13]);
    o[1] = (t1 * m[1] + t6 * m[9] + t9 * m[13]) - (t0 * m[1] + t7 * m[9] + t8 * m[13]);
    o[2] = (t2 * m[1] + t7 * m[5] + t10 * m[13]) - (t3 * m[1] + t6 * m[5] + t11 * m[13]);
    o[3] = (t5 * m[1] + t8 * m[5] + t11 * m[9]) - (t4 * m[1] + t9 * m[5] + t10 * m[9]);

    f32 d = 1.0f / (m[0] * o[0] + m[4] * o[1] + m[8] * o[2] + m[12] * o[3]);

    o[0] = d * o[0];
    o[1] = d * o[1];
    o[2] = d * o[2];
    o[3] = d * o[3];
    o[4] = d * ((t1 * m[4] + t2 * m[8] + t5 * m[12]) - (t0 * m[4] + t3 * m[8] + t4 * m[12]));
    o[5] = d * ((t0 * m[0] + t7 * m[8] + t8 * m[12]) - (t1 * m[0] + t6 * m[8] + t9 * m[12]));
    o[6] = d * ((t3 * m[0] + t6 * m[4] + t11 * m[12]) - (t2 * m[0] + t7 * m[4] + t10 * m[12]));
    o[7] = d * ((t4 * m[0] + t9 * m[4] + t10 * m[8]) - (t5 * m[0] + t8 * m[4] + t11 * m[8]));
    o[8] = d * ((t12 * m[7] + t15 * m[11] + t16 * m[15]) - (t13 * m[7] + t14 * m[11] + t17 * m[15]));
    o[9] = d * ((t13 * m[3] + t18 * m[11] + t21 * m[15]) - (t12 * m[3] + t19 * m[11] + t20 * m[15]));
    o[10] = d * ((t14 * m[3] + t19 * m[7] + t22 * m[15]) - (t15 * m[3] + t18 * m[7] + t23 * m[15]));
    o[11] = d * ((t17 * m[3] + t20 * m[7] + t23 * m[11]) - (t16 * m[3] + t21 * m[7] + t22 * m[11]));
    o[12] = d * ((t14 * m[10] + t17 * m[14] + t13 * m[6]) - (t16 * m[14] + t12 * m[6] + t15 * m[10]));
    o[13] = d * ((t20 * m[14] + t12 * m[2] + t19 * m[10]) - (t18 * m[10] + t21 * m[14] + t13 * m[2]));
    o[14] = d * ((t18 * m[6] + t23 * m[14] + t15 * m[2]) - (t22 * m[14] + t14 * m[2] + t19 * m[6]));
    o[15] = d * ((t22 * m[10] + t16 * m[2] + t21 * m[6]) - (t20 * m[6] + t23 * m[10] + t17 * m[2]));

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Translation(Vector3f position) {
    Matrix4x4f result = Matrix4x4f_Identity();
    result.Data[12] = position.x;
    result.Data[13] = position.y;
    result.Data[14] = position.z;
    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Scale(Vector3f scale) {
    Matrix4x4f result = Matrix4x4f_Identity();
    result.Data[0] = scale.x;
    result.Data[5] = scale.y;
    result.Data[10] = scale.z;
    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_EulerX(f32 angleRadians) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32 c = f32cos(angleRadians);
    f32 s = f32sin(angleRadians);

    result.Data[5] = c;
    result.Data[6] = s;
    result.Data[9] = -s;
    result.Data[10] = c;

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_EulerY(f32 angleRadians) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32 c = f32cos(angleRadians);
    f32 s = f32sin(angleRadians);

    result.Data[0] = c;
    result.Data[2] = -s; // TODO: Which way does this rotate?
    result.Data[8] = s;
    result.Data[10] = c;

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_EulerZ(f32 angleRadians) {
    Matrix4x4f result = Matrix4x4f_Identity();

    f32 c = f32cos(angleRadians);
    f32 s = f32sin(angleRadians);

    result.Data[0] = c;
    result.Data[1] = s;
    result.Data[4] = -s;
    result.Data[5] = c;

    return result;
}

THALLIUM_INLINE Matrix4x4f Matrix4x4f_Euler(Vector3f rotationRadians) {
    Matrix4x4f x = Matrix4x4f_EulerX(rotationRadians.x);
    Matrix4x4f y = Matrix4x4f_EulerX(rotationRadians.y);
    Matrix4x4f z = Matrix4x4f_EulerX(rotationRadians.z);
    return Matrix4x4f_Mul(Matrix4x4f_Mul(x, y), z); // TODO: Maybe change the rotation order?
}

THALLIUM_INLINE Vector3f Matrix4x4f_Forward(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = matrix.Data[2];
    result.y = matrix.Data[6];
    result.z = matrix.Data[10];
    return Vector3f_Normalized(result);
}

THALLIUM_INLINE Vector3f Matrix4x4f_Backward(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = -matrix.Data[2];
    result.y = -matrix.Data[6];
    result.z = -matrix.Data[10];
    return Vector3f_Normalized(result);
}

THALLIUM_INLINE Vector3f Matrix4x4f_Up(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = matrix.Data[1];
    result.y = matrix.Data[5];
    result.z = matrix.Data[9];
    return Vector3f_Normalized(result);
}

THALLIUM_INLINE Vector3f Matrix4x4f_Down(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = -matrix.Data[1];
    result.y = -matrix.Data[5];
    result.z = -matrix.Data[9];
    return Vector3f_Normalized(result);
}

THALLIUM_INLINE Vector3f Matrix4x4f_Left(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = -matrix.Data[0];
    result.y = -matrix.Data[4];
    result.z = -matrix.Data[8];
    return Vector3f_Normalized(result);
}

THALLIUM_INLINE Vector3f Matrix4x4f_Right(Matrix4x4f matrix) {
    Vector3f result = {};
    result.x = matrix.Data[0];
    result.y = matrix.Data[4];
    result.z = matrix.Data[8];
    return Vector3f_Normalized(result);
}
