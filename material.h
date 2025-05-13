#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <glm/glm.hpp>

class Material {

private:
    glm::vec3 amb;
    glm::vec3 diff;
    glm::vec3 spec;
    float shininess;
    
public:
    Material();
    Material(const glm::vec3& amb, const glm::vec3& diff, const glm::vec3& spec, float shine);
    glm::vec3 getAmb();
    glm::vec3 getDiff();
    glm::vec3 getSpec();
    float getShininess();
    
    static Material White() {
        return Material(
            glm::vec3(1.0f, 1.0f, 1.0f),   // ambient
            glm::vec3(1.0f, 1.0f, 1.0f), // diffuse
            glm::vec3(1.0f, 1.0f, 1.0f),    // specular
            0.0f                                    // shininess
        );
    }

    static Material Black() {
        return Material(
            glm::vec3(0.0f, 0.0f, 0.0f),   // ambient
            glm::vec3(0.0f, 0.0f, 0.0f), // diffuse
            glm::vec3(0.0f, 0.0f, 0.0f),    // specular
            0.0f                                    // shininess
        );
    }

    static Material Red() {
        return Material(
            glm::vec3(1.0f, 0.0f, 0.0f),   // ambient
            glm::vec3(1.0f, 0.0f, 0.0f), // diffuse
            glm::vec3(1.0f, 0.0f, 0.0f),    // specular
            0.0f                                    // shininess
        );
    }

    static Material Green() {
        return Material(
            glm::vec3(0.0f, 1.0f, 0.0f),   // ambient
            glm::vec3(0.0f, 1.0f, 0.0f), // diffuse
            glm::vec3(0.0f, 1.0f, 0.0f),    // specular
            0.0f                                    // shininess
        );
    }

    static Material Blue() {
        return Material(
            glm::vec3(0.0f, 0.0f, 1.0f),   // ambient
            glm::vec3(0.0f, 0.0f, 1.0f), // diffuse
            glm::vec3(0.0f, 0.0f, 1.0f),    // specular
            0.0f                                    // shininess
        );
    }

    static Material RedRealistic() {
        return Material(
            glm::vec3(0.15f, 0.0f, 0.0f),   // ambient
            glm::vec3(0.7f, 0.0f, 0.0f), // diffuse
            glm::vec3(0.3f, 0.3f, 0.3f),    // specular
            0.2f                                    // shininess
        );
    }

    static Material GreenRealistic() {
        return Material(
            glm::vec3(0.0f, 0.15f, 0.0f),   // ambient
            glm::vec3(0.0f, 0.7f, 0.0f), // diffuse
            glm::vec3(0.3f, 0.3f, 0.3f),    // specular
            0.7f                                    // shininess
        );
    }

    static Material BlueRealistic() {
        return Material(
            glm::vec3(0.0f, 0.0f, 0.15f),   // ambient
            glm::vec3(0.0f, 0.0f, 0.7f), // diffuse
            glm::vec3(0.3f, 0.3f, 0.3f),    // specular
            0.3f                                    // shininess
        );
    }

    static Material Gold() {
        return Material(
            glm::vec3(0.24725f, 0.1995f, 0.0745f),   // ambient
            glm::vec3(0.75164f, 0.60648f, 0.22648f), // diffuse
            glm::vec3(0.628281f, 0.555802f, 0.366065f),    // specular
            0.4f                                    // shininess
        );
    }

    static Material Copper() {
        return Material(
            glm::vec3(0.19125f, 0.0735f, 0.0225f),   // ambient
            glm::vec3(0.7038f, 0.27048f, 0.0828f), // diffuse
            glm::vec3(0.256777f, 0.137622f, 0.086014f),    // specular
            0.1f                                    // shininess
        );
    }

    static Material Silver() {
        return Material(
            glm::vec3(0.19225f, 0.19225f, 0.19225f),   // ambient
            glm::vec3(0.50754f, 0.50754f, 0.50754f), // diffuse
            glm::vec3(0.508273f, 0.508273f, 0.508273f),    // specular
            0.4f                                    // shininess
        );
    }

    static Material Emerald() {
        return Material(
            glm::vec3(0.0215f, 0.1745f, 0.0215f),   // ambient
            glm::vec3(0.07568f, 0.61424f, 0.07568f), // diffuse
            glm::vec3(0.633f, 0.727811f, 0.633f),    // specular
            0.6f                                    // shininess
        );
    }

    static Material Jade() {
        return Material(
            glm::vec3(0.135f, 0.2225f, 0.1575f),   // ambient
            glm::vec3(0.54f, 0.89f, 0.63f), // diffuse
            glm::vec3(0.316228f, 0.316228f, 0.316228f),    // specular
            0.1f                                    // shininess
        );
    }

    static Material Obsidian() {
        return Material(
            glm::vec3(0.05375f, 0.05f, 0.06625f),   // ambient
            glm::vec3(0.18275f, 0.17f, 0.22525f), // diffuse
            glm::vec3(0.332741f, 0.328634f, 0.346435f),    // specular
            0.3f                                    // shininess
        );
    }

    static Material Pearl() {
        return Material(
            glm::vec3(0.25f, 0.20725f, 0.20725f),   // ambient
            glm::vec3(1.0f, 0.829f, 0.829f), // diffuse
            glm::vec3(0.296648f, 0.296648f, 0.296648f),    // specular
            0.088f                                    // shininess
        );
    }

    static Material Ruby() {
        return Material(
            glm::vec3(0.1745f, 0.01175f, 0.01175f),   // ambient
            glm::vec3(0.61424f, 0.04136f, 0.04136f), // diffuse
            glm::vec3(0.727811f, 0.626959f, 0.626959f),    // specular
            0.6f                                    // shininess
        );
    }

    static Material Turquoise() {
        return Material(
            glm::vec3(0.1f, 0.18725f, 0.1745f),   // ambient
            glm::vec3(0.396f, 0.74151f, 0.69102f), // diffuse
            glm::vec3(0.297254f, 0.30829f, 0.306678f),    // specular
            0.1f                                    // shininess
        );
    }

    static Material Brass() {
        return Material(
            glm::vec3(0.329412f, 0.223529f, 0.027451f),   // ambient
            glm::vec3(0.780392f, 0.568627f, 0.113725f), // diffuse
            glm::vec3(0.992157f, 0.941176f, 0.807843f),    // specular
            0.21794872f                                    // shininess
        );
    }

    static Material Bronze() {
        return Material(
            glm::vec3(0.2125f, 0.1275f, 0.054f),   // ambient
            glm::vec3(0.714f, 0.4284f, 0.18144f), // diffuse
            glm::vec3(0.393548f, 0.271906f, 0.166721f),    // specular
            0.2f                                    // shininess
        );
    }

    static Material Chrome() {
        return Material(
            glm::vec3(0.25f, 0.25f, 0.25f),   // ambient
            glm::vec3(0.4f, 0.4f, 0.4f), // diffuse
            glm::vec3(0.774597f, 0.774597f, 0.774597f),    // specular
            0.6f                                    // shininess
        );
    }
    static Material RedRubber() {
        return Material(
            glm::vec3(0.05f, 0.0f, 0.0f),   // ambient
            glm::vec3(0.6f, 0.4f, 0.4f), // diffuse
            glm::vec3(0.7f, 0.04f, 0.04f),    // specular
            0.078125f                                    // shininess
        );
    }
};



#endif