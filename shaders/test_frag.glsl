#version 410 core

in vec3 fragPos;
// in vec3 fragNormal;
in vec2 fragTexCoord;

// uniform mat4 P;
// uniform mat4 V;
uniform vec3 camPos;
// uniform mat4 M;
// uniform vec3 matAmb;
// uniform vec3 matDiff;
// uniform vec3 matSpec;
// vec3 matAmb = vec3(0.1);
// vec3 matDiff = vec3(0.2);
// vec3 matSpec = vec3(0.3);

uniform float matShininess;

uniform sampler2D diffuseMap;
uniform bool useDiffuseMap;

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
    // vec3 N = normalize(fragNormal);
    vec3 ViewDir = normalize(camPos - fragPos);

    // sample your diffuse texture (or use matDiff if you don't)
    vec3 baseColor = useDiffuseMap
        ? texture(diffuseMap, fragTexCoord).rgb
        : vec3(1.f, 1.f, 0.f);
    fColor = vec4(baseColor, 1.0f);

    // vec3 ambient  = vec3(0.0);
    // vec3 diffuse  = vec3(0.0);
    // vec3 specular = vec3(0.0);

    // for (int i = 0; i < numLights; ++i) {
    //     vec3 L = normalize(lights[i].position - fragPos);
    //     vec3 R = normalize(reflect(-L, N));

    //     ambient += lights[i].ambient;

    //     float dif = max(dot(N, L), 0.0);
    //     diffuse += dif * lights[i].diffuse * baseColor;

    //     float spec = pow(max(dot(R, ViewDir), 0.0), 64);
    //     specular += spec * lights[i].specular;
    // }

    // fColor = vec4(ambient + diffuse + specular, 1.0);
}
