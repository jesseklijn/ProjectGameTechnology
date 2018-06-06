#pragma once
#include <irrlicht.h>
#pragma once
#include "GameManager.h"

class DetectCollision {
public:

	DetectCollision();
	~DetectCollision();

	bool lol = false;

	bool hasKey = false;
	bool allowCollision = false;
	int colTime = 100;
	bool arrayFilled = false;
	std::vector<GameObject*> oList;
	std::vector<GameObject*> nearestObjects;

	float findNearestObjectsTime = 5.0f * 1000.0f;
	float findNearestObjectsTimer = 0.0f;


	void fillInitialArray();
	void getNearestObjectsFromPosition(GameObject* object, GameObject* objectTwo);
	bool Col(irr::scene::ISceneNode* objectOne, irr::scene::ISceneNode* objectTwo, float size);
	void Detect(irr::scene::ISceneManager* smgr);

	void Resolve(GameObject* obj1, GameObject* obj2);
	float Dot(irr::core::vector3df vector1, irr::core::vector3df vector2);
	void ResetArray();
};

