#include <stdbool.h>
#include "window.h"

#include <cglm/cglm.h>

int g_old_time = 0;
int g_frames = 0;

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

void window_swapbuffers(struct Window *win) {
    glfwSwapBuffers(win->window);
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
