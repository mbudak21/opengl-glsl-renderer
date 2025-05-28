#version 420 core

struct Material {
    sampler2D diffuseMap;
    sampler2D ambientMap;
    sampler2D specularMap;
    sampler2D emissiveMap;
    sampler2D normalMap;
    float shininess;
}; uniform Material material;

struct Light {
    vec3 position;
    vec3 dir;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float cutOff;
    // float outerCutOff;
    float constant;
    float linear;
    float quadratic;
};
#define MAX_LIGHTS 32

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;
in mat3 TBN;
uniform vec3 camPos;
out vec4 fColor;

void main() {
    vec3 ViewDir = normalize(camPos - fragPos);
    // vec3 N = normalize(texture(material.normalMap, fragTexCoord).rgb);
    // if (any(isnan(N))) {
    //     N = normalize(fragNormal); // can remove norm
    // }
    vec3 N = normalize(fragNormal);

    // vec3 baseAmbient  = texture(material.ambientMap, fragTexCoord).rgb;
    vec3 baseDiffuse = texture(material.diffuseMap, fragTexCoord).rgb;
    // vec3 baseSpecular = texture(material.specularMap, fragTexCoord).rgb;
    // vec3 baseEmissive = texture(material.emissiveMap, fragTexCoord).rgb;

    fColor = vec4(baseDiffuse*0.1, 1.0);
}