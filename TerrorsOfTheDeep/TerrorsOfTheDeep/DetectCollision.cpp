#include "DetectCollision.h"
#include "GameManager.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 100;


// simple collision code. 
bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, float size) {
	vector3df dif = objectOne->getAbsolutePosition() - objectTwo->getAbsolutePosition();
	float difSize = sqrt(dif.X * dif.X + dif.Y * dif.Y + dif.Z * dif.Z);
	return (difSize < size);
	//return (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size);
}


void Detect(bool pickedUp[]) 
{
	if (colTime > 0) {
		colTime--;

		if (colTime <= 0) {
			allowCollision = true;
		}
	}
	
	//shark->setPosition(shark->getPosition() + irr::core::vector3df(0, 0, -0.05));

	if (allowCollision) {
		
		for (int i = 0; i < GameManager::gameObjects.size(); i++)
		{
			GameObject* obj1 = GameManager::gameObjects[i];
			if (obj1->tag == GameObject::PLAYER || obj1->tag == GameObject::MONSTER) {

				for (int j = 1; j < GameManager::gameObjects.size(); j++)
				{
					GameObject* obj2 = GameManager::gameObjects[j];
					float temp = obj1->getTransformedBoundingBox().getExtent().X + obj2->getTransformedBoundingBox().getExtent().X;
					//float size = (temp * 0.43);
					float size = 100;

					if (obj1 != obj2)
					{
						if (Col(obj1, obj2, size))
						{
							std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
							if (obj1->tag == GameObject::PLAYER && obj2->tag == GameObject::KEY)
							{
								obj2->setScale(vector3df(0.1));
							}
						}
					}
				}
			}
		}
	}
}

