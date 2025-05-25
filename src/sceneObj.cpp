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
void SceneObject::setShader(GLuint ID){
	this->shprg = ID;
};

void SceneObject::prepareObject(){
    obj* o = this->objInstance;
    
	obj_proc(o);

	// layout(location = 0) in vec3 vPos;
	// layout(location = 1) in vec3 vNorm;
	// layout(location = 2) in vec2 vTexCoord;
	// layout(location = 3) in vec3 vTangent;
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
	GLuint id = this->shprg;
	this->renderObject(id, P, V);
}

void SceneObject::renderObject(GLuint shaderId, glm::mat4 P, glm::mat4 V) {
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

	glUseProgram(shprg);
	setMatrix4fv(shprg, "P", P);
	setMatrix4fv(shprg, "V", V);
	setMatrix4fv(shprg, "M", M);
	setMatrix4fv(shprg, "PVM", P*V*M);

	obj_set_prop_loc(o, OBJ_KD, -1, glGetUniformLocation(shprg, "diffuseMap"), -1);
	glUniform1i(glGetUniformLocation(shprg,"useDiffuseMap"), 1);


  	// obj_set_prop_loc(o, OBJ_KN, -1, glGetUniformLocation(shprg, "NormalTexture"), -1);
	// obj_set_prop_loc(o, OBJ_KA, -1, glGetUniformLocation(shprg, "matAmb"), -1);
	// obj_set_prop_loc(o, OBJ_KD, -1, glGetUniformLocation(shprg, "matDiff"), -1);
	// obj_set_prop_loc(o, OBJ_KS, -1, glGetUniformLocation(shprg, "matSpec"), -1);
	// obj_set_prop_loc(o, OBJ_KE, -1, -1, -1);
	// obj_set_prop_loc(o, OBJ_NS, -1, glGetUniformLocation(shprg, "matShininess"), -1);
	// obj_set_prop_loc(o, OBJ_KD, -1, glGetUniformLocation(shprg, "diffuseMap"), -1);

	obj_render(o);
	// printf("vao = %u, vbo = %u, vloc = %d\n", o->vao, o->vbo, o->vloc);
}