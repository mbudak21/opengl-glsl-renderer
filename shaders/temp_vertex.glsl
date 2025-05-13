#version 410 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

flat out vec3 faceNormal;
out vec3 fragPos;

void main() {
    fragPos = vec3(M * vec4(vPos, 1.0));
    faceNormal = normalize(mat3(transpose(inverse(M))) * vNorm);
    gl_Position = P * V * vec4(fragPos, 1.0);
}
