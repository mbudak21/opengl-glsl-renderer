#ifndef _SCENELOADER_H_
#define _SCENELOADER_H_


#include <cassert>
#include "camera.h"
#include "lights.h"
#include "animations.h"
#include <stdio.h>
#include "sceneObj.h"

void loadScene(int scene, Camera* cam, std::vector<SceneObject*>* objList, GLuint shprg);

#endif