#version 410 core

in vec3 fragPos;
in vec3 fragNormal;

uniform mat4 V;
uniform mat4 P;
uniform vec3 camPos;
uniform mat4 M;
uniform vec3 matAmb;
uniform vec3 matDiff;
uniform vec3 matSpec;
uniform float matShininess;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_LIGHTS 8
uniform Light lights[MAX_LIGHTS];
uniform int numLights;

out vec4 fColor;

void main() {
    vec3 N = normalize(fragNormal);
    vec3 V = normalize(camPos - fragPos);

    vec3 ambient = vec3(0.0);
    vec3 diffuse = vec3(0.0);
    vec3 specular = vec3(0.0);
    float diff = 0;

    for (int i = 0; i < numLights; ++i) {
        vec3 L = normalize(lights[i].position - fragPos);
        vec3 R = normalize(reflect(-L, N));

        ambient += matAmb * lights[i].ambient;

        diff += max(dot(N, L), 0.0);
        diffuse += diff * matDiff * lights[i].diffuse;

        float spec = pow(max(dot(R, V), 0.0), matShininess*128);
        specular += spec * matSpec * lights[i].specular;
    }

    float shadingVal;
    if (diff < 0.2)
        shadingVal =  0.2;
    else if (diff < 0.4)
        shadingVal = 0.4;
    else if (diff < 0.6)
        shadingVal = 0.6;
    else if (diff < 0.8)
        shadingVal = 0.8;
    else
        shadingVal = 1.0;

    // fColor = vec4(1.0, 1.0, 1.0, 1.0);
    // fColor = vec4(ambient + diffuse + specular, 1.0);
    fColor = vec4((ambient + diffuse) * shadingVal, 1.0);
}
