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

void loadScene(int scene, Camera* cam, _Mesh** meshList, std::vector<obj*>* objList, GLuint shprg) {
    printf("Loading scene number: %i\n", scene);
	if (scene == 121) { // SCENE_DEFAULT_CAMERA
		glm::vec3 cowPos = {0.f, 0.f, 0.f};
		glm::vec3 cowRot = {0.f, 0.f, 0.f};
		glm::vec3 cowScale = {20.f, 20.f, 20.f};

		glm::vec3 triceratopsPos = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsRot = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsScale = {3.f, 3.f, 3.f};

		assert(cam->position.x == 0.0f);
		assert(cam->position.y == 0.0f);
		assert(cam->position.z == 20.0f);

		insertModel(meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::RedRealistic());
		insertModel(meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::GreenRealistic());
	}
	if (scene == 122) { // SCENE_GIVEN_CAMERA
		cam->position = {-5.f, -5.f, 20.f};
		assert(cam->position.x == -5.0f);
		assert(cam->position.y == -5.0f);
		assert(cam->position.z == 20.0f);
		cam->setRotation({-10.f, 30.f, 45.f});
		// cam.setRotation({-10.f, -30.f, -45.f});
		// cam.setRotation({10.f, 30.f, 45.f});

		glm::vec3 cowPos = {0.f, 0.f, 0.f};
		glm::vec3 cowRot = {0.f, 0.f, 0.f};
		glm::vec3 cowScale = {20.f, 20.f, 20.f};

		glm::vec3 triceratopsPos = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsRot = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsScale = {3.f, 3.f, 3.f};

		insertModel(meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::RedRealistic());
		insertModel(meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::GreenRealistic());
	}
	if (scene == 131) { // SCENE_KNOT_ORTHOGONAL
		insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
	}
	if (scene == 132) { // SCENE_KNOT_ORTHOGONAL
		insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
	}
	if (scene == 141) { // SCENE_KNOT_COLORFULL
		insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
	}
	if (scene == 151) { // SCENE_COW_TEAPOT_TRICERATOPS
		glm::vec3 cowPos = {0.f, 1.5f, -8.f};
		glm::vec3 cowRot = {0.f, -90.f, 0.f};
		glm::vec3 cowScale = {20.f, 20.f, 20.f};

		glm::vec3 teapotPos = {0.f, -7.f, 0.f};
		glm::vec3 teapotRot = {-90.f, 180.f, 0.f};
		glm::vec3 teapotScale = {3.f, 3.f, 3.f};

		glm::vec3 triceratopsPos = {-6.8f, 0.8f, 5.f};
		glm::vec3 triceratopsRot = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsScale = {0.3f, 0.3f, 0.3f};

		insertModel(meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::Brass());
		insertModel(meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::RedRubber());
		insertModel(meshList, teapot.nov, teapot.verts, teapot.nof, teapot.faces, teapotScale, teapotPos, teapotRot, Material::Silver());

		glm::vec3 lightPos = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos, glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.9, 0.9, 0.7), glm::vec3(1.0, 1.0, 1.0));

		// insertModel(&meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, 60.0);
		// insertModel(&meshList, cube.nov, cube.verts, cube.nof, cube.faces, 5.0);
		// insertModel(&meshList, frog.nov, frog.verts, frog.nof, frog.faces, 2.5);
		// insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0);
		// insertModel(&meshList, sphere.nov, sphere.verts, sphere.nof, sphere.faces, 12.0);
	}
	if (scene == 250) { // SCENE_LIGHTS
		cam->position = {5.f, 7.f, 20.f};
		cam->setRotation({-13.f, 13.f, 0.f});

		glm::vec3 cubeScale = {4.f, 0.1f, 4.f};
		glm::vec3 cubePos = {-10.f, -1.f, -10.f};
		glm::vec3 cubeRot = {0.f, 0.f, 0.f};

		glm::vec3 knotScale = {0.4f, 0.4f, 0.4f};
		glm::vec3 knotPos = {0.f, 4.f, 0.f};
		glm::vec3 knotRot = {0.f, 0.f, 0.f};
	
		insertModel(meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::White());
		insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, knotScale, knotPos, knotRot, Material::White());

		glm::vec3 lightPos1 = glm::vec3(-20.0, 4.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.0, 0.0), glm::vec3(0.3, 0.0, 0.0), glm::vec3(0.2, 0.0, 0.0));

		glm::vec3 lightPos2 = glm::vec3(+20.0, 4.0, 10.0);
		Light l2 = Light(lightPos2, glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0, 0.0, 0.3), glm::vec3(0.0, 0.0, 0.2));
	}
	if (scene == 321) { // SCENE_ANIMATIONS
		cam->position = {5.f, 7.f, 20.f};
		cam->setRotation({+13.f, -13.f, 0.f});

		glm::vec3 cubeScale = {4.f*5, 0.1f*5, 4.f*5}; //*5 for original scaling
		glm::vec3 cubePos = {-10.f, -1.f, -10.f};
		glm::vec3 cubeRot = {0.f, 0.f, 0.f};

		glm::vec3 knotScale = {0.4f, 0.4f, 0.4f};
		glm::vec3 knotPos = {0.f, 4.f, 0.f};
		glm::vec3 knotRot = {0.f, 0.f, 0.f};

		glm::vec3 bunnyScale = {60.f, 60.f, 60.f};
		glm::vec3 bunnyPos = {0.f, 8.f, +1.f};
		glm::vec3 bunnyRot = {0.f, 0.f, 0.f};
	
		Mesh* rawCubeMesh = insertModel(meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::Brass());
		Mesh* rawKnotMesh = insertModel(meshList, knot.nov, knot.verts, knot.nof, knot.faces, knotScale, knotPos, knotRot, Material::RedRealistic());
		Mesh* rawBunnyMesh = insertModel(meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, bunnyScale, bunnyPos, bunnyRot, Material::Obsidian());

		Anim::addRotate(rawKnotMesh, 50.0f);
		Anim::addRotate(rawBunnyMesh, -50.0f);
		Anim::addScale(rawBunnyMesh, glm::vec3(-5.0f, -5.0f, -5.0f));
		Anim::addScale(rawCubeMesh, glm::vec3(1.0f, 0.0f, 1.0f));

		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.2, 0.1, 0.1), glm::vec3(0.9, 0.45, 0.35), glm::vec3(1.0, 1.0/2, 1.0/2));

		glm::vec3 lightPos2 = glm::vec3(+20.0, 10.0, 10.0);
		Light l2 = Light(lightPos2, glm::vec3(0.1, 0.1, 0.2), glm::vec3(0.45, 0.45, 0.7), glm::vec3(1.0/2, 1.0/2, 1.0));
	}
	if (scene == 901) { // SCENE_CADILLAC
		// Camera
		cam->position = {0.f, 0.f, 5.f};
		cam->setRotation({0.f, 0.f, 0.f});
		// Light
		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
		// Cadillac
		// obj* caddillac = loadObject("../objects/Cadillac/Cadillac.obj");
		// objList->push_back(caddillac);

		// obj* cubemap = loadObject("../objects/RoadSkybox/cubemap.obj");
		// objList->push_back(cubemap);

		// obj* desert = loadObject("../objects/RoadSkybox/desert.obj");
		// objList->push_back(desert);

		// obj* lamp = loadObject("../objects/RoadSkybox/lamp.obj");
		// objList->push_back(lamp);

		obj* road = loadObject("../objects/RoadSkybox/road.obj");
		objList->push_back(road);

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
