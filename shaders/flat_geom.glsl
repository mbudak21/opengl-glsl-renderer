#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in vec3 worldPos[];
flat out vec3 faceNormal;
out vec3 fragPos;

void main() {
    vec3 A = worldPos[0];
    vec3 B = worldPos[1];
    vec3 C = worldPos[2];

    vec3 normal = normalize(cross(B - A, C - A));

    for (int i = 0; i < 3; ++i) {
        fragPos = worldPos[i];
        faceNormal = normal;
        gl_Position = gl_in[i].gl_Position;
        EmitVertex();
    }
    EndPrimitive();
}
