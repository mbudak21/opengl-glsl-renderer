#ifndef _MESH_H_
#define _MESH_H_

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "material.h"


typedef struct _Triangle {
	int vInds[3]; //vertex indices
} Triangle;

typedef struct _Mesh { 
	int nv;
	glm::vec3 *vertices;
	glm::vec3 *vnorms;
	int nt;				
	Triangle *triangles;

    glm::vec3 pos;
    glm::vec3 rot;
    glm::vec3 scale;
	Material mat;

	struct _Mesh *next; 
		
	unsigned int vbo, ibo, vao; // OpenGL handles for rendering
} Mesh;

Mesh* insertModel(Mesh** list, int nv, float* vArr, int nt, int* tArr, float scale, Material mat);
Mesh* insertModel(Mesh** list, int nv, float* vArr, int nt, int* tArr, glm::vec3 Scale, glm::vec3 Pos, glm::vec3 Rot, Material mat);

glm::vec3 getScale();
void setScale(glm::vec3 scale);

glm::vec3 getPos();
void setPos(glm::vec3 pos);

glm::vec3 getRot();
void setRot(glm::vec3 rot);
#endif


