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
#define MAX_LIGHTS 128

uniform Light lights[MAX_LIGHTS];
uniform int numLights;

in vec3 gFragPos;
in vec3 gFragNormal;
in vec2 gFragTexCoord;
in mat3 gTBN;
in float heightOffset;

uniform vec3 camPos;
out vec4 fColor;

void main() {
    vec3 ViewDir = normalize(camPos - gFragPos);

    vec3 N;
    vec3 tangentNormal = texture(material.normalMap, gFragTexCoord).rgb;
    if (length(tangentNormal) < 0.01) { // The length should be 1 anyway, this makes sure the values are not 0,0,0 or nan.
        N = normalize(gFragNormal); // can remove norm
    }
    else{
        N = normalize(gTBN * (tangentNormal * 2.0 - 1.0));
    }

    vec3 baseAmbient  = texture(material.ambientMap, gFragTexCoord).rgb;
    vec3 baseDiffuse = texture(material.diffuseMap, gFragTexCoord).rgb;
    vec3 baseSpecular = texture(material.specularMap, gFragTexCoord).rgb;
    vec3 baseEmissive = texture(material.emissiveMap, gFragTexCoord).rgb;

    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);

    for (int i = 0; i < numLights; ++i) {
        vec3 FL = lights[i].position - gFragPos;
        vec3 L = normalize(FL);
        vec3 R = normalize(reflect(-L, N));

        float theta = dot(-L, normalize(lights[i].dir));
        float epsilon = 0.55; // smooth edge
        float intensity = smoothstep(cos(lights[i].cutOff + epsilon), cos(lights[i].cutOff), theta);

        float distance = length(FL);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        float dif = max(dot(N, L), 0.0);
        float spec = pow(max(dot(R, ViewDir), 0.0), material.shininess);

        ambient += lights[i].ambient * baseAmbient * attenuation * intensity;
        diffuse += dif * lights[i].diffuse * baseDiffuse * attenuation * intensity;        
        specular += spec * lights[i].specular * baseSpecular * attenuation * intensity;
    }

    fColor = vec4(ambient + diffuse + specular, 1.0);
    // if (!isnan(heightOffset)) {
    //     fColor = vec4(vec3(heightOffset), 1.0);
    // } else {
    //     fColor = vec4(1.0, 0.0, 1.0, 1.0);
    // }
    // fColor = vec4(vec3(attenuation), 1.0); // grayscale based on distance
}