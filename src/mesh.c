#include "mesh.h"
#include <cglm/cglm.h>

uint32_t program_create(const char *vertex_source, const char *fragment_source) {
    int success;
    char infolog[512];

    uint32_t vs = glCreateShader(GL_VERTEX_SHADER);
    uint32_t fs = glCreateShader(GL_FRAGMENT_SHADER);
    uint32_t program = glCreateProgram();

    glShaderSource(vs, 1, &vertex_source, NULL);
    glCompileShader(vs);

    glShaderSource(fs, 1, &fragment_source, NULL);
    glCompileShader(fs);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);

    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
    glGetShaderInfoLog(vs, 512, NULL, infolog);
        printf("%s\n", infolog);
    }

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, NULL, infolog);
        printf("%s\n", infolog);
    }

    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 512, NULL, infolog);
        printf("%s\n", infolog);
    }

    glDetachShader(program, vs);
    glDetachShader(program, fs);
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

void mesh_create_vbo(struct Mesh *mesh, size_t mesh_size, float* mesh_verts) {
    glGenBuffers(1, &mesh->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, mesh_size, mesh_verts, GL_STATIC_DRAW);
}

void mesh_create_default_vao(struct Mesh *mesh) {
    glGenVertexArrays(1, &mesh->vao);
    glBindVertexArray(mesh->vao);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void *)0);
}

void mesh_create_default_program(struct Mesh *mesh) {
    char* vsource = file_read("res/default.vert");
    char* fsource = file_read("res/default.frag");
    mesh->program = program_create(vsource, fsource);
    free(vsource);
    free(fsource);
}

void mesh_draw(struct Mesh mesh, uint32_t vertcount) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh.vbo);
    glBindVertexArray(mesh.vao);
    glUseProgram(mesh.program);
    glDrawArrays(GL_TRIANGLES, 0, vertcount);
}

void mesh_matrices_init(struct Mesh *mesh) {
   mat4_identity(mesh->model_rotation);
   mat4_identity(mesh->model_translation);
   mat4_identity(mesh->model_scale);
}

void mesh_matrices_update(struct Mesh *mesh, struct Camera cam) {

    Matrix4 resulting_matrix;

    Matrix4 A = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12},
        {13, 14, 15, 16}
    };
    Matrix4 B = {
        {17, 18, 19, 20},
        {21, 22, 23, 24},
        {25, 26, 27, 28},
        {29, 30, 31, 32}
    };
    // Matrix4 C = {
    //     {1, 2, 3, 4},
    //     {5, 6, 7, 8},
    //     {9, 10, 11, 12},
    //     {13, 14, 15, 16}
    // };

    // mat4_transpose(A, A);
    // mat4_transpose(B, B);

    // glm_mul(A, B, resulting_matrix);

    // mat4_mul(mesh->model_translation, mesh->model_rotation, resulting_matrix);
    // mat4_mul(resulting_matrix, mesh->model_scale, resulting_matrix);
    // mat4_mul(cam.view, resulting_matrix, resulting_matrix);
    // mat4_mul(cam.proj, resulting_matrix, resulting_matrix);

    glm_mul(mesh->model_translation, mesh->model_rotation, resulting_matrix);
    glm_mul(resulting_matrix, mesh->model_scale, resulting_matrix);
    glm_mul(cam.view, resulting_matrix, resulting_matrix);
    glm_mul(cam.proj, resulting_matrix, resulting_matrix);

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++)
            printf("%f ", resulting_matrix[i][j]);
        puts("");
    }


    puts("");
    //exit(0);

    glUniformMatrix4fv(
        glGetUniformLocation(mesh->program, "trans"),
        1, GL_FALSE, (const float*)resulting_matrix
    );
}

void mesh_update(struct Mesh *mesh, uint32_t vertcount, struct Camera cam) {
    mesh_draw(*mesh, vertcount);
    mesh_matrices_update(mesh, cam);
}
