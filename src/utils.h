#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

int fps();

char *file_read(const char *fname);

void print_openglerror();

float deg2rad(float deg);
