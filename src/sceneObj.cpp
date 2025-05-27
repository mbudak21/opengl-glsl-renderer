#include "sceneObj.h"

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
	glUseProgram(this->shprg);
	GLuint diffPos = locCache::getUniformLoc(this->shprg, "material.diffuseMap");
	GLuint ambPos = locCache::getUniformLoc(this->shprg, "material.ambientMap");
	GLuint emissivePos = locCache::getUniformLoc(this->shprg, "material.emissiveMap");
	GLuint specPos = locCache::getUniformLoc(this->shprg, "material.specularMap");
	GLuint shinePos = locCache::getUniformLoc(this->shprg, "material.shininess");
	GLuint normPos = locCache::getUniformLoc(this->shprg, "material.normalMap");

	printf("=== Set Shader Upload ===\n");
	printf("Locations:\n");
	printf("  diffuseMap:	%d\n", diffPos);
	printf("  ambientMap:	%d\n", ambPos);
	printf("  emissiveMap:	%d\n", emissivePos);
	printf("  specularMap:	%d\n", specPos);
	printf("  shininess:	%d\n", shinePos);
	printf("  normal:	%d\n", normPos);

	obj_set_prop_loc(o, OBJ_KD, -1, diffPos, -1);
	obj_set_prop_loc(o, OBJ_KA, -1, ambPos, -1);
	obj_set_prop_loc(o, OBJ_KE, -1, emissivePos, -1);
	obj_set_prop_loc(o, OBJ_KS, -1, specPos, -1);
	obj_set_prop_loc(o, OBJ_NS, -1, shinePos, -1);
	obj_set_prop_loc(o, OBJ_KN, -1, normPos, -1);
	this->prepareObject();
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
	// locCache::getUniformLoc();
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

void SceneObject::renderObject(glm::mat4 P, glm::mat4 V) {    
	// Matrix M = Translate(pos) * Rotate(rot) * Scale(scale);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), this->pos);
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.x), glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.y), glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(this->rot.z), glm::vec3(0, 0, 1));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), this->scale);
	
	glm::mat4 R = Rz * Ry * Rx;	
	glm::mat4 M = T * R * S;
	// glm::mat4 PV_M = PV * M;

	glUseProgram(this->getShader());
	// setMatrix4fv(shaderId, "P", P);
	// setMatrix4fv(shaderId, "V", V);
	setMatrix4fv(this->getShader(), "M", M);
	setMatrix4fv(this->getShader(), "PVM", P*V*M);

	obj_render(this->objInstance);
	// printf("vao = %u, vbo = %u, vloc = %d\n", o->vao, o->vbo, o->vloc);
}