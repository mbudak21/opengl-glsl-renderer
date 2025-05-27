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
#include "sceneLoader.h"
#include <chrono>
#include "sceneObj.h"

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

auto lastTime = std::chrono::high_resolution_clock::now();
int numLights = 0;
glm::vec3 lightWorldPos[MAX_LIGHTS];
ProjectionMode currentProjMode = PROJ_MODE;
GLenum currentPolygonMode = POLYGON_MODE;

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

void cleanUp(void);

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
	Anim::update(frameCount * ANIMATION_SPEED);
    glutPostRedisplay();
}


int fpsCounter = 0;
float fpsTimer = 0.0f;

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

	glutMainLoop();	

	cleanUp();	
	return 0;
}


