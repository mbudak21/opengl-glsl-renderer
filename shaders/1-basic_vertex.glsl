// Simple vertex shader treating vertex normals as RGB colors
#version 420 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;

uniform mat4 M;
uniform mat4 V;
uniform mat4 P;
out vec3 fragPos;
out vec3 fragNormal;


void main(void)
{
    fragNormal = normalize(mat3(transpose(inverse(M))) * vNorm);;
    gl_Position = P * V * M * vec4(vPos, 1.0);
}