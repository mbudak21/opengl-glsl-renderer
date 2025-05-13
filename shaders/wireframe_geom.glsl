#version 410 core

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

out vec3 edgeDist;

void main() {
    vec3 p0 = gl_in[0].gl_Position.xyz / gl_in[0].gl_Position.w; // convert to NDC
    vec3 p1 = gl_in[1].gl_Position.xyz / gl_in[1].gl_Position.w;
    vec3 p2 = gl_in[2].gl_Position.xyz / gl_in[2].gl_Position.w;

    // Edge lengths
    float l0 = length(p1 - p2);
    float l1 = length(p2 - p0);
    float l2 = length(p0 - p1);

    edgeDist = vec3(1, 0, 0);
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    edgeDist = vec3(0, 1, 0);
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    edgeDist = vec3(0, 0, 1);
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();
}
