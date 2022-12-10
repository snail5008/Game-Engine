#include "camera.h"
#include <cglm/cglm.h>

struct Camera camera_create(float fov, float w, float h) {
    struct Camera cam;
    glm_perspective(fov, w / h, 0.1, 100, cam.proj);
    mat4_identity(cam.view);

    vec3_cpy((Vector3) { 0, 0, -5 }, cam.position);
    vec3_cpy((Vector3) { 0, 0, 1 }, cam.direction);

    return cam;
}

void camera_update(struct Camera* cam) {
    Vector3 tmpvec;
    vec3_add(cam->position, cam->direction, tmpvec);
    glm_lookat(cam->position, tmpvec, (Vector3) { 0, 1, 0 }, cam->view);
}

void camera_translate(struct Camera* cam, Vector3 translate_to) {
    vec3_cpy(cam->position, translate_to);
}
