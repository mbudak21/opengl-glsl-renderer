#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include "locCache.h"

class Light {
private:
    static std::vector<Light*> lights;

public:
    glm::vec3 pos;
    glm::vec3 amb;
    glm::vec3 diff;
    glm::vec3 spec;
    glm::vec3 dir;
    float cutOff;
    float constant;
    float linear;
    float quadratic;

    Light();
    Light(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, const glm::vec3& dir);
    Light(
        const glm::vec3& position, 
        const glm::vec3& ambient, 
        const glm::vec3& diffuse, 
        const glm::vec3& specular, 
        const glm::vec3& direction,
        float cutOffAngle,
        float constantAtt,
        float linearAtt,
        float quadraticAtt
    );

    // glm::vec3 getPos() const;
    // glm::vec3 getAmb() const;
    // glm::vec3 getDiff() const;
    // glm::vec3 getSpec() const;

    static const std::vector<Light*>& getAllLights();
    static void clearLights();
    static void sendLightsToShader(GLuint programID);
    void setPosition(glm::vec3 pos);
};

#endif
