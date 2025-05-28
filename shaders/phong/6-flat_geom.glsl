// #version 410 core

// layout(triangles) in;
// layout(triangle_strip, max_vertices = 3) out;

// in vec3 fragPos[];
// flat out vec3 faceNormal;
// out vec3 fPos;

// void main() {
//     vec3 A = fragPos[0];
//     vec3 B = fragPos[1];
//     vec3 C = fragPos[2];

//     vec3 normal = normalize(cross(B - A, C - A));

//     for (int i = 0; i < 3; ++i) {
//         fPos = fragPos[i];
//         faceNormal = normal;
//         gl_Position = gl_in[i].gl_Position;
//         EmitVertex();
//     }
//     EndPrimitive();
// }
