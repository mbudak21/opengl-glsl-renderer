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

    vec3 N;
    vec3 tangentNormal = texture(material.normalMap, fragTexCoord).rgb;
    if (length(tangentNormal) < 0.01) { // The length should be 1 anyway, this makes sure the values are not 0,0,0 or nan.
        N = normalize(fragNormal); // can remove norm
    }
    else{
        N = normalize(TBN * (tangentNormal * 2.0 - 1.0));
    }

    vec3 baseAmbient  = texture(material.ambientMap, fragTexCoord).rgb;
    vec3 baseDiffuse = texture(material.diffuseMap, fragTexCoord).rgb;
    vec3 baseSpecular = texture(material.specularMap, fragTexCoord).rgb;
    vec3 baseEmissive = texture(material.emissiveMap, fragTexCoord).rgb;

    vec3 ambient  = vec3(0.0);
    vec3 diffuse  = vec3(0.0);
    vec3 specular = vec3(0.0);
    float totalDiff = 0.0f;

    for (int i = 0; i < numLights; ++i) {
        vec3 FL = lights[i].position - fragPos;
        vec3 L = normalize(FL);
        vec3 R = normalize(reflect(-L, N));

        float theta = dot(-L, normalize(lights[i].dir));
        float epsilon = 0.55; // smooth edge
        float intensity = smoothstep(cos(lights[i].cutOff + epsilon), cos(lights[i].cutOff), theta);

        float distance = length(FL);
        float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * distance * distance);

        float diff = max(dot(N, L), 0.0);
        totalDiff += diff;
        float spec = pow(max(dot(R, ViewDir), 0.0), material.shininess);

        ambient += lights[i].ambient * baseAmbient * attenuation * intensity;
        diffuse += diff * lights[i].diffuse * baseDiffuse * attenuation * intensity;        
        specular += spec * lights[i].specular * baseSpecular * attenuation * intensity;
    }

    float shadingVal;
    if (totalDiff < 0.2) shadingVal =  0.2;
    else if (totalDiff < 0.4) shadingVal = 0.4;
    else if (totalDiff < 0.6) shadingVal = 0.6;
    else if (totalDiff < 0.8) shadingVal = 0.8;
    else shadingVal = 1.0;

    // fColor = vec4(1.0, 1.0, 1.0, 1.0);
    // fColor = vec4(ambient + diffuse + specular, 1.0);
    fColor = vec4((ambient + diffuse) * shadingVal, 1.0);
}
