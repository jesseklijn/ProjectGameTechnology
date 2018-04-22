#pragma once
#include <irrlicht.h>
#include "GameObject.h"

using namespace irr;

bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, int size);
void Detect(bool pickedUp[]);
void Resolve(GameObject* obj1, GameObject* obj2);
float Dot(core::vector3df vector1, core::vector3df vector2);
