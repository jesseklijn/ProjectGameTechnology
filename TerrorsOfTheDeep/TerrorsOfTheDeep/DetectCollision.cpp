#include "DetectCollision.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 10;


// simple collision code. 
bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, int size) {
	if (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size) {
		return true;
	}
	else { return false; }
}

		IrrlichtDevice *device =
			createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
				false, false, false, 0);

		if (colTime < 0) {
			allowCollision = true;
		}
	}
	shark->setPosition(shark->getPosition() + irr::core::vector3df(0, 0, -0.05));

	if (allowCollision) {
		if (Col(Player, win, 10)) {
			Player->setPosition(Player->getPosition() + irr::core::vector3df(0, 0, 0));

			if (hasKey) {
				//font->draw(L"Congrats You win xDDDDD", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
			else {
				//font->draw(L"Get a key you noob!", irr::core::rect<s32>(30, 100, 30, 100), irr::video::SColor(255, 255, 255, 255));
			}
		}
		else
		{
			Player->setPosition(Player->getPosition() + irr::core::vector3df(0, 0, 0.05));
		}
		if (Player&&key) {
			if (Col(key, Player, 10)) {
				hasKey = true;
				key->setPosition(key->getPosition() + irr::core::vector3df(10000, 10000, 10000));
			}
		}

		// collision code.
		if (Col(Player, shark, 10)) {
			// push player out of the way. 
			Player->setPosition(Player->getPosition() + irr::core::vector3df(rand() % 1 - 1, 0, 0));
		}



		return 0;
	}
