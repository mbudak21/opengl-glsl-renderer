#include "sceneLoader.h"

// #include "mesh_bunny.h"
// #include "mesh_cow.h"
// #include "mesh_cube.h"
// #include "mesh_frog.h"
// #include "mesh_knot.h"
// #include "mesh_sphere.h"
// #include "mesh_teapot.h"
// #include "mesh_triceratops.h"

void loadScene(int scene, Camera* cam, std::vector<SceneObject*>* objList, GLuint shprg) {
    printf("Loading scene number: %i\n", scene);
	if (scene == 901) { // SCENE_CADILLAC
		// Camera
		cam->position = {0.f, 0.f, 5.f};
		cam->setRotation({0.f, 0.f, 0.f});
		// Light
		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));
		// Cadillac
		SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
		caddillac->pos = glm::vec3(-1.f, -1.f, 0.f);
		caddillac->rot = glm::vec3(-90.f, 215.f, 0.f);
		objList->push_back(caddillac);
		Anim::addRotate(caddillac, glm::vec3(1.f, -1.f, 0.f));
		Anim::addTranslate(caddillac, glm::vec3(1.f, 0.f, 0.f));
	}
	if (scene == 902) { // SCENE_CADILLAC_BENCHMARK
		// Camera
		cam->position = {0.f, 0.f, 5.f};
		cam->setRotation({0.f, 0.f, 0.f});
		// Light
		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.1, 0.1), glm::vec3(1.0, 1.0, 1.0), glm::vec3(1.0, 1.0, 1.0));

		// Cadillacs
		int total = 0;
		for (int x = -20; x <= 18; x+=2) {
			for (int y = -20; y <= 18; y+=2) {
				for (int z = -50; z >= -58; z-=2) {
					total += 1;
					SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
					caddillac->scale = glm::vec3(0.3f, 0.3f, 0.3f);
					caddillac->pos = glm::vec3(float(x), float(y), float(z));
					caddillac->shprg = shprg;
					Anim::addRotate(caddillac, glm::vec3(10.f, -10.f, 0.f));
					// Anim::addTranslate(caddillac, glm::vec3(0.f, 0.f, 5.00f), 40);
					objList->push_back(caddillac);
				}
			}
		}
		printf("Total cadillacs: %i\n", total);
	}
	if (scene == 903) { // SCENE_SYNTHWAVE
	}
}
