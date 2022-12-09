#include "camera.h"

struct Camera camera_create(float fov, float w, float h) {
    struct Camera cam;
    glm_perspective(fov, w / h, 0.1, 100, cam.proj);
    glm_mat4_identity(cam.view);

    glm_vec3_copy((vec3) { 0, 0, -5 }, cam.position);
    glm_vec3_copy((vec3) { 0, 0, 1 }, cam.direction);

    return cam;
}

void camera_update(struct Camera* cam) {
    vec3 tmpvec;
    glm_vec3_add(cam->position, cam->direction, tmpvec);
    glm_lookat(cam->position, tmpvec, (vec3) { 0, 1, 0}, cam->view);
}

void camera_translate(struct Camera* cam, vec3 translate_to) {
    glm_vec3_copy(cam->position, translate_to);
}
