#version 410 core

layout(location = 0) in vec3 vPos;
uniform mat4 M, V, P;

void main() {
    gl_Position = P * V * M * vec4(vPos, 1.0);
}
