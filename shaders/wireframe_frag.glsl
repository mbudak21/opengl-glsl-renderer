#version 410 core

in vec3 edgeDist;
out vec4 fragColor;

uniform vec3 fillColor = vec3(0.9); // face col
uniform vec3 wireColor = vec3(0.0); // edge col
uniform float thickness = 0.3;

float edgeFactor() {
    vec3 d = fwidth(edgeDist);
    vec3 a3 = smoothstep(vec3(0.0), d * thickness, edgeDist);
    return min(min(a3.x, a3.y), a3.z);
}

void main() {
    float e = edgeFactor();
    fragColor = vec4(mix(wireColor, fillColor, e), 1.0);
}
