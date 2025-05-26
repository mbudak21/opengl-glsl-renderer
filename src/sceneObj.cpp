#include "sceneObj.h"
#include <shaders.h>


std::unordered_map<std::string, std::shared_ptr<obj>> SceneObject::objectMap;

SceneObject::SceneObject(const char* path){
	if(objectMap.find(path) != objectMap.end()){
		// printf("Loading the object from cache...");
		this->objInstance = objectMap[path].get();
	}
	else{
		obj* o; 
		o = obj_create(path);

		FILE *f = fopen(path, "r");
		if (!f) {
			printf("ERROR: failed to load the Obj from: %s\n", path);
		} else {
			printf("Loaded the Obj from: %s\n", path);
			fclose(f);
			auto objPtr = std::shared_ptr<obj>(o);
			objectMap[path] = objPtr;
			this->objInstance = objectMap[path].get();
		}
	}
}
GLuint SceneObject::getShader(){
	return this->shprg;
}

void SceneObject::setShader(GLuint shaderId){
	if(!fixShader){
		this->shprg = shaderId;
	}
	// printf("Sending new material data...\n");
	obj* o = this->objInstance;
	obj_set_prop_loc(o, OBJ_KD, -1, glGetUniformLocation(shaderId, "diffuseMap"), -1);
	obj_set_prop_loc(o, OBJ_KA, -1, glGetUniformLocation(shaderId, "ambientMap"), -1);
	obj_set_prop_loc(o, OBJ_KE, -1, glGetUniformLocation(shaderId, "emissiveMap"), -1);
	obj_set_prop_loc(o, OBJ_KS, -1, glGetUniformLocation(shaderId, "specularMap"), -1);
	obj_set_prop_loc(o, OBJ_NS, -1, glGetUniformLocation(shaderId, "shininess"), -1);
	obj_set_prop_loc(o, OBJ_KN, -1, glGetUniformLocation(shaderId, "normal"), -1);

	glUniform1i(glGetUniformLocation(shaderId, "useDiffuseMap"), 1);
	glUniform1i(glGetUniformLocation(shaderId, "useAmbientMap"), 1);
	glUniform1i(glGetUniformLocation(shaderId, "useSpecularMap"), 1);
	glUniform1i(glGetUniformLocation(shaderId, "useEmissiveMap"), 1);
};

void SceneObject::prepareObject(){
	// printf("Preparing objects for shading...\n");
    obj* o = this->objInstance;
	obj_proc(o);
	int vTangentLoc = 3;
	int vNormLoc = 1;
	int vTexCoordLoc = 2;
	int vPosLoc = 0;
	obj_set_vert_loc(o, vTangentLoc, vNormLoc, vTexCoordLoc, vPosLoc);
	// printf("OBJ Diagnostics:\n");
	// printf("  Materials:       %d\n", obj_num_mtrl(o));
	// printf("  Vertices:        %d\n", obj_num_vert(o));
	// printf("  Surfaces:        %d\n", obj_num_surf(o));

	// for (int si = 0; si < obj_num_surf(o); ++si) {
	// 	printf("    Surface %d: Polygons = %d, Lines = %d\n",
	// 		si,
	// 		obj_num_poly(o, si),
	// 		obj_num_line(o, si));
	// }
}

void SceneObject::renderObject(glm::mat4 P, glm::mat4 V){
	this->renderObject(P, V, this->shprg);
}

void SceneObject::renderObject(glm::mat4 P, glm::mat4 V, GLuint shaderId) {
    obj* o = this->objInstance;
    
	// Matrix M = Translate(pos) * Rotate(rot) * Scale(scale);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), this->pos);
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.x), glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.y), glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.z), glm::vec3(0, 0, 1));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scale);
	
	glm::mat4 R = Rz * Ry * Rx;	
	glm::mat4 M = T * R * S;
	// glm::mat4 PV_M = PV * M;

	glUseProgram(shaderId);
	setMatrix4fv(shaderId, "P", P);
	setMatrix4fv(shaderId, "V", V);
	setMatrix4fv(shaderId, "M", M);
	setMatrix4fv(shaderId, "PVM", P*V*M);

	obj_render(o);
	// printf("vao = %u, vbo = %u, vloc = %d\n", o->vao, o->vbo, o->vloc);
}