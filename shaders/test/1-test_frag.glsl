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

    vec3 baseAmbient  = texture(material.ambientMap, fragTexCoord).rgb;
    vec3 baseDiffuse = texture(material.diffuseMap, fragTexCoord).rgb;
    vec3 baseSpecular = texture(material.specularMap, fragTexCoord).rgb;
    vec3 baseEmissive = texture(material.emissiveMap, fragTexCoord).rgb;

    // vec3 ambient  = vec3(0.0);
    // vec3 diffuse  = vec3(0.0);
    // vec3 specular = vec3(0.0);

    // for (int i = 0; i < numLights; ++i) {
    //     vec3 L = normalize(lights[i].position - fragPos);
    //     vec3 R = normalize(reflect(-L, N));

    //     float dif = max(dot(N, L), 0.0);
    //     float spec = pow(max(dot(R, ViewDir), 0.0), material.shininess);

    //     ambient += lights[i].ambient * baseAmbient;
    //     diffuse += dif * lights[i].diffuse * baseDiffuse;        
    //     specular += spec * lights[i].specular * baseSpecular;
    // }

    fColor = vec4(baseAmbient + baseDiffuse + baseSpecular, 1.0);
}