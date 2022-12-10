#pragma once
#include "maths.h"

struct Camera {
    Matrix4 view;
    Matrix4 proj;

    Vector3 position;
    Vector3 direction;
};

struct Camera camera_create(float fov, float w, float h);

void camera_update(struct Camera* cam);

void camera_translate(struct Camera* cam, Vector3 translate_to);
