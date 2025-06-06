// #version 420 core

// in vec3 fPos;
// in vec2 fragTexCoord;
// in vec3 fragNormal;
// in vec3 faceNormal;

// uniform vec3 camPos;
// uniform float shininess;

// uniform sampler2D diffuseMap;
// uniform sampler2D ambientMap;
// uniform sampler2D specularMap;
// uniform sampler2D emissiveMap;

// uniform bool useDiffuseMap;
// uniform bool useAmbientMap;
// uniform bool useSpecularMap;
// uniform bool useEmissiveMap;

// struct Light {
//     vec3 position;
//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
// };
// #define MAX_LIGHTS 8
// uniform Light lights[MAX_LIGHTS];
// uniform int numLights;

// out vec4 fColor;

// void main() {
//     vec3 N = normalize(faceNormal);
//     vec3 ViewDir = normalize(camPos - fragPos);

//     vec3 baseDiffuse = true ? texture(diffuseMap, fragTexCoord).rgb : vec3(0.0);
//     vec3 baseAmbient  = useAmbientMap  ? texture(ambientMap, fragTexCoord).rgb  : vec3(0.0);
//     vec3 baseSpecular = useSpecularMap ? texture(specularMap, fragTexCoord).rgb : vec3(0.0);
//     vec3 baseEmissive = useEmissiveMap ? texture(emissiveMap, fragTexCoord).rgb : vec3(0.0);

//     vec3 ambient  = vec3(0.0);
//     vec3 diffuse  = vec3(0.0);
//     vec3 specular = vec3(0.0);

//     for (int i = 0; i < numLights; ++i) {
//         vec3 L = normalize(lights[i].position - fragPos);
//         vec3 R = normalize(reflect(-L, N));

//         float diff = max(dot(N, L), 0.0);
//         float spec = pow(max(dot(R, ViewDir), 0.0), shininess);

//         ambient += baseAmbient * lights[i].ambient;
//         diffuse += diff * baseDiffuse * lights[i].diffuse;
//         specular += spec * baseSpecular * lights[i].specular;
//     }

//     fColor = vec4(ambient + diffuse + specular, 1.0);
// }
