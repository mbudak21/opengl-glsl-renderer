#include "lights.h"

std::vector<Light*> Light::lights;

Light::Light(): pos(0.0f), amb(1.0f), diff(1.0f), spec(1.0f) {
    lights.push_back(this);
}

Light::Light(const glm::vec3& position, const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction)
    : pos(position), amb(ambient), diff(diffuse), spec(specular), dir(direction) {
    // Default values for spotlight parameters
    cutOff = glm::radians(45.f); // 45 degrees
    constant = 1.0f;
    linear = 0.7f;
    quadratic = 1.8;
    lights.push_back(this);
}
Light::Light(
    const glm::vec3& position, 
    const glm::vec3& ambient, 
    const glm::vec3& diffuse, 
    const glm::vec3& specular, 
    const glm::vec3& direction,
    float cutOffAngle,
    float constantAtt,
    float linearAtt,
    float quadraticAtt) :
    pos(position), 
    amb(ambient), 
    diff(diffuse), 
    spec(specular), 
    dir(direction),
    cutOff(glm::radians(cutOffAngle)),
    constant(constantAtt), 
    linear(linearAtt),
    quadratic(quadraticAtt) {
        lights.push_back(this);
    }

const std::vector<Light*>& Light::getAllLights() {
    return lights;
}

void Light::clearLights() {
    lights.clear();
}

void Light::sendLightsToShader(GLuint programID) {
    glUseProgram(programID);
    
    const auto& lights = Light::getAllLights();
    int numLights = static_cast<int>(lights.size());
    printf("Sending %i lights to shader ID: %i\n", numLights, programID);
    // Send number of lights
    GLint loc = glGetUniformLocation(programID, "numLights");
    glUniform1i(loc, numLights);

    for (int i = 0; i < numLights; ++i) {
        Light* light = lights[i];
        std::string prefix = "lights[" + std::to_string(i) + "].";

        GLuint posLoc = locCache::getUniformLoc(programID, (prefix + "position").c_str());
        GLuint ambLoc = locCache::getUniformLoc(programID, (prefix + "ambient").c_str());
        GLuint diffLoc = locCache::getUniformLoc(programID, (prefix + "diffuse").c_str());
        GLuint specLoc = locCache::getUniformLoc(programID, (prefix + "specular").c_str());
        GLuint dirLoc = locCache::getUniformLoc(programID, (prefix + "dir").c_str());
        GLuint cutOffLoc = locCache::getUniformLoc(programID, (prefix + "cutOff").c_str());
        GLuint constantLoc = locCache::getUniformLoc(programID, (prefix + "constant").c_str());
        GLuint linearLoc = locCache::getUniformLoc(programID, (prefix + "linear").c_str());
        GLuint quadraticLoc = locCache::getUniformLoc(programID, (prefix + "quadratic").c_str());

        // printf("=== Light Uniform Upload ===\n");
        // printf("Locations:\n");
        // printf("  Position:   %d\n", posLoc);
        // printf("  Ambient:    %d\n", ambLoc);
        // printf("  Diffuse:    %d\n", diffLoc);
        // printf("  Specular:   %d\n", specLoc);
        // printf("  Direction:  %d\n", dirLoc);
        // printf("  CutOff:     %d\n", cutOffLoc);
        // printf("  Constant:   %d\n", constantLoc);
        // printf("  Linear:     %d\n", linearLoc);
        // printf("  Quadratic:  %d\n", quadraticLoc);

        // printf("Values:\n");
        // printf("  Position:   (%.2f, %.2f, %.2f)\n", light->pos.x, light->pos.y, light->pos.z);
        // printf("  Ambient:    (%.2f, %.2f, %.2f)\n", light->amb.x, light->amb.y, light->amb.z);
        // printf("  Diffuse:    (%.2f, %.2f, %.2f)\n", light->diff.x, light->diff.y, light->diff.z);
        // printf("  Specular:   (%.2f, %.2f, %.2f)\n", light->spec.x, light->spec.y, light->spec.z);
        // printf("  Direction:  (%.2f, %.2f, %.2f)\n", light->dir.x, light->dir.y, light->dir.z);
        // printf("  CutOff Cos: %.4f\n", glm::cos(light->cutOff));
        // printf("  Constant:   %.2f\n", light->constant);
        // printf("  Linear:     %.2f\n", light->linear);
        // printf("  Quadratic:  %.2f\n", light->quadratic);

        glUniform3fv(posLoc, 1, glm::value_ptr(light->pos));
        glUniform3fv(ambLoc, 1, glm::value_ptr(light->amb));
        glUniform3fv(diffLoc, 1, glm::value_ptr(light->diff));
        glUniform3fv(specLoc, 1, glm::value_ptr(light->spec));
        glUniform3fv(dirLoc, 1, glm::value_ptr(light->dir));
        glUniform1f(cutOffLoc, glm::cos(light->cutOff));
        glUniform1f(constantLoc, light->constant);
        glUniform1f(linearLoc, light->linear);
        glUniform1f(quadraticLoc, light->quadratic);
    }
}

void Light::setPosition(glm::vec3 pos){
    this->pos = pos;
    assert(this->pos.x == pos.x && this->pos.y == pos.y && this->pos.z == pos.z);
}

// struct Light {
//     vec3 position;
//     vec3 dir;
//     float cutOff;
//     float outerCutOff;

//     vec3 ambient;
//     vec3 diffuse;
//     vec3 specular;
    
//     float constant;
//     float linear;
//     float quadratic;
// };