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


// Initializations
Camera cam;
InputManager inputManager; // Handles keyboard input
Mesh *meshList = NULL; // Global pointer to linked list of triangle meshes
GLuint shprg; // Shader program id
int windowHandle;

// Constants
int key_delay = KEY_DELAY;
int screen_width = 1024;
int screen_height = 768;

int numLights = 0;
glm::vec3 lightWorldPos[MAX_LIGHTS];
ProjectionMode currentProjMode = PROJ_MODE;
GLenum currentPolygonMode = POLYGON_MODE;

// Global transform matrices
// V is the view transform
// P is the projection transform
// PV = P * V is the combined view-projection transform
glm::mat4 V, P;

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
	// glm::mat4 PV_M = PV * M;

	// Pass everything to the shader
	setMatrix4fv(shprg, "M", M);
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

void display(void) {
	Mesh *mesh;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	V = cam.getViewMatrix(); // moves world coordinates into camera space
	
	if (currentProjMode == PERSPECTIVE) {
		P = cam.getPerspectiveMatrix(screen_width, screen_height); // projects 3D points into 2D screen space
	} else {
		P = cam.getOrthographicMatrix(-20, 20, -10, 10);
	}

	// Select the shader program to be used during rendering 
	glUseProgram(shprg);

	setMatrix4fv(shprg, "V", V);
	setMatrix4fv(shprg, "P", P);
	setVector3fv(shprg, "camPos", cam.position);

	Light::sendLightsToShader(shprg);
	// setVector3fv(shprg, "lightPos", Light::getAllLights()[0].getPos());
	// setVector3fv(shprg, "lightAmbient", Light::getAllLights()[0].getAmb());
	// setVector3fv(shprg, "lightDiffuse", Light::getAllLights()[0].getDiff());
	// setVector3fv(shprg, "lightSpecular", Light::getAllLights()[0].getSpec());

	// Render all meshes in the scene
	mesh = meshList;
		
	while (mesh != NULL) {
		renderMesh(mesh);
		mesh = mesh->next;
	}
	glFlush();
}

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

	if (inputManager.isPressed('o')){
		
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
    glutPostRedisplay();
}



void init(void) {
	// Compile and link the given shader program (vertex shader and fragment shader)
	initShaders();
	shprg = getCurrentShader();
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	//glFrontFace(GL_CCW); //

	glEnable(GL_DEPTH_TEST);

	// Setup OpenGL buffers for rendering of the meshes
	Mesh * mesh = meshList;
	while (mesh != NULL) {
		prepareMesh(mesh);
		mesh = mesh->next;
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


// Include data for some triangle meshes (hard coded in struct variables)
#include "mesh_bunny.h"
#include "mesh_cow.h"
#include "mesh_cube.h"
#include "mesh_frog.h"
#include "mesh_knot.h"
#include "mesh_sphere.h"
#include "mesh_teapot.h"
#include "mesh_triceratops.h"

Mesh* anim1;
Mesh* anim2;
void animateMeshes(int delay_ms) {
    if (anim1) {
        anim1->rot.y += 2.f;
    }
    if (anim2) {
        anim2->rot.y -= 2.f;
    }
    glutPostRedisplay(); // Trigger redraw
    glutTimerFunc(delay_ms, animateMeshes, delay_ms); // Repeat timer
}

void loadScene(int scene){
	
	if (scene == 121) { // SCENE_DEFAULT_CAMERA
		glm::vec3 cowPos = {0.f, 0.f, 0.f};
		glm::vec3 cowRot = {0.f, 0.f, 0.f};
		glm::vec3 cowScale = {20.f, 20.f, 20.f};

		glm::vec3 triceratopsPos = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsRot = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsScale = {3.f, 3.f, 3.f};

		assert(cam.position.x == 0.0f);
		assert(cam.position.y == 0.0f);
		assert(cam.position.z == 20.0f);

		insertModel(&meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::RedRealistic());
		insertModel(&meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::GreenRealistic());
	}
	if (scene == 122) { // SCENE_GIVEN_CAMERA
		cam.position = {-5.f, -5.f, 20.f};
		assert(cam.position.x == -5.0f);
		assert(cam.position.y == -5.0f);
		assert(cam.position.z == 20.0f);
		cam.setRotation({-10.f, 30.f, 45.f});
		// cam.setRotation({-10.f, -30.f, -45.f});
		// cam.setRotation({10.f, 30.f, 45.f});

		glm::vec3 cowPos = {0.f, 0.f, 0.f};
		glm::vec3 cowRot = {0.f, 0.f, 0.f};
		glm::vec3 cowScale = {20.f, 20.f, 20.f};

		glm::vec3 triceratopsPos = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsRot = {0.f, 0.f, 0.f};
		glm::vec3 triceratopsScale = {3.f, 3.f, 3.f};

		insertModel(&meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::RedRealistic());
		insertModel(&meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::GreenRealistic());
	}
	if (scene == 131) { // SCENE_KNOT_ORTHOGONAL
		insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
	}
	if (scene == 132) { // SCENE_KNOT_ORTHOGONAL
		insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
	}
	if (scene == 141) { // SCENE_KNOT_COLORFULL
		insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0, Material::White());
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

		insertModel(&meshList, cow.nov, cow.verts, cow.nof, cow.faces, cowScale, cowPos, cowRot, Material::Brass());
		insertModel(&meshList, triceratops.nov, triceratops.verts, triceratops.nof, triceratops.faces, triceratopsScale, triceratopsPos, triceratopsRot, Material::RedRubber());
		insertModel(&meshList, teapot.nov, teapot.verts, teapot.nof, teapot.faces, teapotScale, teapotPos, teapotRot, Material::Silver());

		glm::vec3 lightPos = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos, glm::vec3(0.2, 0.2, 0.2), glm::vec3(0.9, 0.9, 0.7), glm::vec3(1.0, 1.0, 1.0));

		// insertModel(&meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, 60.0);
		// insertModel(&meshList, cube.nov, cube.verts, cube.nof, cube.faces, 5.0);
		// insertModel(&meshList, frog.nov, frog.verts, frog.nof, frog.faces, 2.5);
		// insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, 1.0);
		// insertModel(&meshList, sphere.nov, sphere.verts, sphere.nof, sphere.faces, 12.0);
	}
	if (scene == 250) { // SCENE_LIGHTS
		cam.position = {5.f, 7.f, 20.f};
		cam.setRotation({-13.f, 13.f, 0.f});

		glm::vec3 cubeScale = {4.f, 0.1f, 4.f};
		glm::vec3 cubePos = {-10.f, -1.f, -10.f};
		glm::vec3 cubeRot = {0.f, 0.f, 0.f};

		glm::vec3 knotScale = {0.4f, 0.4f, 0.4f};
		glm::vec3 knotPos = {0.f, 4.f, 0.f};
		glm::vec3 knotRot = {0.f, 0.f, 0.f};
	
		insertModel(&meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::White());
		insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, knotScale, knotPos, knotRot, Material::White());

		glm::vec3 lightPos1 = glm::vec3(-20.0, 4.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.1, 0.0, 0.0), glm::vec3(0.3, 0.0, 0.0), glm::vec3(0.2, 0.0, 0.0));

		glm::vec3 lightPos2 = glm::vec3(+20.0, 4.0, 10.0);
		Light l2 = Light(lightPos2, glm::vec3(0.0, 0.0, 0.1), glm::vec3(0.0, 0.0, 0.3), glm::vec3(0.0, 0.0, 0.2));
	}
	if (scene == 321) { // SCENE_ANIMATIONS
		cam.position = {5.f, 7.f, 20.f};
		cam.setRotation({+13.f, -13.f, 0.f});

		glm::vec3 cubeScale = {4.f*5, 0.1f*5, 4.f*5}; //*5 for original scaling
		glm::vec3 cubePos = {-10.f, -1.f, -10.f};
		glm::vec3 cubeRot = {0.f, 0.f, 0.f};

		glm::vec3 knotScale = {0.4f, 0.4f, 0.4f};
		glm::vec3 knotPos = {0.f, 4.f, 0.f};
		glm::vec3 knotRot = {0.f, 0.f, 0.f};

		glm::vec3 bunnyScale = {60.f, 60.f, 60.f};
		glm::vec3 bunnyPos = {0.f, 8.f, +1.f};
		glm::vec3 bunnyRot = {0.f, 0.f, 0.f};
	
		Mesh* rawCubeMesh = insertModel(&meshList, cube.nov, cube.verts, cube.nof, cube.faces, cubeScale, cubePos, cubeRot, Material::Brass());
		Mesh* rawKnotMesh = insertModel(&meshList, knot.nov, knot.verts, knot.nof, knot.faces, knotScale, knotPos, knotRot, Material::RedRealistic());
		Mesh* rawBunnyMesh = insertModel(&meshList, bunny.nov, bunny.verts, bunny.nof, bunny.faces, bunnyScale, bunnyPos, bunnyRot, Material::Obsidian());

		anim1 = rawKnotMesh;
		anim2 = rawBunnyMesh;
		glutTimerFunc(20, animateMeshes, 20); // 20ms = 50 FPS

		glm::vec3 lightPos1 = glm::vec3(-20.0, 10.0, 10.0);
		Light l1 = Light(lightPos1, glm::vec3(0.2, 0.1, 0.1), glm::vec3(0.9, 0.45, 0.35), glm::vec3(1.0, 1.0/2, 1.0/2));

		glm::vec3 lightPos2 = glm::vec3(+20.0, 10.0, 10.0);
		Light l2 = Light(lightPos2, glm::vec3(0.1, 0.1, 0.2), glm::vec3(0.45, 0.45, 0.7), glm::vec3(1.0/2, 1.0/2, 1.0));
	}
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

	loadScene(SCENE);

	init();
	glutMainLoop();

	cleanUp();	
	return 0;
}


