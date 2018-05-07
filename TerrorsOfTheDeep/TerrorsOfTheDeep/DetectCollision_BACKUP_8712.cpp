#include "DetectCollision.h"



// simple collision code. 
bool DetectCollision::Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, float size) {
	return (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size);
}

void DetectCollision::fillInitialArray()
{
	if (!arrayFilled)
	{
		for (int k = 0; k < GameManager::gameObjects.size(); k++)
		{
			if (GameManager::gameObjects[k]->tag != GameObject::CREATURE)
			{
				oList.push_back(GameManager::gameObjects[k]);
			}
		}
		arrayFilled = true;
		std::cout << "Number of objects in oList: " << oList.size() << std::endl;
	}
}

void DetectCollision::getNearestObjectsFromPosition(GameObject* object, GameObject* objectTwo)
{
	float distance = (object->getAbsolutePosition() - objectTwo->getAbsolutePosition()).getLength();

	if (distance <= 1000)
	{
		nearestObjects.push_back(objectTwo);
	}
}

void DetectCollision::Detect(
	bool pickedUp[],
	irr::scene::ISceneManager* smgr
) {
	if (!arrayFilled)
	{
		fillInitialArray();
	}

	findNearestObjectsTimer = GameManager::Clamp(findNearestObjectsTimer - GameManager::deltaTimeMS, 0.0f, findNearestObjectsTime);

	if (findNearestObjectsTimer <= 0) {
		nearestObjects.clear();

		for (int i = 0; i < oList.size(); i++)
		{
			getNearestObjectsFromPosition(GameManager::FindGameObjectWithTag<GameObject>(GameObject::PLAYER, GameManager::gameObjects), oList[i]);
		}
		//std::cout << nearestObjects.size() << std::endl;
		findNearestObjectsTimer = findNearestObjectsTime;
	}

	if (colTime > 0) {
		colTime--;

		if (colTime <= 0) {
			allowCollision = true;
		}
	}

	if (allowCollision) {
		for (int i = 0; i < nearestObjects.size(); i++)
		{
			GameObject* obj1 = nearestObjects[i];


			if (obj1->tag == GameObject::PLAYER || obj1->tag == GameObject::MONSTER) {
				for (int j = 0; j < nearestObjects.size(); j++)
				{
					GameObject* obj2 = nearestObjects[j];
					float size = ((obj1->getTransformedBoundingBox().getExtent().X +
						(obj2->getTransformedBoundingBox().getExtent().X)) * 0.5f
						);

					//std::cout << "obj1: " << obj1->tag << " obj2: " << obj2->tag << " size: " << size << std::endl;

					if (obj1 != obj2)
					{
						if (Col(obj1, obj2, size))
						{
							//std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
							switch (obj1->tag) {
							case GameObject::PLAYER:
								switch (obj2->tag)
								{
								case GameObject::KEY:
									GameManager::keyPickedUp = true;
									obj2->mesh->remove();
									GameManager::gameObjects.erase(GameManager::gameObjects.begin() + j);
									break;
								case GameObject::CHEST:
									if (GameManager::keyPickedUp)
										GameManager::escaped = true;
									// Win!

									break;
								case GameObject::WORLD_OBJECT:
									// collision resolution
									// Resolve(obj1, obj2);
									break;
								case GameObject::MONSTER:
									GameManager::hasDied = true;
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
									// Resolve(obj1, obj2);
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

<<<<<<< HEAD

//void Resolve(GameObject* obj1, GameObject* obj2)
//{
//	vector3df currentVelocity = obj1->getVelocity();
//	float sizeVelocity = currentVelocity.getLength();
//	vector3df normal = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
//	vector3df reflection = currentVelocity - Dot(currentVelocity, normal) * normal;
//
//	vector3df direction = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
//	direction.normalize();
//	if (obj1->tag == GameObject::PLAYER)
//	{
//		//GameManager::smgr->getActiveCamera()->setPosition(GameManager::smgr->getActiveCamera()->getPosition() - 5 * currentVelocity);
//		GameManager::smgr->getActiveCamera()->setPosition(GameManager::smgr->getActiveCamera()->getPosition() - 5 * sizeVelocity * direction);
//
//	}
//	else
//	{
//		obj1->setPosition(obj1->getAbsolutePosition() - 5 * sizeVelocity * direction);
//		obj1->setVelocity(sizeVelocity * reflection.normalize());
//	}
//}
//
//float Dot(vector3df vector1, vector3df vector2)
//{
//	return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
//}


	DetectCollision::DetectCollision()
=======
/**
 * Resolves collision between a moving object 1 and a static object 2 by moving object 1 away
 */
void Resolve(GameObject* obj1, GameObject* obj2)
{
	vector3df currentVelocity = obj1->GetVelocity();
	float sizeVelocity = currentVelocity.getLength();
	vector3df normal = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
	vector3df reflection = currentVelocity - Dot(currentVelocity, normal) * normal;

	vector3df direction = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
	direction.normalize();
	if (obj1->tag == GameObject::PLAYER)
>>>>>>> origin/feature/fv-camera-rework
	{

	}

	DetectCollision::~DetectCollision()
	{
<<<<<<< HEAD
=======
		obj1->setPosition(obj1->getAbsolutePosition() - 5 * sizeVelocity * direction);
		obj1->SetVelocity(sizeVelocity * reflection.normalize());
>>>>>>> origin/feature/fv-camera-rework
	}

