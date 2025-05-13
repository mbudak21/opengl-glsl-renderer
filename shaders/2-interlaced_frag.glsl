#version 420 core

in vec4 fragColor;
in vec4 fragPos;
out vec4 fColor;

float rand(vec2 co){
    return fract(sin(dot(co, vec2(12.9898, 78.233))) * 43758.5453);
}

void main(void)
{
	int y = int(gl_FragCoord.y)/2;
	
	if((y) % 2 == 0)
		fColor = fragColor + vec4(1,0,0,1);
	else
		fColor = vec4(0,0,0,0);
}