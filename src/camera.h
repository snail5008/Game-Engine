#pragma once
#include <cglm/cglm.h>

struct Camera {
    mat4 view;
    mat4 proj;

    vec3 position;
    vec3 direction;
};

struct Camera camera_create(float fov, float w, float h);

void camera_update(struct Camera* cam);

void camera_translate(struct Camera* cam, vec3 translate_to);
