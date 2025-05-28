#version 420 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTangent;

// uniform mat4 P;
uniform mat4 M;
// uniform mat4 PV;
uniform mat4 PVM;

void main() {
    gl_Position = PVM * vec4(vPos, 1.0);
}
