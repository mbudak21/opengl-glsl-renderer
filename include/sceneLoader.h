#ifndef _SCENELOADER_H_
#define _SCENELOADER_H_


#include "mesh.h"

#include <cassert>
#include "camera.h"
#include "lights.h"
#include "animations.h"
#include "obj.h"
#include <stdio.h>

void loadScene(int scene, Camera* cam, _Mesh** meshList, std::vector<obj*>* objList, GLuint shprg);

#endif