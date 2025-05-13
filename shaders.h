#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

struct ShaderProgram {
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
	GLuint programID;
	std::string name;
};

void setMatrix4fv(GLuint programID, const std::string &name, glm::mat4 matrix);
void setVector3fv(GLuint programID, const std::string &name, glm::vec3 vector);
void setFloat1f(GLuint programID, const std::string &name, float value);


std::string loadShaderFromFile(const std::string& filename);
GLuint compileShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "");
void printShaderErrorInfo(GLuint shaderProgram);

void initShaders();
void useNextShader();
GLuint getCurrentShader();
void destroyAllShaders();




#endif