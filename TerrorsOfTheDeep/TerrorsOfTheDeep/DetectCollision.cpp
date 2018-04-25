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
){
	if (!arrayFilled)
	{
		fillInitialArray();
	}

	findNearestObjectsTimer = GameManager::Clamp(findNearestObjectsTimer - GameManager::deltaTimeMS, 0.0f, findNearestObjectsTime);

	if (findNearestObjectsTimer <= 0) {
		nearestObjects.clear();

		for (int i = 0; i < oList.size(); i++)
		{
			getNearestObjectsFromPosition(GameManager::FindGameObjectWithTag(GameObject::PLAYER), oList[i]);
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
							std::cout << obj1->tag << " collides with " << obj2->tag << " size: " << size << std::endl;
						}
					}
				}
			}
		}
	}
}

DetectCollision::DetectCollision()
{
}

DetectCollision::~DetectCollision()
{
}

