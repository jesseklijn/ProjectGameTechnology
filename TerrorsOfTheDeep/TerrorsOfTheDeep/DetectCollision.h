#pragma once
#include <irrlicht.h>

using namespace irr;

bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, int size);
void Detect(
	irr::scene::ISceneNode* Player,
	irr::scene::ISceneNode* win,
	irr::scene::ISceneNode* key,
	irr::scene::ISceneNode* shark
);

