#include "DetectCollision.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 3000;


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
		
		if (Col(player, win, 20)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(0.25, 0.25, 0.25));

			if (hasKey) {
				//font->draw(L"Congrats You win xDDDDD", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
			else {
				//font->draw(L"Get a key you noob!", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
		}

		if (Col(key, player, 20)) {
			hasKey = true;
			key->setPosition(key->getPosition() + irr::core::vector3df(10000, 10000, 10000));
		}

		if (Col(player, shark, 30)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() +irr::core::vector3df(rand() % 2 - 1, 0, 0));
		}
		if (Col(player, rock, 40)) {
			smgr->getActiveCamera()->setPosition(smgr->getActiveCamera()->getPosition() + irr::core::vector3df(1, 1, 1));
		}

	}
}
