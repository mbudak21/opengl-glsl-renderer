#include "material.h"

Material::Material(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, float shine)
    : amb(ambient), diff(diffuse), spec(specular), shininess(shine) {}
glm::vec3 Material::getAmb() { return amb; }
glm::vec3 Material::getDiff() { return diff; }
glm::vec3 Material::getSpec() { return spec; }
float Material::getShininess() { return shininess; }