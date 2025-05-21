#include "lights.h"
#include <glm/gtc/type_ptr.hpp>
#include <string>

std::vector<Light> Light::lights;

Light::Light(): pos(0.0f), amb(1.0f), diff(1.0f), spec(1.0f) {
    lights.push_back(*this);
}

Light::Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular)
    : pos(position), amb(ambient), diff(diffuse), spec(specular){
    lights.push_back(*this);
}

glm::vec3 Light::getPos() const { return pos; }
glm::vec3 Light::getAmb() const { return amb; }
glm::vec3 Light::getDiff() const { return diff; }
glm::vec3 Light::getSpec() const { return spec; }

const std::vector<Light>& Light::getAllLights() {
    return lights;
}

void Light::clearLights() {
    lights.clear();
}

void Light::sendLightsToShader(GLuint programID) {
    const auto& lights = Light::getAllLights();
    int numLights = static_cast<int>(lights.size());

    // Send number of lights
    GLint loc = glGetUniformLocation(programID, "numLights");
    glUniform1i(loc, numLights);

    for (int i = 0; i < numLights; ++i) {
        const Light& light = lights[i];
        std::string prefix = "lights[" + std::to_string(i) + "].";

        glUniform3fv(glGetUniformLocation(programID, (prefix + "position").c_str()), 1, glm::value_ptr(light.getPos()));
        glUniform3fv(glGetUniformLocation(programID, (prefix + "ambient").c_str()), 1, glm::value_ptr(light.getAmb()));
        glUniform3fv(glGetUniformLocation(programID, (prefix + "diffuse").c_str()), 1, glm::value_ptr(light.getDiff()));
        glUniform3fv(glGetUniformLocation(programID, (prefix + "specular").c_str()), 1, glm::value_ptr(light.getSpec()));
    }
}