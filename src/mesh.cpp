#include <stdlib.h>
#include "mesh.h"
#include "features.h"
#include "material.h"
#include <stdio.h>
#include <shaders.h>

Mesh* insertModel(Mesh** list, int nv, float* vArr, int nt, int* tArr, float scale, Material mat) {
	glm::vec3 Scale = {scale, scale, scale};
	glm::vec3 Pos = {0.f, 0.f, 0.f};
	glm::vec3 Rot = {0.f, 0.f, 0.f};

	return insertModel(list, nv, vArr, nt, tArr, Scale, Pos, Rot, mat);
}

Mesh* insertModel(Mesh** list, int nv, float* vArr, int nt, int* tArr, glm::vec3 Scale, glm::vec3 Pos, glm::vec3 Rot, Material material) {
	Mesh* mesh = (Mesh*)malloc(sizeof(Mesh));
	mesh->nv = nv;
	mesh->nt = nt;
	mesh->scale = Scale;
	mesh->pos = Pos;
	mesh->rot = Rot;
	mesh->mat = material;
	mesh->vertices = (glm::vec3*)malloc(nv * sizeof(glm::vec3));
	mesh->vnorms = (glm::vec3*)malloc(nv * sizeof(glm::vec3));
	mesh->triangles = (Triangle*)malloc(nt * sizeof(Triangle));

	// set mesh vertices
	for (int i = 0; i < nv; i++) {
		mesh->vertices[i].x = vArr[i * 3];
		mesh->vertices[i].y = vArr[i * 3 + 1];
		mesh->vertices[i].z = vArr[i * 3 + 2];
	}

	// set mesh triangles
	for (int i = 0; i < nt; i++) {
		mesh->triangles[i].vInds[0] = tArr[i * 3];
		mesh->triangles[i].vInds[1] = tArr[i * 3 + 1];
		mesh->triangles[i].vInds[2] = tArr[i * 3 + 2];
	}

	glm::vec3 init_vec = {0.f, 0.f, 0.f};
	for (int i = 0; i < nv; i++) {
		mesh->vnorms[i] = init_vec;
	}
	for (int i = 0; i < nt; i++) {
		int i0 = mesh->triangles[i].vInds[0];
		int i1 = mesh->triangles[i].vInds[1];
		int i2 = mesh->triangles[i].vInds[2];
	
		glm::vec3 v0 = mesh->vertices[i0];
		glm::vec3 v1 = mesh->vertices[i1];
		glm::vec3 v2 = mesh->vertices[i2];
	
		glm::vec3 edge1 = v1 - v0; // v1 - v0
		glm::vec3 edge2 = v2 - v0; // v2 - v0

		glm::vec3 faceNormal = glm::normalize(glm::cross(edge1, edge2));
		// printf("faceNormal: %f, %f, %f\n", faceNormal.x, faceNormal.y, faceNormal.z);
	
		mesh->vnorms[i0] = (mesh->vnorms[i0] + faceNormal);
		mesh->vnorms[i1] = (mesh->vnorms[i1] + faceNormal);
		mesh->vnorms[i2] = (mesh->vnorms[i2] + faceNormal);
	}
	for (int i = 0; i < nv; i++) {
		mesh->vnorms[i] = glm::normalize(mesh->vnorms[i]);
		// printf("Vnorm: %f, %f, %f\n", mesh->vnorms[i].x, mesh->vnorms[i].y, mesh->vnorms[i].z);
		// printf("Vnorm: %f\n", mesh->vnorms[i].x);
	}
	mesh->next = *list;
	*list = mesh;
	return mesh; // Return the current mesh
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

// void renderMesh(Mesh *mesh) {
// 	// Matrix M = Translate(pos) * Rotate(rot) * Scale(scale);
// 	glm::mat4 T = glm::translate(glm::mat4(1.0f), mesh->pos);
// 	glm::mat4 Rx = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.x), glm::vec3(1, 0, 0));
// 	glm::mat4 Ry = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.y), glm::vec3(0, 1, 0));
// 	glm::mat4 Rz = glm::rotate(glm::mat4(1.0f), glm::radians(mesh->rot.z), glm::vec3(0, 0, 1));
// 	glm::mat4 S = glm::scale(glm::mat4(1.0f), mesh->scale);
	
// 	glm::mat4 R = Rz * Ry * Rx;
// 	glm::mat4 M = T * R * S;

// 	setMatrix4fv(shprg, "M", M);
// 	setMatrix4fv(shprg, "PVM", P*V*M);
// 	setVector3fv(shprg, "matAmb", mesh->mat.getAmb());
// 	setVector3fv(shprg, "matDiff", mesh->mat.getDiff());
// 	setVector3fv(shprg, "matSpec", mesh->mat.getSpec());
// 	setFloat1f(shprg, "matShininess", mesh->mat.getShininess());

// 	// Select current resources
// 	glBindVertexArray(mesh->vao);
	
// 	glPolygonMode(GL_FRONT_AND_BACK, currentPolygonMode); 

// 	// Draw all triangles
// 	glDrawElements(GL_TRIANGLES, mesh->nt * 3, GL_UNSIGNED_INT, NULL); 
// 	glBindVertexArray(0);
// }
