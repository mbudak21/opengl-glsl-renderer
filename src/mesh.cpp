#include <stdlib.h>
#include "mesh.h"
#include "features.h"
#include "material.h"
#include <stdio.h>

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
