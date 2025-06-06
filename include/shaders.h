#ifndef SHADERS_H
#define SHADERS_H

#include <string>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class ShaderProgram{
public:
	std::string name;
	GLuint programID;
	std::string vertexSource;
	std::string fragmentSource;
	std::string geometrySource;
};

class ShaderManager{
private:
	static std::string loadShaderFromFile(const std::string& filename);
	static void printShaderErrorInfo(GLuint shaderProgram);
	static int currentShaderIndex;
	
public:
	static bool perlinShaderUsed;

	static std::vector<ShaderProgram> shaderArr;

	static GLuint compileShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "");
	
	static void initShaders();
	static void destroyAllShaders();

	static GLuint getCurrentShader();
	static void useNextShader();

	// TODO: Use enums
	static GLuint getCubemapShader(){
		return shaderArr[2].programID;
	}
	static GLuint getPerlinShader(){
		perlinShaderUsed = true;
		return shaderArr[3].programID;
	}
};

void setMatrix4fv(GLuint programID, const std::string &name, glm::mat4 matrix);
void setVector3fv(GLuint programID, const std::string &name, glm::vec3 vector);
void setFloat1f(GLuint programID, const std::string &name, float value);


#endif