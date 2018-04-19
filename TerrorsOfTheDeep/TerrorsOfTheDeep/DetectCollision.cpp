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
	}
}

