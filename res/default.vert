#version 330 core
layout (location = 0) in vec3 vertex_position;
uniform mat4 trans;

void main() {
    gl_Position = trans * vec4(vertex_position, 1.);
}
