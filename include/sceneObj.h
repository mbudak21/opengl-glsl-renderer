#ifndef _SCENE_OBJ_H_
#define _SCENE_OBJ_H_
#include <glm/vec3.hpp>
#include "obj.h"
#include <stdio.h>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <GL/glew.h>
#include <unordered_map>
#include <string>
#include <memory>


class SceneObject{
private:
	static std::unordered_map<std::string, std::shared_ptr<obj>> objectMap;
public:
    obj* objInstance;
	GLuint shprg;
	glm::vec3 pos = {0.f, 0.f, 0.f};
	glm::vec3 rot = {0.f, 0.f, 0.f};
	glm::vec3 scale = {1.f, 1.f, 1.f};

    SceneObject(obj* o): objInstance(o){};

	SceneObject(const char* path);
	void setShader(GLuint ID);
	void prepareObject();
	void renderObject(glm::mat4 P, glm::mat4 V);
	void renderObject(GLuint shprg, glm::mat4 P, glm::mat4 V);
	
};

#endif