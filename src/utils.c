#include "utils.h"

extern int g_frames;
extern int g_old_time;

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

void print_openglerror() {
    printf("OpenGL Error: 0x%x\n", glGetError());
}

float deg2rad(float deg) {
    return 0.01745329251 * deg;
}
