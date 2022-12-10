#pragma once
#include <math.h>

typedef __attribute((aligned(16))) float Vector3[3];
typedef __attribute((aligned(16))) float Matrix4[4][4];

float vec3_len(Vector3 a);

float vec3_dot(Vector3 a, Vector3 b);

void vec3_cpy(Vector3 a, Vector3 b);

void vec3_add(Vector3 a, Vector3 b, Vector3 res);

void vec3_sub(Vector3 a, Vector3 b, Vector3 res);

void vec3_negate(Vector3 a);

void vec3_normalise(Vector3 a, Vector3 res);

void mat4_cpy(Matrix4 A, Matrix4 B);

void mat4_transpose(Matrix4 A, Matrix4 res);

void mat4_mul(Matrix4 A, Matrix4 B, Matrix4 res);

void mat4_identity(Matrix4 A);

void mat4_perspective(float fov, float aspect, float near, float far, Matrix4 A);

void mat4_lookat(Vector3 eye, Vector3 at, Vector3 up, Matrix4 A);
