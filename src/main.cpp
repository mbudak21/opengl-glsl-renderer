//#define _CRT_SECURE_NO_WARNINGS
#if defined(WIN32) || defined(_WIN32) || defined(_WIN64)
	#include <glew.h>
	#include <freeglut.h>
#elif defined(__APPLE__)
	#define GL_SILENCE_DEPRECATION
	#include <GL/glew.h>
	#include <GLUT/glut.h>
#else
	#include <GL/glew.h>
	#include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include "camera.h"
#include "shaders.h"
#include "constants.h"
#include <glm/gtc/type_ptr.hpp>
#include "inputManager.h"
#include "lights.h"
#include "animations.h"
#include <chrono>
#include "sceneObj.h"

void changeSize(int w, int h);
void keypress(unsigned char key, int x, int y);
void keyrelease(unsigned char key, int x, int y);
void update();
void display(void);
void init(void);
void cleanUp(void);
int main(int argc, char **argv);
void loadScene(int scene, Camera* cam, std::vector<SceneObject*>* objList, GLuint shprg);

// Initializations
Camera cam;
InputManager inputManager; // Handles keyboard input
static std::vector<SceneObject*> objList; // Global pointer to list of Obj's
GLuint currShprg;
int windowHandle;

// Constants
int screen_width = 1024;
int screen_height = 768;

int frameCount = 1;
auto startTime = std::chrono::high_resolution_clock::now();
float startTimeFp = std::chrono::duration<float>(startTime.time_since_epoch()).count();
int fpsCounter = 0;
float fpsTimer = 0.0f;

auto lastTime = std::chrono::high_resolution_clock::now();
int numLights = 0;
glm::vec3 lightWorldPos[MAX_LIGHTS];
ProjectionMode currentProjMode = PROJ_MODE;
GLenum currentPolygonMode = POLYGON_MODE;

Light* headlightL;
Light* headlightR;

// Global transform matrices
// V is the view transform
// P is the projection transform
// PV = P * V is the combined view-projection transform
glm::mat4 V, P;

void changeSize(int w, int h) {
	screen_width = w;
	screen_height = h;
	glViewport(0, 0, screen_width, screen_height);
}

void keypress(unsigned char key, int x, int y) {
	int modifiers = glutGetModifiers();
	if (modifiers && GLUT_ACTIVE_SHIFT) { inputManager.shiftPress(); }
	if (modifiers && GLUT_ACTIVE_CTRL)  { inputManager.ctrlPress(); }
    inputManager.keyPress(key);
}

void keyrelease(unsigned char key, int x, int y) {
	int modifiers = glutGetModifiers();
	if (!(modifiers && GLUT_ACTIVE_SHIFT)) { inputManager.shiftRelease(); }
	if (!(modifiers && GLUT_ACTIVE_CTRL))  { inputManager.ctrlRelease(); }
    inputManager.keyRelease(key);
}

void update() {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - lastTime).count();
	static float key_delay = 0.0f;
	key_delay += dt;

	if (inputManager.isPressed('p') && key_delay >= KEY_DELAY) printf("X: %f, Y: %f, Z: %f\n", cam.position.x, cam.position.y, cam.position.z);
	
	if (inputManager.isPressed(',') && key_delay >= KEY_DELAY) { // First-person view
		cam.position = {-0.45f, 1.1f, -0.395f};
		cam.setRotation({-8.0f, 0.0f, 0.0f});
	}
	if (inputManager.isPressed('.') && key_delay >= KEY_DELAY) { // Third-person view
		cam.position = {0.0f, 2.75f, 7.50f};
		cam.setRotation({0.0f, 0.0f, 0.0f});
	}
	

	// === Camera based movement ===
    if (inputManager.isPressed('w')) cam.moveForward(MOVEMENT_SPEED);
    if (inputManager.isPressed('a')) cam.moveRight(-MOVEMENT_SPEED);
    if (inputManager.isPressed('s')) cam.moveForward(-MOVEMENT_SPEED);
    if (inputManager.isPressed('d')) cam.moveRight(MOVEMENT_SPEED);
	if (inputManager.isPressed(' ')) cam.moveUp(MOVEMENT_SPEED); // Space 
	if (inputManager.isPressed('c')) cam.moveUp(-MOVEMENT_SPEED); 

	// === Camera rotation ===
	if (inputManager.isPressed('q')) cam.roll(ROTATION_SPEED);
	if (inputManager.isPressed('e')) cam.roll(-ROTATION_SPEED);
	if (inputManager.isPressed('r')) cam.pitch(ROTATION_SPEED);
	if (inputManager.isPressed('f')) cam.pitch(-ROTATION_SPEED);
	if (inputManager.isPressed('t')) cam.yaw(ROTATION_SPEED);
	if (inputManager.isPressed('g')) cam.yaw(-ROTATION_SPEED);

	// === Axis based movement ===
	if (inputManager.isPressed('x') && !inputManager.shiftPressed()) cam.position.x += MOVEMENT_SPEED;
	if (inputManager.isPressed('y') && !inputManager.shiftPressed()) cam.position.y += MOVEMENT_SPEED;
	if (inputManager.isPressed('z') && !inputManager.shiftPressed()) cam.position.z += MOVEMENT_SPEED;
	if (inputManager.isPressed('X') && inputManager.shiftPressed()) cam.position.x -= MOVEMENT_SPEED;
	if (inputManager.isPressed('Y') && inputManager.shiftPressed()) cam.position.y -= MOVEMENT_SPEED;
	if (inputManager.isPressed('Z') && inputManager.shiftPressed()) cam.position.z -= MOVEMENT_SPEED;

	// === Utilities ===
	if (inputManager.isPressed('m') && key_delay >= KEY_DELAY) {
		key_delay = 0.f;
		if (currentPolygonMode == GL_POINT) {currentPolygonMode = GL_LINE;}
		else if (currentPolygonMode == GL_LINE){currentPolygonMode = GL_FILL;}
		else if (currentPolygonMode == GL_FILL){currentPolygonMode = GL_POINT;}
	}
	// Projection Modes
	if (inputManager.isPressed('1')) currentProjMode = PERSPECTIVE;
	if (inputManager.isPressed('2')) currentProjMode = ORTHOGONAL;

	// Shader Changes
	if (inputManager.isPressed('b') && key_delay >= KEY_DELAY) {
		key_delay = 0.f;
		ShaderManager::useNextShader();
		currShprg = ShaderManager::getCurrentShader();
		for(auto o:objList){
			o->setShader(currShprg);
		}
		Light::sendLightsToShader(currShprg);

	}
		glm::vec3 amb = {0.1, 0.1, 0.1};
		glm::vec3 diff = {1.0, 1.0, 0.7};
		glm::vec3 spec = {1.0, 1.0, 0.7};
		glm::vec3 off = {0.0, 0.0, 0.0};
	// Car header lights
	// TODO: Fix this mess
	if (inputManager.isPressed('L') && key_delay >= KEY_DELAY){
		key_delay = 0.f;
		if((headlightL->amb == off) && (headlightL->diff == off) && (headlightL->spec == off) || 
		(headlightR->amb == off) && (headlightR->diff == off) && (headlightR->spec == off)) {
			headlightL->amb = amb; headlightR->amb = amb;
			headlightL->diff = diff; headlightR->diff = diff;
			headlightL->spec = spec; headlightR->spec = spec;
		} else{
			headlightL->amb = off; headlightR->amb = off;
			headlightL->diff = off; headlightR->diff = off;
			headlightL->spec = off; headlightR->spec = off;
		}
	}

	// Exit
	if (inputManager.isPressed('q') && inputManager.shiftPressed()) {
		#ifdef __APPLE__
			// MAC OS GLUT does not implement LeaveMainLoop().
			cleanUp();
			exit(0);
		#else
			glutLeaveMainLoop();
		#endif
	}
	if(ShaderManager::perlinShaderUsed){ // TODO: There is probably a better way to do this. This shit kills all the fps
		Light::sendLightsToShader(ShaderManager::getPerlinShader());
	}
	Anim::update(frameCount * ANIMATION_SPEED);
    glutPostRedisplay();
}

void display(void) {
    auto now = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(now - lastTime).count();
    lastTime = now;

    // === Timing/FPS calculation ===

    fpsCounter++;
    fpsTimer += dt;

    if (fpsTimer >= 1.0f) {
        float fps = fpsCounter / fpsTimer;
        // printf("FPS: %.1f\n", fps);
		printf("FPS: %.1f, Delay: %.3fms\n", fps, dt*1000);

		fpsCounter = 0;
		fpsTimer = 0.0f;
    } frameCount++;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	V = cam.getViewMatrix(); // moves world coordinates into camera space
	
	if (currentProjMode == PERSPECTIVE) {
		P = cam.getPerspectiveMatrix(screen_width, screen_height); // projects 3D points into 2D screen space
	} else {
		P = cam.getOrthographicMatrix(-20, 20, -10, 10);
	}

	// Select the shader program to be used during rendering 
	glUseProgram(currShprg);
	setVector3fv(currShprg, "camPos", cam.position);

	Light::sendLightsToShader(currShprg);

	for(auto o:objList){
		o->renderObject(P, V);		
	}
	glutSwapBuffers();
}

void init(void) {
	ShaderManager::initShaders();
	currShprg = ShaderManager::getCurrentShader();
	loadScene(SCENE, &cam, &objList, currShprg);
	

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	
	for(auto o:objList){
		o->setShader(currShprg);
		o->prepareObject();
	}
}

void cleanUp(void) {	
	printf("Running cleanUp function... ");
	// Free openGL resources
	// ...

	glutDestroyWindow(windowHandle);
	ShaderManager::destroyAllShaders();

	// Free meshes
	// ...
	
	printf("Done!\n\n");
}

int main(int argc, char **argv) {	
	// Setup freeGLUT
	unsigned int _glut_mode = GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
	#ifdef __APPLE__
	_glut_mode |= GLUT_3_2_CORE_PROFILE;
	#endif
	glutInitDisplayMode(_glut_mode);
	glutInit(&argc, argv);
	glutInitWindowSize(screen_width, screen_height);
	windowHandle = glutCreateWindow("OpenGL");
	glutDisplayFunc(display);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(keypress);
	glutKeyboardUpFunc(keyrelease);
	glutIdleFunc(update);


	//This is a freeglut define value. In MacOS GLUT, these functions do not exist.
	#ifdef GLUT_CORE_PROFILE 
  	glutInitContextVersion(4, 1);
  	glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);
	#endif
	
	glewExperimental = GL_TRUE;
	// Uses GLEW as OpenGL Loading Library to get access to modern core features as well as extensions
	GLenum err = glewInit(); 
	if (GLEW_OK != err) { fprintf(stdout, "Error: %s\n", glewGetErrorString(err)); return 1; }

	// Output OpenGL version info
	fprintf(stdout, "GLEW version: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, "OpenGL version: %s\n", (const char *)glGetString(GL_VERSION));
	fprintf(stdout, "OpenGL vendor: %s\n\n", glGetString(GL_VENDOR));

	init();
	
	// Object Count
	printf("Objects being rendered: %i\n", objList.size());
	GLint maxFragUniforms;
	glGetIntegerv(GL_MAX_FRAGMENT_UNIFORM_COMPONENTS, &maxFragUniforms);
	printf("Max fragment uniform components: %d\n", maxFragUniforms);

	glutMainLoop();	

	cleanUp();	
	return 0;
}
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
					Anim::addRotate(caddillac, glm::vec3(10.f*x, -10.f, 0.f));
					// Anim::addTranslate(caddillac, glm::vec3(0.f, 0.f, 5.00f), 40);
					objList->push_back(caddillac);
				}
			}
		}
		printf("Total cadillacs: %i\n", total);
	}
	if (scene == 903) { // SCENE_SYNTHWAVE
		srand(std::time({}));
		SceneObject::seed = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/15.0f));


		// === Camera Setup ===
		cam->position = {-30.f, 4.f, +15.f};
		cam->yaw(-45.f);

		// === Changeable Constants ===
		int const road_segments = 30;
		float const road_offset = +24.0f;
		float const carSpeed = 0.2f;

		// === Objects and Related Values ===
		SceneObject* caddillac = new SceneObject("../objects/Cadillac/Cadillac.obj");
		SceneObject* cubemap = new SceneObject("../objects/RoadSkybox/cubemap.obj");
		cubemap->scale = {680.f, 680.f, 680.f};
		cubemap->pos = {0.f, -300.f, 0.f};
		cubemap->setShader(ShaderManager::getCubemapShader());
		cubemap->fixShader = true;

		float roadLenght = 24.f;
		float roadWidth = 8.0f;
		float desertLenght = 8.f;

		caddillac->scale = {1.0f, 1.0f, 1.0f};
		caddillac->rot = {270.0f, 0.0f, 0.0f};
		caddillac->pos = {0.0f, 0.0f, 0.0f};

		// === Light Values ===
		glm::vec3 lightAmb = {0.1, 0.1, 0.1};
		glm::vec3 lightDiff = {1.0, 1.0, 0.7};
		glm::vec3 lightSpec = {1.0, 1.0, 0.7};
		glm::vec3 lightDir = {0.0, 0.0, -1.0};
		float LightIntensity = 1.0f;
		float lightCutoff = 90.f;
		float lampLightConstant = 1.0f;
		float lampLightLinear = 0.045f;
		float lampLightQuad = 0.0075f;
		float LightConstant = 1.0f;
		float LightLinear = 0.0007f;
		float LightQuad = 0.0002f;

		// Light X: -0.819298, Y: 0.688937, Z: -3.603488
		glm::vec3 headlightLPos = {-0.819298f, 0.605937f, -2.853488f};
		glm::vec3 headlightRPos = {-headlightLPos.x, headlightLPos.y, headlightLPos.z};

		headlightL = new Light(headlightLPos, lightAmb, lightDiff*LightIntensity, lightSpec*LightIntensity, lightDir, lightCutoff, LightConstant, LightLinear, LightQuad);
		headlightR = new Light(headlightRPos, lightAmb, lightDiff*LightIntensity, lightSpec*LightIntensity, lightDir, lightCutoff, LightConstant, LightLinear, LightQuad);

		glm::vec3 vec = {0.0f, 0.0f, carSpeed};
		for (int i = 0; i < road_segments; i++) {
			SceneObject* road = new SceneObject("../objects/RoadSkybox/road.obj");			
			road->pos = {0.0f, 0.0f, i*-24.0f + road_offset};
			glm::vec3 startPosRoad = {0.0f, 0.0f, (road_segments-1)*-24.0f + road_offset};
			Anim::addTranslate(road, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
			objList->push_back(road);

			for (int d = 0; d < 3; d++) // 3 deserts for each road generated
			{
				for (int w = 1; w <= 10; w++)
				{
					SceneObject* desertLeft = new SceneObject("../objects/RoadSkybox/desert.obj");
					SceneObject* desertRight = new SceneObject("../objects/RoadSkybox/desert.obj");
					
					desertLeft->pos = {-8.0f*w, 0.0f, i*-24.0f + road_offset + d*8.0f};
					desertRight->pos = {+8.0f*w, 0.0f, i*-24.0f + road_offset + d*8.0f};
					desertLeft->orgPos = desertLeft->pos;
					desertRight->orgPos = desertRight->pos;

					desertRight->rot = {0.0f, 180.0f, 0.0f};
					desertLeft->scale = {1.0f, 1.0f, 1.0f};
					desertRight->scale = {1.0f, 1.0f, 1.0f};
					Anim::addTranslate(desertLeft, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
					Anim::addTranslate(desertRight, vec, startPosRoad, glm::vec3(0.0f, 0.0f, road_offset+roadLenght));
					objList->push_back(desertLeft);
					objList->push_back(desertRight);

					desertLeft->setShader(ShaderManager::getPerlinShader());
					desertRight->setShader(ShaderManager::getPerlinShader());
					desertLeft->fixShader = true;
					desertRight->fixShader = true;
				}
				

			}

			if (i%2 == 0){
				SceneObject* lampLeft = new SceneObject("../objects/RoadSkybox/lamp.obj");
				SceneObject* lampRight = new SceneObject("../objects/RoadSkybox/lamp.obj");

				glm::vec3 LlampLightDir = {0.5f, -1.0f, 0.0f};
				glm::vec3 RlampLightDir = {-LlampLightDir.x, LlampLightDir.y, LlampLightDir.z};

				lampLeft->pos = {-4.2f, 3.0f, i*-24.0f + road_offset};
				Light* lampLeftLight = new Light({-4.2f, 10.0f, i*-24.0f + road_offset}, lightAmb, lightDiff, lightSpec, LlampLightDir, lightCutoff, lampLightConstant, lampLightLinear, lampLightQuad);

				lampRight->pos = glm::vec3(+4.2f, 3.0f, i*-24.0f + road_offset);
				Light* lampRightLight = new Light({+4.2f, 10.0f, i*-24.0f + road_offset}, lightAmb, lightDiff, lightSpec, RlampLightDir, lightCutoff, lampLightConstant, lampLightLinear, lampLightQuad);

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
	}
}
