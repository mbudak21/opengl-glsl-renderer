#include "shaders.h"
#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

static vector<ShaderProgram> shaders;
static int currentShaderIndex = 0;

void setMatrix4fv(GLuint programID, const std::string &name, glm::mat4 matrix) {
	// TODO: Cache the location 
	GLint loc = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix));
}

void setVector3fv(GLuint programID, const std::string &name, glm::vec3 vector) {
	GLint loc = glGetUniformLocation(programID, name.c_str());
	glUniform3fv(loc, 1, glm::value_ptr(vector));
}

void setFloat1f(GLuint programID, const std::string &name, float value){
	GLint loc = glGetUniformLocation(programID, name.c_str());
	glUniform1f(loc, value);
}

std::string loadShaderFromFile(const std::string& filename) {
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

GLuint compileShaderProgram(const std::string& vs_str, const std::string& fs_str, const std::string& geometrySrc) {
	const char* vs_ptr = vs_str.c_str();
	const char* fs_ptr = fs_str.c_str();

	GLuint shaderProgram = glCreateProgram();
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
		return 0;
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
		return 0;
	}

	// === Geometry Shader (if exists) === 
	GLuint gs;
	bool hasGeometry = !geometrySrc.empty();
	if (hasGeometry) {
		printf("Loading geometry shader");
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
			return 0;
		}
	}

	// === Program Linking ===
	glAttachShader(shaderProgram, vs);
	if(hasGeometry) {glAttachShader(shaderProgram, gs);};
	glAttachShader(shaderProgram, fs);
	glLinkProgram(shaderProgram);
	GLint isLinked = GL_FALSE;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &isLinked);
	if (!isLinked) {
		printf("Link error in shader program!\n");
		printShaderErrorInfo(shaderProgram);
		glDeleteProgram(shaderProgram);
		shaderProgram = 0;
	}

	// Delete shaders after linking
	glDeleteShader(vs);
	if(hasGeometry) glDeleteShader(gs);
	glDeleteShader(fs);

	if (shaderProgram != 0) {
		printf("Shader program linked successfully!\n");
	}

	return shaderProgram;
}

// Extract the log from the shader object
void printShaderErrorInfo(GLuint shaderProgram) {
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
void initShaders() {
	shaders.clear();
	ShaderProgram temp;
	temp.vertexSource = loadShaderFromFile("./shaders/temp_vertex.glsl");
	temp.fragmentSource = loadShaderFromFile("./shaders/temp_frag.glsl");
	temp.programID = compileShaderProgram(temp.vertexSource, temp.fragmentSource);
	temp.name = "temp";
	shaders.push_back(temp);	


	ShaderProgram flat;
	flat.vertexSource = loadShaderFromFile("./shaders/flat_vertex.glsl");
	flat.geometrySource = loadShaderFromFile("./shaders/flat_geom.glsl");
	flat.fragmentSource = loadShaderFromFile("./shaders/flat_frag.glsl");
	flat.programID = compileShaderProgram(flat.vertexSource, flat.fragmentSource, flat.geometrySource);
	flat.name = "flat";
	shaders.push_back(flat);

	ShaderProgram basic;
	basic.vertexSource = loadShaderFromFile("./shaders/1-basic_vertex.glsl");
	basic.fragmentSource = loadShaderFromFile("./shaders/1-basic_frag.glsl");
	basic.programID = compileShaderProgram(basic.vertexSource, basic.fragmentSource);
	basic.name = "basic";
	shaders.push_back(basic);

	ShaderProgram interlaced;
	interlaced.vertexSource = loadShaderFromFile("./shaders/2-interlaced_vertex.glsl");
	interlaced.fragmentSource = loadShaderFromFile("./shaders/2-interlaced_frag.glsl");
	interlaced.programID = compileShaderProgram(interlaced.vertexSource, interlaced.fragmentSource);
	interlaced.name = "interlaced";
	shaders.push_back(interlaced);

	ShaderProgram phong;
	phong.vertexSource = loadShaderFromFile("./shaders/3-phong_vertex.glsl");
	phong.fragmentSource = loadShaderFromFile("./shaders/3-phong_frag.glsl");
	phong.programID = compileShaderProgram(phong.vertexSource, phong.fragmentSource);
	phong.name = "phong";
	shaders.push_back(phong);

	ShaderProgram cartoon;
	cartoon.vertexSource = loadShaderFromFile("./shaders/4-cartoon_vertex.glsl");
	cartoon.fragmentSource = loadShaderFromFile("./shaders/4-cartoon_frag.glsl");
	cartoon.programID = compileShaderProgram(cartoon.vertexSource, cartoon.fragmentSource);
	cartoon.name = "cartoon";
	shaders.push_back(cartoon);

	ShaderProgram wireframe;
	wireframe.vertexSource = loadShaderFromFile("./shaders/wireframe_vertex.glsl");
	wireframe.geometrySource = loadShaderFromFile("./shaders/wireframe_geom.glsl");
	wireframe.fragmentSource = loadShaderFromFile("./shaders/wireframe_frag.glsl");
	wireframe.programID = compileShaderProgram(wireframe.vertexSource, wireframe.fragmentSource, wireframe.geometrySource);
	wireframe.name = "wireframe";
	shaders.push_back(wireframe);
}

void useNextShader() {
	if (shaders.empty()) return;
	currentShaderIndex = (currentShaderIndex + 1) % shaders.size();
	std::cout << "Switched to shader: " << shaders[currentShaderIndex].name << std::endl;
}

GLuint getCurrentShader() {
	if (shaders.empty()) return 0;
	return shaders[currentShaderIndex].programID;
}

void destroyAllShaders() {
	for (const auto& shader : shaders) {
		glDeleteProgram(shader.programID);
	}
	shaders.clear();
}