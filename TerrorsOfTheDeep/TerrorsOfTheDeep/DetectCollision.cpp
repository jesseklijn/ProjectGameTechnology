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
					float temp = obj1->getTransformedBoundingBox().getExtent().Y + obj2->getTransformedBoundingBox().getExtent().Y;
					float size = (temp * 0.43);
					//float size = 100;

					if (obj1 != obj2)
					{
						if (Col(obj1, obj2, size))
						{
							std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
							if (obj1->tag == GameObject::PLAYER)
							{
								switch (obj2->tag)
								{
								case GameObject::KEY:
									hasKey = true;
									pickedUp[0] = true;
									// enable 'has key'
									// destroy key (or make invisible or smth)
									break;
								case GameObject::CHEST:
									pickedUp[1] = hasKey;
									// if has key -> win
									// else nothing (or text?)
									break;
								case GameObject::WORLD_OBJECT:
									// collision resolution
									Resolve(obj1, obj2);
									break;
								case GameObject::GROUND:
									// vertical collision resolution
									break;
								case GameObject::MONSTER:
									pickedUp[2] = true;
									break;
								default:
									break;
								}
								//obj2->setPosition(obj2->getPosition() + vector3df(0, 300, 0));
							} else
							{
								switch (obj2->tag)
								{
								case GameObject::KEY:
									// same as world_object
								case GameObject::CHEST:
									// same as world_object
								case GameObject::WORLD_OBJECT:
									// collision resolution
									Resolve(obj1, obj2);
									break;
								case GameObject::GROUND:
									// vertical collision resolution
									break;
								default:
									break;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Resolve(GameObject* obj1, GameObject* obj2)
{
	vector3df currentVelocity = obj1->getVelocity();
	vector3df normal = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
	vector3df reflection = currentVelocity - Dot(currentVelocity, normal) * normal;
	if (obj1->tag == GameObject::PLAYER)
	{
		GameManager::smgr->getActiveCamera()->setPosition(obj1->getAbsolutePosition() + 5 * reflection);
	} else
	{
		obj1->setVelocity(reflection);
	}
}

float Dot(vector3df vector1, vector3df vector2)
{
	return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
}
//public static Vector3 Reflect(Vector3 vector, Vector3 normal)
//{
//	return vector - 2 * Vector3.Dot(vector, normal) * normal;
//}