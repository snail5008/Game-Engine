#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "camera.h"

struct Window {
    GLFWwindow *window;

    int width;
    int height;
    int width_old;
    int height_old;

    double mouse_x;
    double mouse_y;
};

struct Window window_create();

void window_update(struct Window *win, struct Camera *cam);

void window_swapbuffers(struct Window win);

void window_clear(int r, int g, int b);

int window_should_close(struct Window win);

bool window_keypressed(struct Window win, int key);
