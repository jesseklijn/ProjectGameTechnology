#pragma once
#include <irrlicht.h>

using namespace irr;

bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, int size);
void Detect(
	irr::scene::ISceneNode* player,
	irr::scene::ISceneNode* win,
	irr::scene::ISceneNode* key,
	irr::scene::ISceneNode* shark,
	irr::scene::ISceneNode* rock,
	irr::scene::ISceneNode* rock1,
	irr::scene::ISceneNode* rock2,
	irr::scene::ISceneNode* rock3,
	irr::scene::ISceneNode* rock4,
	bool pickedUp[], 
	irr::scene::ISceneManager* smgr
);

