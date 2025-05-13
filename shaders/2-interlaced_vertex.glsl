#version 420 core

layout(location = 0) in vec3 vPos;
layout(location = 1) in vec3 vNorm;

uniform mat4 V;
uniform mat4 P;
uniform mat4 M;

out vec4 fragColor;
out vec4 fragPos;


void main(void)
{
	fragColor = vec4(vNorm, 1);
	fragPos = P * V * M * vec4(vPos, 1.0f);
	gl_Position = fragPos;
}