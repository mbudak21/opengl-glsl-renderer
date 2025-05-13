#version 410 core

layout(location = 0) in vec3 vPos;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec3 worldPos;

void main() {
    vec4 wp = M * vec4(vPos, 1.0);
    worldPos = vec3(wp);
    gl_Position = P * V * wp;
}
