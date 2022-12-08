#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

float square[] = {
    -1.,  1.,  0.,
     1., -1.,  0.,
    -1., -1.,  0.,
    -1.,  1.,  0.,
     1.,  1.,  0.,
     1., -1.,  0.,
};

int g_old_time = 0;
int g_frames = 0;

struct Window {
    GLFWwindow *window;

    int width;
    int height;
    int width_old;
    int height_old;

    double mouse_x;
    double mouse_y;
};

struct Mesh {
    uint32_t vao;
    uint32_t vbo;
    uint32_t program;

    mat4 model_translation;
    mat4 model_scale;
    mat4 model_rotation;
};

struct Camera {
    mat4 view;
    mat4 proj;

    vec3 position;
    vec3 direction;
};

char *file_read(const char *fname) {
    FILE *f = fopen(fname, "r");
    if (!f) {
        printf("Could not open file '%s'\n", fname);
        return NULL;
    }

    int i = 0;
    int current_size = 32;
    char *contents = malloc(current_size);
    char c;

    while ((c = getc(f)) != EOF) {
        if (i >= current_size - 2) {
            current_size *= 2;
            contents = realloc(contents, current_size);
        }
        contents[i++] = c;
    }
    contents[i] = 0;

    fclose(f);
    return contents;
}

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
   glm_mat4_identity(mesh->model_rotation);
   glm_mat4_identity(mesh->model_translation);
   glm_mat4_identity(mesh->model_scale);
}

void mesh_matrices_update(struct Mesh *mesh, struct Camera cam) {

    mat4 resulting_matrix;

    glm_mul(mesh->model_translation, mesh->model_rotation, resulting_matrix);
    glm_mul(resulting_matrix, mesh->model_scale, resulting_matrix);
    glm_mul(cam.view, resulting_matrix, resulting_matrix);
    glm_mul(cam.proj, resulting_matrix, resulting_matrix);

    glUniformMatrix4fv(
        glGetUniformLocation(mesh->program, "trans"),
        1, GL_FALSE, (const float*)resulting_matrix
    );
}

void mesh_update(struct Mesh *mesh, uint32_t vertcount, struct Camera cam) {
    mesh_draw(*mesh, vertcount);
    mesh_matrices_update(mesh, cam);
}

struct Window window_create() {
    struct Window win;

    if (!glfwInit()) {
        win.window = NULL;
        return win;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    win.window = glfwCreateWindow(800, 600, "Game", NULL, NULL);
    glfwMakeContextCurrent(win.window);
    glfwSwapInterval(1);
    if (!gladLoadGL()) {
        win.window = NULL;
        return win;
    }

    g_old_time = (int)glfwGetTime();

    return win;
}

void window_update(struct Window *win, struct Camera *cam) {
    glfwGetWindowSize(win->window, &win->width, &win->height);
    glfwGetCursorPos(win->window, &win->mouse_x, &win->mouse_y);

    if (win->width_old != win->width || win->height_old != win->height) {
        glViewport(0, 0, win->width, win->height);
        glm_perspective(glm_rad(45), (float)win->width / (float)win->height, 0.1, 100, cam->proj);
    }
    win->width_old = win->width;
    win->height_old = win->height;
}

void window_swapbuffers(struct Window win) {
    glfwSwapBuffers(win.window);
    glfwPollEvents();
}

void window_clear(int r, int g, int b) {
    glClearColor(r / 255.f, g / 255.f, b / 255.f, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

int window_should_close(struct Window win) {
    return glfwWindowShouldClose(win.window);
}

bool window_keypressed(struct Window win, int key) {
    return glfwGetKey(win.window, key);
}

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

int fps() {
    int g_frames_copy;
    g_frames++;

    if (g_old_time != (int)glfwGetTime()) {
        g_frames_copy = g_frames;
        g_frames = 0;
        g_old_time = (int)glfwGetTime();
        return g_frames_copy;
    }

    else return -1;
}

float deg2rad(float deg) {
    return 0.01745329251 * deg;
}

void print_openglerror() {
    printf("OpenGL Error: 0x%x\n", glGetError());
}

// int main() {
//     struct Window window = window_create();
    
//     struct Mesh mesh;
//     mesh_create_vbo(&mesh, sizeof square, square);
//     mesh_create_default_vao(&mesh);
//     mesh_create_default_program(&mesh);
//     mesh_matrices_init(&mesh);

//     struct Camera cam = camera_create(glm_rad(45), 800, 600);

//     glfwSetInputMode(window.window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

//     vec3 direction = {0, 0, 1};
//     float speed = 0.1;
//     float sensitivity = 50;
//     while (!glfwWindowShouldClose(window.window)) {
//         // update the window
//         window_update(&window, &cam);

//         // get input
//         vec3 tmpvec;
//         glm_vec3_scale(cam.direction, speed, tmpvec);
//         if (glfwGetKey(window.window, GLFW_KEY_W))
//             glm_vec3_add(cam.position, tmpvec, cam.position);
//         else if (glfwGetKey(window.window, GLFW_KEY_S))
//             glm_vec3_sub(cam.position, tmpvec, cam.position);
        
//         glm_vec3_cross(cam.direction, (vec3) { 0, 1, 0 }, tmpvec);
//         glm_normalize(tmpvec);
//         glm_vec3_scale(tmpvec, speed, tmpvec);
//         if (glfwGetKey(window.window, GLFW_KEY_A))
//             glm_vec3_sub(cam.position, tmpvec, cam.position);
//         else if (glfwGetKey(window.window, GLFW_KEY_D))
//             glm_vec3_add(cam.position, tmpvec, cam.position);
        
//         cam.position[1] = 0;

        
//         double pitch = -window.mouse_y / window.height * sensitivity;
//         double yaw = window.mouse_x / window.width * sensitivity;
//         if (pitch >= 89) pitch = 89;
//         else if (pitch <= -89) pitch = -89;

//         direction[0] = cos(glm_rad(yaw)) * cos(glm_rad(pitch));
//         direction[1] = sin(glm_rad(pitch));
//         direction[2] = sin(glm_rad(yaw)) * cos(glm_rad(pitch));
//         glm_normalize(direction);
//         glm_vec3_copy(direction, cam.direction);

//         // clear the screen
//         window_clear(50, 50, 50);

//         // update functions
//         camera_update(&cam);

//         for (int i = 1; i <= 5; i++) {
//             mesh_update(&mesh, 6, cam);
//             glm_translate(mesh.model_translation, (vec3) { i, 0, 0 });
//         }
//         mesh_update(&mesh, 6, cam);
//         glm_translate(mesh.model_translation, (vec3) { -15, 0, 0 });

//         window_swapbuffers(window);

//         // count fps
//         int frames = fps();
//         if (frames != -1) {
//             printf("FPS: %d\n", frames);
//         }
//     }

//     printf("OpenGL Error: 0x%x\n", glGetError());

//     glfwTerminate();
//     return 0;
// }
