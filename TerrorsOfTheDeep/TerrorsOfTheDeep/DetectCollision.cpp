#include "DetectCollision.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 100;


// simple collision code. 
bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, int size) {
	if (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size) {
		return true;
	}
	else { return false; }
}

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
){
		

	if (colTime > 0) {
		colTime--;

		if (colTime <= 0) {
			allowCollision = true;
		}
	}
	
	//shark->setPosition(shark->getPosition() + irr::core::vector3df(0, 0, -0.05));

	if (allowCollision) {
		
		if (Col(player, win, 90)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(0.25, 0.25, 0.25));

			if (pickedUp[0]) {
				pickedUp[1] = true;
				//font->draw(L"Win Text", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
			else {
				//font->draw(L"Key text", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
		}

		if (Col(key, player, 50)) {
			pickedUp[0] = true;
			key->setScale(core::vector3df(0.01, 0.01, 0.01));
			//key->setPosition(key->getPosition() + core::vector3df(10000, 10000, 10000));
		}

		if (Col(player, shark, 30)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() +irr::core::vector3df(rand() % 2 - 1, 0, 0));
			pickedUp[2] = true;
		}
		if (Col(player, rock, 150)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}
		if (Col(player, rock1, 150)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}
		if (Col(player, rock2, 150)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}
		if (Col(player, rock3, 150)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}
		if (Col(player, rock4, 150)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}
	}
}

