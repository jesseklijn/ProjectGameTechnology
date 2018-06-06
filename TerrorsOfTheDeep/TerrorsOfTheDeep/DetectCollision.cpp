#include "DetectCollision.h"
#include "SceneManager.h"

// simple collision code. 
bool DetectCollision::Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, float size) 
{
	if (objectOne && objectTwo)
		return (objectOne->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition()) < size);

	return false;
}

void DetectCollision::fillInitialArray()
{
	if (!arrayFilled)
	{
		for (GameObject* gameObject : GameManager::gameObjects)
		{
			if (gameObject != nullptr && gameObject->tag != GameObject::CREATURE)
			{
				oList.push_back(gameObject);
			}
		}
		arrayFilled = true;
		//std::cout << "Number of objects in oList: " << oList.size() << std::endl;
	}
}

void DetectCollision::getNearestObjectsFromPosition(GameObject* object, GameObject* objectTwo)
{
	if (object == nullptr || objectTwo == nullptr)
		return;

	float distance = GameManager::smgr->getActiveCamera()->getAbsolutePosition().getDistanceFrom(objectTwo->getAbsolutePosition());
	//float distance = (object->getAbsolutePosition() - objectTwo->getAbsolutePosition()).getLength();

	if (distance <= 5000)
	{
		nearestObjects.push_back(objectTwo);
	}
}

void DetectCollision::Detect(irr::scene::ISceneManager* smgr) {
	if (!arrayFilled)
	{
		fillInitialArray();
	}

	findNearestObjectsTimer = GameManager::Clamp(findNearestObjectsTimer - GameManager::deltaTimeMS, 0.0f, findNearestObjectsTime);

	if (findNearestObjectsTimer <= 0) {
		nearestObjects.clear();

		for (int i = 0; i < oList.size(); i++)
		{
			getNearestObjectsFromPosition(GameManager::levelPlayer, oList[i]);
		}
		//std::cout << "nearestObjects size: " << nearestObjects.size() << std::endl;
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


			if (obj1 != nullptr && (obj1->tag == GameObject::PLAYER || obj1->tag == GameObject::MONSTER)) {
				for (int j = 0; j < nearestObjects.size(); j++)
				{
					GameObject* obj2 = nearestObjects[j];

					if (obj2 == nullptr)
						continue;

					// Temporary fix to be replaced by mesh collision
					float size = 500;
					if (obj1->tag == GameObject::MONSTER || obj2->tag == GameObject::MONSTER)
					{
						size = 1000;
					}

					//std::cout << "obj1: " << obj1->tag << " obj2: " << obj2->tag << " size: " << size << std::endl;

					if (obj1 != obj2)
					{
						if (Col(obj1, obj2, size))
						{
							//std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
							int i = 0;
							switch (obj1->tag) {
							case GameObject::PLAYER:
								switch (obj2->tag)
								{
								case GameObject::KEY:
									GameManager::keyPickedUp = true;
									obj2->mesh->remove();
									i = GameManager::FindIndexInList(obj2, GameManager::gameObjects);
									GameManager::gameObjects[i] = nullptr;
									i = GameManager::FindIndexInList(obj2, oList);
									oList[i] = nullptr;
									i = GameManager::FindIndexInList(obj2, nearestObjects);
									nearestObjects[i] = nullptr;
									break;
								case GameObject::CHEST:
									if (GameManager::keyPickedUp)
										GameManager::escaped = true;
									break;
								case GameObject::WORLD_OBJECT:
									GameManager::levelPlayer->NotifyCollision(obj2->getAbsolutePosition());
									break;
								case GameObject::MONSTER:
									GameManager::hasDied = true;
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
									obj1->NotifyCollision(obj2->getAbsolutePosition());
									//Resolve(obj1, obj2);
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

//void DetectCollision::Resolve(GameObject* obj1, GameObject* obj2)
//{
//	vector3df currentVelocity = obj1->GetVelocity();
//	float sizeVelocity = currentVelocity.getLength();
//	vector3df normal = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
//	vector3df reflection = currentVelocity - Dot(currentVelocity, normal) * normal;
//
//	vector3df direction = obj2->getAbsolutePosition() - obj1->getAbsolutePosition();
//	direction.normalize();
//
//	obj1->setPosition(obj1->getAbsolutePosition() - 5 * sizeVelocity * direction);
//	obj1->SetVelocity(sizeVelocity * reflection.normalize());
//}
//
//float DetectCollision::Dot(vector3df vector1, vector3df vector2)
//{
//	return vector1.X * vector2.X + vector1.Y * vector2.Y + vector1.Z * vector2.Z;
//}

void DetectCollision::ResetArray()
{
	arrayFilled = false;
	oList.clear();
	nearestObjects.clear();
}



DetectCollision::DetectCollision()
{

}

DetectCollision::~DetectCollision()
{
}
