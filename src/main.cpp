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
#include "mesh.h"
#include "constants.h"
#include <glm/gtc/type_ptr.hpp>
#include "inputManager.h"
#include "lights.h"
#include "animations.h"
#include "sceneLoader.h"
#include <chrono>
#include "obj.h"

// Initializations
Camera cam;
InputManager inputManager; // Handles keyboard input
Mesh *meshList = NULL; // Global pointer to linked list of triangle meshes
static std::vector<obj*> objList; // Global pointer to list of Obj's
GLuint shprg; // Shader program id
int windowHandle;

// Constants
int key_delay = KEY_DELAY;
int screen_width = 1024;
int screen_height = 768;

auto g_lastTime = std::chrono::high_resolution_clock::now();
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
    float dt = std::chrono::duration<float>(now - g_lastTime).count();
    g_lastTime = now;

    // === FPS calculation ===
    static int   fpsCount = 0;
    static float fpsTimer = 0.0f;
    fpsCount++;
    fpsTimer += dt;
    if (fpsTimer >= 0.2f) {
        float fps = fpsCount / fpsTimer;
        printf("FPS: %.1f\n", fps);
        fpsCount = 0;
        fpsTimer = 0.0f;
    }
    // =============================
	
	if (key_delay > 0) {
		key_delay -= 1;
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
	if (inputManager.isPressed('m') && key_delay <= 0) {
		key_delay = KEY_DELAY;
		if (currentPolygonMode == GL_POINT) {currentPolygonMode = GL_LINE;}
		else if (currentPolygonMode == GL_LINE){currentPolygonMode = GL_FILL;}
		else if (currentPolygonMode == GL_FILL){currentPolygonMode = GL_POINT;}
	}
	// Projection Modes
	if (inputManager.isPressed('1')) currentProjMode = PERSPECTIVE;
	if (inputManager.isPressed('2')) currentProjMode = ORTHOGONAL;

	// Shader Changes
	if (inputManager.isPressed('b') && key_delay <= 0) {
		useNextShader();
		shprg = getCurrentShader();
		key_delay = KEY_DELAY;
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
	Anim::update(dt);
    glutPostRedisplay();
}


void prepareMesh(Mesh *mesh) {
	int sizeVerts = mesh->nv * 3 * sizeof(float);
	int sizeCols  = mesh->nv * 3 * sizeof(float);
	int sizeTris = mesh->nt * 3 * sizeof(int);
	
	// For storage of state and other buffer objects needed for vertex specification
	glGenVertexArrays(1, &mesh->vao);
	glBindVertexArray(mesh->vao);

	// Allocate VBO and load mesh data (vertices and normals)
	glGenBuffers(1, &mesh->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeVerts + sizeCols, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeVerts, (void *)mesh->vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeVerts, sizeCols, (void *)mesh->vnorms);

	// Allocate index buffer and load mesh indices
	glGenBuffers(1, &mesh->ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeTris, (void *)mesh->triangles, GL_STATIC_DRAW);

	// Bind vPos to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Bind vNorm to location 1 (after vertex data)
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)(mesh->nv * 3 * sizeof(float)));
	glBindVertexArray(0);
}

void renderMesh(Mesh *mesh) {
	// Matrix M = Translate(pos) * Rotate(rot) * Scale(scale);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), mesh->pos);
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.x), glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.y), glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.z), glm::vec3(0, 0, 1));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), mesh->scale);
	
	glm::mat4 R = Rz * Ry * Rx;
	glm::mat4 M = T * R * S;

	setMatrix4fv(shprg, "M", M);
	setMatrix4fv(shprg, "PVM", P*V*M);
	setVector3fv(shprg, "matAmb", mesh->mat.getAmb());
	setVector3fv(shprg, "matDiff", mesh->mat.getDiff());
	setVector3fv(shprg, "matSpec", mesh->mat.getSpec());
	setFloat1f(shprg, "matShininess", mesh->mat.getShininess());

	// Select current resources
	glBindVertexArray(mesh->vao);
	
	glPolygonMode(GL_FRONT_AND_BACK, currentPolygonMode); 

	// Draw all triangles
	glDrawElements(GL_TRIANGLES, mesh->nt * 3, GL_UNSIGNED_INT, NULL); 
	glBindVertexArray(0);
}



void prepareObject(obj* o){
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

	printf("OBJ Diagnostics:\n");
	printf("  Materials:       %d\n", obj_num_mtrl(o));
	printf("  Vertices:        %d\n", obj_num_vert(o));
	printf("  Surfaces:        %d\n", obj_num_surf(o));

	for (int si = 0; si < obj_num_surf(o); ++si) {
		printf("    Surface %d: Polygons = %d, Lines = %d\n",
			si,
			obj_num_poly(o, si),
			obj_num_line(o, si));
	}
}

void renderObject(obj* o){

	glm::vec3 pos = {-1.f, -1.f, 0.f};
	glm::vec3 rot = {-90.f, 215.f, 0.f};
	glm::vec3 scale = {1.f, 1.f, 1.f};

	// Matrix M = Translate(pos) * Rotate(rot) * Scale(scale);
	glm::mat4 T = glm::translate(glm::mat4(1.0f), pos);
	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1, 0, 0));
	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0, 1, 0));
	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(rot.z), glm::vec3(0, 0, 1));
	glm::mat4 S = glm::scale(glm::mat4(1.0f), scale);
	
	glm::mat4 R = Rz * Ry * Rx;	
	glm::mat4 M = T * R * S;
	// glm::mat4 PV_M = PV * M;

	glUseProgram(shprg);
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

void display(void) {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	V = cam.getViewMatrix(); // moves world coordinates into camera space
	
	if (currentProjMode == PERSPECTIVE) {
		P = cam.getPerspectiveMatrix(screen_width, screen_height); // projects 3D points into 2D screen space
	} else {
		P = cam.getOrthographicMatrix(-20, 20, -10, 10);
	}

	// Select the shader program to be used during rendering 
	glUseProgram(shprg);

	// Pass the uniforms which are the same for all meshes/objects
	setMatrix4fv(shprg, "PV", P*V);
	setMatrix4fv(shprg, "P", P);
	setMatrix4fv(shprg, "V", V);
	setVector3fv(shprg, "camPos", cam.position);

	Light::sendLightsToShader(shprg);

	// Render all meshes
	Mesh* mesh;
	mesh = meshList;
		
	while (mesh != NULL) {
		renderMesh(mesh);
		mesh = mesh->next;
	}
	for(auto o:objList){
		renderObject(o);
	}
	glFlush();
}


void init(void) {
	// Compile and link the given shader program (vertex shader and fragment shader)
	initShaders();
	shprg = getCurrentShader();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CCW);

	glEnable(GL_DEPTH_TEST);
	
	// Setup OpenGL buffers for rendering
	Mesh * mesh = meshList;
	while (mesh != NULL) {
		prepareMesh(mesh);
		mesh = mesh->next;
	}
	for(auto o:objList){
		prepareObject(o);
	}
}

void cleanUp(void) {	
	printf("Running cleanUp function... ");
	// Free openGL resources
	// ...

	glutDestroyWindow(windowHandle);
	destroyAllShaders();

	// Free meshes
	// ...
	
	printf("Done!\n\n");
}

int main(int argc, char **argv) {	
	// Setup freeGLUT
	unsigned int _glut_mode = GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH;
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

	
	loadScene(SCENE, &cam, &meshList, &objList, shprg);

	init();

	// Mesh Count
	int meshCount;
	Mesh* start = meshList;
	for (meshCount = 0; start != NULL; meshCount++){
		start = start->next;
	};
	printf("Meshes being rendered: %i\n", meshCount);

	// Object Count
	printf("Objects being rendered: %i\n", objList.size());

	glutMainLoop();	

	cleanUp();	
	return 0;
}


