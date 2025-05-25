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
	static std::vector<ShaderProgram> shaderArr;

	static GLuint compileShaderProgram(const std::string& vertexSrc, const std::string& fragmentSrc, const std::string& geometrySrc = "");
	
	static void initShaders();
	static void destroyAllShaders();

	static GLuint getCurrentShader();
	static void useNextShader();

	// temp test flat basic interlaced phong cartoon wireframe
	static GLuint getTempShader(){
		return shaderArr[0].programID;
	}
	static GLuint getFlatShader(){
		return shaderArr[1].programID;
	}
	static GLuint getBasicShader(){
		return shaderArr[2].programID;
	}
	static GLuint getInterlacedShader(){
		return shaderArr[3].programID;
	}
	static GLuint getPhongShader(){
		return shaderArr[4].programID;
	}
	static GLuint getCartoonShader(){
		return shaderArr[5].programID;
	}
	static GLuint getWireframeShader(){
		return shaderArr[6].programID;
	}
};

void setMatrix4fv(GLuint programID, const std::string &name, glm::mat4 matrix);
void setVector3fv(GLuint programID, const std::string &name, glm::vec3 vector);
void setFloat1f(GLuint programID, const std::string &name, float value);


#endif