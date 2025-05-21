#include "animations.h"
#include <GL/freeglut.h>
#include <cstdint> // for intptr_t
#include <stdio.h>
#include "mesh.h"

// Mesh

// void animateMeshRotationY(Mesh* mesh, float delta, int delay_ms) {
//     mesh->rot.y += delta;
//     glutTimerFunc(delay_ms, [](int){
//         animateMeshRotationY(mesh1, delta, delay_ms);
//     }, 0);
// }