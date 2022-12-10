#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdint.h>

#include "camera.h"
#include "utils.h"

struct Mesh {
    uint32_t vao;
    uint32_t vbo;
    uint32_t program;

    Matrix4 model_translation;
    Matrix4 model_scale;
    Matrix4 model_rotation;
};

uint32_t program_create(const char *vertex_source, const char *fragment_source);

void mesh_create_vbo(struct Mesh *mesh, size_t mesh_size, float* mesh_verts);

void mesh_create_default_vao(struct Mesh *mesh);

void mesh_create_default_program(struct Mesh *mesh);

void mesh_draw(struct Mesh mesh, uint32_t vertcount);

void mesh_matrices_init(struct Mesh *mesh);

void mesh_matrices_update(struct Mesh *mesh, struct Camera cam);

void mesh_update(struct Mesh *mesh, uint32_t vertcount, struct Camera cam);
