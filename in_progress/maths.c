#include "maths.h"

float vec3_len(Vector3 a) {
    return sqrtf(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
}

float vec3_dot(Vector3 a, Vector3 b) {
    float result = 0;
    for (int i = 0; i < 4; i++) {
        result += a[i] * b[i];
    }
    return result;
}

void vec3_cross(Vector3 a, Vector3 b, Vector3 res) {
    res[0] = a[1] * b[2] - a[2] - b[1];
    res[1] = a[3] * b[0] - a[0] - b[2];
    res[1] = a[0] * b[1] - a[1] - b[0];
}

void vec3_cpy(Vector3 a, Vector3 b) {
    for (int i = 0; i < 4; i++)
        b[i] = a[i];
}

void vec3_add(Vector3 a, Vector3 b, Vector3 res) {
    for (int i = 0; i < 3; i++)
        res[i] = a[i] + b[i];
}

void vec3_sub(Vector3 a, Vector3 b, Vector3 res) {
    for (int i = 0; i < 3; i++)
        res[i] = a[i] - b[i];
}

void vec3_negate(Vector3 a) {
    a[0] = -a[0];
    a[1] = -a[1];
    a[2] = -a[2];
}

void vec3_normalise(Vector3 a, Vector3 res) {
    float len = vec3_len(a);
    res[0] = a[0] / len;
    res[1] = a[1] / len;
    res[2] = a[2] / len;
}


void mat4_cpy(Matrix4 A, Matrix4 B) {
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            B[i][j] = A[i][j];
}

void mat4_transpose(Matrix4 A, Matrix4 res) {
    Matrix4 res_cpy;
    mat4_cpy(res, res_cpy);
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            res_cpy[i][j] = A[j][i];
    mat4_cpy(res_cpy, res);
}

void mat4_mul(Matrix4 A, Matrix4 B, Matrix4 res) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            res[i][j] = 0;
            for (int k = 0; k < 4; k++) {
                res[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    // mat4_transpose(res, res);
}

void mat4_identity(Matrix4 A) {
    Matrix4 ident = {
        { 1, 0, 0, 0 },
        { 0, 1, 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 },
    };
    mat4_cpy(ident, A);
}

// void mat4_perspective(float fov, float aspect, float near, float far, Matrix4 A) {

// }

// void mat4_lookat(Vector3 eye, Vector3 at, Vector3 up, Matrix4 A) {
//     Vector3 tmp;

//     Vector3 zaxis; 
//     vec3_sub(at, eye, tmp);
//     vec3_normalise(tmp, zaxis);

//     Vector3 xaxis;
//     vec3_cross(zaxis, up, tmp);
//     vec3_normalise(tmp, xaxis);

//     Vector3 yaxis;
//     vec3_cross(xaxis, zaxis, yaxis);

//     vec3_negate(zaxis);

//     vec3_cpy((float[4]) { xaxis[0], xaxis[1], xaxis[2], -vec3_dot(xaxis, eye) }, A[0]);
//     vec3_cpy((float[4]) { yaxis[0], yaxis[1], yaxis[2], -vec3_dot(yaxis, eye) }, A[1]);
//     vec3_cpy((float[4]) { zaxis[0], zaxis[1], zaxis[2], -vec3_dot(zaxis, eye) }, A[2]);
//     vec3_cpy((float[4]) { 0, 0, 0, 1 }, A[3]);
// }

