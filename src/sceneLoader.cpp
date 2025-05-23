#include "sceneLoader.h"

#include "mesh_bunny.h"
#include "mesh_cow.h"
#include "mesh_cube.h"
#include "mesh_frog.h"
#include "mesh_knot.h"
#include "mesh_sphere.h"
#include "mesh_teapot.h"
#include "mesh_triceratops.h"

obj* loadObject(const char* path){
	obj* o; 
	o = obj_create(path);

	FILE *f = fopen(path, "r");
	if (!f) {
		printf("ERROR: failed to load the Obj at: %s\n", path);
		return nullptr;
	} else {
		printf("Loaded the Obj at: %s\n", path);
		fclose(f);
		return o;
	}
}

void loadScene(int scene, Camera* cam, std::vector<obj*>* objList, GLuint shprg) {
    printf("Loading scene number: %i\n", scene);
	if (scene == 901) { // SCENE_CADILLAC
		// Camera
		cam->position = {0.f, 0.f, 5.f};
		cam->setRotation({0.f, 0.f, 0.f});
		// Light
		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
		// Cadillac
		obj* caddillac = loadObject("../objects/Cadillac/Cadillac.obj");
		objList->push_back(caddillac);

		// obj* cubemap = loadObject("../objects/RoadSkybox/cubemap.obj");
		// objList->push_back(cubemap);

		// obj* desert = loadObject("../objects/RoadSkybox/desert.obj");
		// objList->push_back(desert);

		// obj* lamp = loadObject("../objects/RoadSkybox/lamp.obj");
		// objList->push_back(lamp);

		// obj* road = loadObject("../objects/RoadSkybox/road.obj");
		// objList->push_back(road);

		// glm::vec3 cubeScale = {0.1f, 0.1f, 0.1f}; //*5 for original scaling
		// glm::vec3 cubePos = {0.f, 0.f, 0.f};
		// glm::vec3 cubeRot = {0.f, 0.f, 0.f};
		// Mesh* rawCubeMesh = insertModel(meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::Turquoise());
		// glm::vec3 cubeScale = {4.f*5, 0.1f*5, 4.f*5}; //*5 for original scaling
		// glm::vec3 cubePos = {-10.f, -1.f, -10.f};
		// glm::vec3 cubeRot = {0.f, 0.f, 0.f};

		// glm::vec3 knotScale = {0.4f, 0.4f, 0.4f};
		// glm::vec3 knotPos = {0.f, 4.f, 0.f};
		// glm::vec3 knotRot = {0.f, 0.f, 0.f};

		// glm::vec3 bunnyScale = {60.f, 60.f, 60.f};
		// glm::vec3 bunnyPos = {0.f, 8.f, +1.f};
		// glm::vec3 bunnyRot = {0.f, 0.f, 0.f};
	
		// Mesh* rawCubeMesh = insertModel(meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::Brass());
		// Mesh* rawKnotMesh = insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, knotScale, knotPos, knotRot, Material::RedRealistic());
		// Mesh* rawBunnyMesh = insertModel(meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, bunnyScale, bunnyPos, bunnyRot, Material::Obsidian());

		// Anim::addRotate(rawKnotMesh, 50.0f);
		// Anim::addRotate(rawBunnyMesh, -50.0f);
		// Anim::addScale(rawBunnyMesh, glm::vec3(-5.0f, -5.0f, -5.0f));
		// Anim::addScale(rawCubeMesh, glm::vec3(1.0f, 0.0f, 1.0f));


	}
}
