#include "DetectCollision.h"
#include "GameManager.h"

bool hasKey = false;
bool allowCollision = false;
int colTime = 100;


// simple collision code.
bool Col(GameObject* obj1, GameObject* obj2, float size)
{
	//vector3df pos1 = obj1->getAbsolutePosition();
	//vector3df pos2 = obj2->getAbsolutePosition();

	//float sizeX = obj1->mesh->getTransformedBoundingBox().getExtent().X + obj2->mesh->getTransformedBoundingBox().getExtent().X;
	//if (std::abs(pos1.X - pos2.X) <  0.45 * sizeX)
	//{
	//	float sizeY = obj1->mesh->getTransformedBoundingBox().getExtent().Y + obj2->mesh->getTransformedBoundingBox().getExtent().Y;
	//	if (std::abs(pos1.Y - pos2.Y) < 0.1 * sizeY)
	//	{
	//		float sizeZ = obj1->mesh->getTransformedBoundingBox().getExtent().Z + obj2->mesh->getTransformedBoundingBox().getExtent().Z;
	//		if (std::abs(pos1.Z - pos2.Z) < 0.45 * sizeZ)
	//		{
	//			return true;
	//		}
	//	}
	//}
	//return false;

	vector3df dif = obj1->getAbsolutePosition() - obj2->getAbsolutePosition();
	float difSize = sqrt(std::abs(dif.X * dif.X + dif.Y * dif.Y + dif.Z * dif.Z));
	return (difSize < size);
}


void Detect(bool pickedUp[]) 
{
	if (colTime > 0) {
		colTime--;

		if (colTime <= 0) {
			allowCollision = true;
		}
	}
	
	if (allowCollision) {
		
		for (int i = 0; i < GameManager::gameObjects.size(); i++)
		{
			GameObject* obj1 = GameManager::gameObjects[i];
			if (obj1->tag == GameObject::MONSTER || obj1->tag == GameObject::PLAYER) {
				
				for (int j = 1; j < GameManager::gameObjects.size(); j++)
				{
					if (j != i)
					{
						GameObject* obj2 = GameManager::gameObjects[j];
						float size = 100;
						if (obj2->tag == GameObject::WORLD_OBJECT)
						{
							size = 150;
						}
						if (obj1->tag == GameObject::MONSTER || obj2->tag == GameObject::MONSTER)
						{
							size = 750;
						}

						if (Col(obj1, obj2, size))
						{
							//std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
							switch (obj1->tag) {
							case GameObject::PLAYER:
								switch (obj2->tag)
								{
								case GameObject::KEY:
									hasKey = true;
									pickedUp[0] = true;
									obj2->mesh->remove();
									break;
								case GameObject::CHEST:
									pickedUp[1] = hasKey;
									// Win!
									break;
								case GameObject::WORLD_OBJECT:
									// collision resolution
									Resolve(obj1, obj2);
									break;
								case GameObject::MONSTER:
									pickedUp[2] = true;
									// Die!
									break;
								default:
									break;
								}
								break;
							case GameObject::MONSTER:
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
								default:
									break;
								}
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

void Resolve(GameObject* obj1, GameObject* obj2)
{
	vector3df currentVelocity = obj1->getVelocity();
	float sizeVelocity = currentVelocity.getLength();
	vector3df normal = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
	vector3df reflection = currentVelocity - Dot(currentVelocity, normal) * normal;
	if (obj1->tag == GameObject::PLAYER)
	{
		GameManager::smgr->getActiveCamera()->setPosition(GameManager::smgr->getActiveCamera()->getPosition() - 5 * currentVelocity);
	} else
	{
		obj1->setPosition(obj1->getAbsolutePosition() - currentVelocity);
		obj1->setVelocity(sizeVelocity * reflection.normalize());
	}
}

float Dot(vector3df vector1, vector3df vector2)
{
	return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
}
