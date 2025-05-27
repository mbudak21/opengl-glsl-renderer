#version 420 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;
layout(location = 2) in vec2 vTexCoord;
layout(location = 3) in vec3 vTangent;

uniform mat4 M;
uniform mat4 PVM;
// uniform mat4 PV;
// uniform mat4 P;
// uniform mat4 V;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;
out mat3 TBN;

void main() {
    vec3 N = normalize(mat3(M) * vNorm);
    vec3 T = normalize(mat3(M) * vTangent);
    vec3 B = cross(N, T);

    TBN = mat3(T, B, N);


    vec4 worldPos = M * vec4(vPos, 1.0);
    fragPos = vec3(worldPos);
    fragNormal = normalize(mat3(transpose(inverse(M))) * vNorm);
    fragTexCoord = vTexCoord;
    gl_Position = PVM * vec4(vPos, 1.0);
}
