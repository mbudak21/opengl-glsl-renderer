#include "shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "locCache.h"

using namespace std;

int ShaderManager::currentShaderIndex = 0;
extern vector<ShaderProgram> ShaderManager::shaderArr;


std::string ShaderManager::loadShaderFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        std::string fallbackPath = "." + filename;
        file.open(fallbackPath);

        if (!file.is_open()) {
            std::cerr << "Failed to open shader file: " << filename << " or " << fallbackPath << std::endl;
            return "";
        }
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLuint ShaderManager::compileShaderProgram(const std::string& vs_str, const std::string& fs_str, const std::string& geometrySrc) {
	/*
	Compiles and links the given shader.
	*/ 
	const char* vs_ptr = vs_str.c_str();
	const char* fs_ptr = fs_str.c_str();

	GLuint glProgramID = glCreateProgram();
	GLint success = GL_FALSE;

	// === Vertex Shader ===
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_ptr, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
	if (!success) {
		printf("Error in vertex shader!\n");
		printShaderErrorInfo(vs);
		glDeleteShader(vs);
		return -1;
	}

	// === Fragment Shader ===
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_ptr, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
	if (!success) {
		printf("Error in fragment shader!\n");
		printShaderErrorInfo(fs);
		glDeleteShader(fs);
		glDeleteShader(vs); // Also delete vertex shader if frag fails
		return -1;
	}

	// === Geometry Shader (if exists) === 
	GLuint gs;
	bool hasGeometry = !geometrySrc.empty();
	if (hasGeometry) {
		printf("Loading geometry shader\n");
		gs = glCreateShader(GL_GEOMETRY_SHADER);
		const char* gs_ptr = geometrySrc.c_str();
		glShaderSource(gs, 1, &gs_ptr, NULL);
		glCompileShader(gs);
		glGetShaderiv(gs, GL_COMPILE_STATUS, &success);
		if (!success) {
			printf("Error in fragment shader!\n");
			printShaderErrorInfo(gs);
			glDeleteShader(gs);
			glDeleteShader(fs);
			glDeleteShader(vs);
			return -1;
		}
	}

	// === Program Linking ===
	glAttachShader(glProgramID, vs);
	if(hasGeometry) {glAttachShader(glProgramID, gs);};
	glAttachShader(glProgramID, fs);
	glLinkProgram(glProgramID);
	GLint isLinked = GL_FALSE;
	glGetProgramiv(glProgramID, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		printf("Link error in shader program!\n");
		printShaderErrorInfo(glProgramID);
		glDeleteProgram(glProgramID);
		return -1;
	}

	// Delete shaders after linking
	glDeleteShader(vs);
	if(hasGeometry) glDeleteShader(gs);
	glDeleteShader(fs);

	if (glProgramID != 0) {
		//printf("Shader program linked successfully!\n");
	}

	return glProgramID;
}

// Extract the log from the shader object
void ShaderManager::printShaderErrorInfo(GLuint shaderProgram) {
	GLint maxLength = 0;
	glGetShaderiv(shaderProgram, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	GLchar * errorLog = (GLchar*)calloc(maxLength, sizeof(GLchar));
	glGetShaderInfoLog(shaderProgram, maxLength, &maxLength, &errorLog[0]);

	// Provide the infolog in whatever manor you deem best.
	printf("%s\n", errorLog);
	printf("\n");
	free(errorLog);
}

// ----------- Shader Manager ----------
void ShaderManager::initShaders() {
	shaderArr.clear();
	ShaderProgram temp;
	temp.vertexSource = loadShaderFromFile("./shaders/test/0-temp_vertex.glsl");
	temp.fragmentSource = loadShaderFromFile("./shaders/test/0-temp_frag.glsl");
	temp.programID = compileShaderProgram(temp.vertexSource, temp.fragmentSource);
	temp.name = "temp";
	shaderArr.push_back(temp);

	ShaderProgram test;
	test.vertexSource = loadShaderFromFile("./shaders/test/1-test_vertex.glsl");
	test.fragmentSource = loadShaderFromFile("./shaders/test/1-test_frag.glsl");
	test.programID = compileShaderProgram(test.vertexSource, test.fragmentSource);
	test.name = "test";
	shaderArr.push_back(test);	

	ShaderProgram basic;
	basic.vertexSource = loadShaderFromFile("./shaders/basic/2-basic_vertex.glsl");
	basic.fragmentSource = loadShaderFromFile("./shaders/basic/2-basic_frag.glsl");
	basic.programID = compileShaderProgram(basic.vertexSource, basic.fragmentSource);
	basic.name = "basic";
	shaderArr.push_back(basic);

	ShaderProgram interlaced;
	interlaced.vertexSource = loadShaderFromFile("./shaders/basic/3-interlaced_vertex.glsl");
	interlaced.fragmentSource = loadShaderFromFile("./shaders/basic/3-interlaced_frag.glsl");
	interlaced.programID = compileShaderProgram(interlaced.vertexSource, interlaced.fragmentSource);
	interlaced.name = "interlaced";
	shaderArr.push_back(interlaced);

	ShaderProgram phong;
	phong.vertexSource = loadShaderFromFile("./shaders/phong/4-phong_vertex.glsl");
	phong.fragmentSource = loadShaderFromFile("./shaders/phong/4-phong_frag.glsl");
	phong.programID = compileShaderProgram(phong.vertexSource, phong.fragmentSource);
	phong.name = "phong";
	shaderArr.push_back(phong);

	// ShaderProgram cartoon;
	// cartoon.vertexSource = loadShaderFromFile("./shaders/5-cartoon_vertex.glsl");
	// cartoon.fragmentSource = loadShaderFromFile("./shaders/5-cartoon_frag.glsl");
	// cartoon.programID = compileShaderProgram(cartoon.vertexSource, cartoon.fragmentSource);
	// cartoon.name = "cartoon";
	// shaderArr.push_back(cartoon);

	// ShaderProgram flat; // TODO: Fix the loading of flat shader
	// flat.vertexSource = loadShaderFromFile("./shaders/7-wireframe_vertex.glsl");
	// flat.geometrySource = loadShaderFromFile("./shaders/7-wireframe_geom.glsl");
	// flat.fragmentSource = loadShaderFromFile("./shaders/7-wireframe_frag.glsl");
	// flat.programID = compileShaderProgram(flat.vertexSource, flat.fragmentSource, flat.geometrySource);
	// flat.name = "flat";
	// shaderArr.push_back(flat); 

	// ShaderProgram wireframe;
	// wireframe.vertexSource = loadShaderFromFile("./shaders/7-wireframe_vertex.glsl");
	// wireframe.geometrySource = loadShaderFromFile("./shaders/7-wireframe_geom.glsl");
	// wireframe.fragmentSource = loadShaderFromFile("./shaders/7-wireframe_frag.glsl");
	// wireframe.programID = compileShaderProgram(wireframe.vertexSource, wireframe.fragmentSource, wireframe.geometrySource);
	// wireframe.name = "wireframe";
	// shaderArr.push_back(wireframe);
}

void ShaderManager::destroyAllShaders() {
	for (const auto& shader : shaderArr) {
		glDeleteProgram(shader.programID);
	}
	shaderArr.clear();
}

void ShaderManager::useNextShader() {
	if (shaderArr.empty()) return;
	currentShaderIndex = (currentShaderIndex + 1) % shaderArr.size();
	std::cout << "Switched to shader: " << shaderArr[currentShaderIndex].name << std::endl;
}

GLuint ShaderManager::getCurrentShader() {
	if (shaderArr.empty()) {
		perror("shadarArr access without initialization!");
		return -1;
	} else {
		// printf("returning shader id: %i\n", currentShaderIndex);
		return shaderArr[currentShaderIndex].programID;
	}
}

// std::vector<ShaderProgram> getShaderArray() {
// 	return shaders;
// }















void setMatrix4fv(GLuint programID, const std::string &name, glm::mat4 matrix) {
	// TODO: Cache the location 
	// GLint loc = glGetUniformLocation(programID, name.c_str());
	GLint loc = locCache::getUniformLoc(programID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void setVector3fv(GLuint programID, const std::string &name, glm::vec3 vector) {
	GLint loc = locCache::getUniformLoc(programID, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void setFloat1f(GLuint programID, const std::string &name, float value){
	GLint loc = locCache::getUniformLoc(programID, name.c_str());
	glUniform1f(loc, value);
}