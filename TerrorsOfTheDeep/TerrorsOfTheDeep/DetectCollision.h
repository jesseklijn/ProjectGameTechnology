#pragma once
#include <irrlicht.h>
#include "GameObject.h"

bool Col(GameObject* objectOne, GameObject* objectTwo, int size);
void Detect(bool pickedUp[]);

void Resolve(GameObject* obj1, GameObject* obj2);
float Dot(irr::core::vector3df vector1, irr::core::vector3df vector2);
