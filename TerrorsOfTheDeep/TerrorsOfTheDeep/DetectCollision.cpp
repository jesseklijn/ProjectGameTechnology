#include "DetectCollision.h"
#include "GameManager.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 100;


// simple collision code. 
bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, float size) {
	return (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size);
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
		
		for(int i = 0; i < GameManager::gameObjects.size(); i++)
		{
			GameObject* obj1 = GameManager::gameObjects[i];

			for(int j = i+1; j < GameManager::gameObjects.size(); j++)
			{
				GameObject* obj2 = GameManager::gameObjects[j];
				float temp = obj1->getTransformedBoundingBox().getExtent().X + obj2->getTransformedBoundingBox().getExtent().X;
				float size = (temp * 0.43);

				
				if (obj1 != obj2)
				{
					if (Col(obj1, obj2, size))
					{
						std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
					}
				}
			}
		}


		/*
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
			key->setPosition(key->getPosition() + irr::core::vector3df(10000, 10000, 10000));
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
		*/
	}
}

