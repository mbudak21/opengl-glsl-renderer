// Simple fragment shader for color interpolation
#version 420 core

in vec3 fragPos;
in vec3 fragNormal;
out vec4 fColor;

void main(void)
{
    fColor = vec4(normalize(fragNormal), 0);
    // fColor = vec4(fragNormal, 1.0);
    // fColor = vec4(1.0, 1.0, 1.0, 1.0);
}