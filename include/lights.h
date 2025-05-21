#ifndef _LIGHTS_H_
#define _LIGHTS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>

class Light {
private:
    static std::vector<Light> lights;

    glm::vec3 pos;
    glm::vec3 amb;
    glm::vec3 diff;
    glm::vec3 spec;

public:
    Light();
    Light(const glm::vec3& pos, const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec);

    glm::vec3 getPos() const;
    glm::vec3 getAmb() const;
    glm::vec3 getDiff() const;
    glm::vec3 getSpec() const;

    static const std::vector<Light>& getAllLights();
    static void clearLights();
    static void sendLightsToShader(GLuint programID);
};

#endif
