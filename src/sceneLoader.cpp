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
		Light l1 = Light(lightPos1, 
			glm::vec3(0.1, 0.1, 0.1), // amb
			glm::vec3(1.0, 1.0, 1.0), // diff
			glm::vec3(1.0, 1.0, 1.0), // spec
			glm::vec3(0.0, -1.0, 0.0) // dir
		);
		// Cadillac
		SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
		caddillac->pos = glm::vec3(-1.f, -1.f, 0.f);
		caddillac->rot = glm::vec3(-90.f, 215.f, 0.f);
		objList->push_back(caddillac);
		Anim::addRotate(caddillac, glm::vec3(1.f, -1.f, 0.f));
		// Anim::addTranslate(caddillac, glm::vec3(1.f, 0.f, 0.f));
	}
	if (scene == 902) { // SCENE_CADILLAC_BENCHMARK
		// Camera
		cam->position = {0.f, 0.f, 5.f};
		cam->setRotation({0.f, 0.f, 0.f});
		// Light
		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, 
			glm::vec3(0.1, 0.1, 0.1), // amb
			glm::vec3(1.0, 1.0, 1.0), // diff
			glm::vec3(1.0, 1.0, 1.0), // spec
			glm::vec3(0.0, -1.0, 0.0) // dir
		);

		// Cadillacs
		int total = 0;
		for (int x = -20; x <= 18; x+=2) {
			for (int y = -20; y <= 18; y+=2) {
				for (int z = -50; z >= -58; z-=2) {
					total += 1;
					SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
					caddillac->scale = glm::vec3(0.3f, 0.3f, 0.3f);
					caddillac->pos = glm::vec3(float(x), float(y), float(z));
					caddillac->setShader(shprg);
					Anim::addRotate(caddillac, glm::vec3(10.f, -10.f, 0.f));
					// Anim::addTranslate(caddillac, glm::vec3(0.f, 0.f, 5.00f), 40);
					objList->push_back(caddillac);
				}
			}
		}
		printf("Total cadillacs: %i\n", total);
	}
	if (scene == 903) { // SCENE_SYNTHWAVE
		cam->position = {-30.f, 4.f, +15.f};
		// cam->pitch(-90.f);
		cam->yaw(-45.f);

		// Light sun = Light( // X: -2.324030, Y: 5.286932, Z: -5.944574
		// 	// glm::vec3(0.045758f, 2.029418f, 3.462264f),
		// 	glm::vec3(-0.917018f, 0.679168f, -4.061481f),
		// 	glm::vec3(0.1, 0.1, 0.1), // amb
		// 	glm::vec3(1.0, 1.0, 1.0), // diff
		// 	glm::vec3(1.0, 1.0, 1.0), // spec
		// 	glm::vec3(0.0, -1.0, 0.0), // dir
		// 	45.f, // cutoff angle
		// 	1.0f, // const
		// 	0.09f, // linear
		// 	0.032f // quadratic
		// );
		// Light::sendLightsToShader(shprg);

		glm::vec3 lightAmb = {0.1, 0.1, 0.1};
		glm::vec3 lightDiff = {1.0, 0.9, 0.9};
		glm::vec3 lightSpec = {1.0, 0.9, 0.9};
		glm::vec3 lightDir = {0.0, 0.0, -1.0};
		float lightCutoff = 45.f;
		float lightConstant = 1.0f;
		float lightLinear = 0.045f;
		float lightQuad = 0.0075f;

		// Light X: -0.819298, Y: 0.688937, Z: -3.603488
		glm::vec3 headlightLPos = {-0.819298f, 0.688937f, -3.603488f};
		glm::vec3 headlightRPos = {-headlightLPos.x, headlightLPos.y, headlightLPos.z};

		Light headlightL = Light(headlightLPos, lightAmb, lightDiff, lightSpec, lightDir, lightCutoff, lightConstant, lightLinear, lightQuad);
		Light headlightR = Light(headlightRPos, lightAmb, lightDiff, lightSpec, lightDir, lightCutoff, lightConstant, lightLinear, lightQuad);

	
		SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
		SceneObject* cubemap = new SceneObject("../objects/RoadSkybox/cubemap.obj");

		cubemap->scale = {680.f, 680.f, 680.f};
		cubemap->pos = {0.f, -300.f, 0.f};
		// cubemap->setShader(ShaderManager::getTestShader());
		// cubemap->fixShader = true;

		float roadLenght = 24.f;
		float roadWidth = 8.0f;

		caddillac->scale = {1.0f, 1.0f, 1.0f};
		caddillac->rot = {270.0f, 0.0f, 0.0f};
		caddillac->pos = {0.0f, 0.0f, 0.0f};

		float const carSpeed = 1.f; 
		glm::vec3 vec = {0.0f, 0.0f, carSpeed};
		int const road_segments = 30;
		float const road_offset = +24.0f;

		for (int i = 0; i < road_segments; i++) {
			SceneObject* road = new SceneObject("../objects/RoadSkybox/road.obj");			
			road->pos = {0.0f, 0.0f, i*-24.0f + road_offset};
			glm::vec3 startPosRoad = {0.0f, 0.0f, (road_segments-1)*-24.0f + road_offset};
			Anim::addTranslate(road, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
			objList->push_back(road);

			SceneObject* desertLeft = new SceneObject("../objects/RoadSkybox/desert.obj");
			SceneObject* desertRight = new SceneObject("../objects/RoadSkybox/desert.obj");
			desertLeft->pos = {-64.0f, 0.0f, i*-24.0f + road_offset};
			desertRight->pos = {+64.0f, 0.0f, i*-24.0f + road_offset};
			desertRight->rot = {0.0f, 180.0f, 0.0f};
			desertLeft->scale = {15.0f, 1.0, 3.0f};
			desertRight->scale = {15.0f, 1.0, 3.0f};

			// glm::vec3 startPosLamp = glm::vec3(0.0f, 0.0f, (road_segments-1)*-24.0f + road_offset);
			Anim::addTranslate(desertLeft, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
			Anim::addTranslate(desertRight, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
			objList->push_back(desertLeft);
			objList->push_back(desertRight);

			// glm::vec3 lampAmbient {0.1f, 0.1f, 0.1f};
			// glm::vec3 lampDiff {0.7f, 0.7f, 0.7f};
			// glm::vec3 lampSpec {0.2f, 0.2f, 0.2f};


			if (i%2 == 0){
				SceneObject* lampLeft = new SceneObject("../objects/RoadSkybox/lamp.obj");
				SceneObject* lampRight = new SceneObject("../objects/RoadSkybox/lamp.obj");

				glm::vec3 lampLightDir = {0.0f, -1.0f, 0.0f};

				lampLeft->pos = {-4.2f, 3.0f, i*-24.0f + road_offset};
				Light* lampLeftLight = new Light({-4.2f, 10.0f, i*-24.0f + road_offset}, lightAmb, lightDiff, lightSpec, lampLightDir, lightCutoff, lightConstant, lightLinear, lightQuad);

				lampRight->pos = glm::vec3(+4.2f, 3.0f, i*-24.0f + road_offset);
				Light* lampRightLight = new Light({+4.2f, 10.0f, i*-24.0f + road_offset}, lightAmb, lightDiff, lightSpec, lightDir, lightCutoff, lightConstant, lightLinear, lightQuad);

				lampLeft->rot = {0.0f, 180.0f, 0.0f};
				lampLeft->scale = {1.0f, 1.5f, 1.0f};
				lampRight->scale = {1.0f, 1.5f, 1.0f};

				// glm::vec3 startPosLamp = glm::vec3(0.0f, 0.0f, (road_segments-1)*-24.0f + road_offset);
				Anim::addTranslate(lampLeft, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
				Anim::addTranslate(lampLeftLight, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));

				Anim::addTranslate(lampRight, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
				Anim::addTranslate(lampRightLight, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));

				objList->push_back(lampLeft);
				objList->push_back(lampRight);
			}
		}
		objList->push_back(caddillac);
		objList->push_back(cubemap);
		Light::sendLightsToShader(shprg);
	}
}
