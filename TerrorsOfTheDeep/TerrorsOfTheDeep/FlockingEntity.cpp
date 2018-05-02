#pragma once
#include "FlockingEntity.h"
#include "GameManager.h"


FlockingEntity::FlockingEntity(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{
	
	SpawnFish();


}


FlockingEntity::~FlockingEntity()
{
}


void FlockingEntity::Update()
{
	// Run the Update() of our base class
	GameObject::Update();

	//Set the goal every now and then
	std::uniform_int_distribution<> chance(0, 1000);
	std::mt19937 eng(rd());// seed the generator
	if (chance(eng) <= 10) {
		std::uniform_int_distribution<> randomNumX(goalPosition.X - goalRadius, goalPosition.X + goalRadius);
		std::uniform_int_distribution<> randomNumY(goalPosition.Y - goalRadius, goalPosition.Y + goalRadius);
		std::uniform_int_distribution<> randomNumZ(goalPosition.Z - goalRadius, goalPosition.Z + goalRadius);

		goalPosition.X = randomNumX(eng);
		goalPosition.Y = randomNumY(eng);
		if (goalPosition.Y < 20) goalPosition.Y = 20;
		goalPosition.Z = randomNumZ(eng);

	}



	for (size_t i = 0; i < flockOfFishes.size(); i++)
	{
		flockOfFishes[i]->ApplyFlocking(flockOfFishes, goalPosition);
		/* Add a vector of length moveSpeed in the direction towards our target position*/
		flockOfFishes[i]->Move();
	}

}


void FlockingEntity::SpawnFish() {



	std::mt19937 eng(rd());// seed the generator
	// define the ranges for x y z starting positions
	std::uniform_int_distribution<> distrXPosition(-spawnRadius.X, spawnRadius.X);
	std::uniform_int_distribution<> distrYPosition(5, spawnRadius.Y);
	std::uniform_int_distribution<> distrZPosition(-spawnRadius.Z, spawnRadius.Z);
	// define the ranges for x y z starting scales (size of fish)
	std::uniform_int_distribution<> distrXScale(-fishSizeOffset.X, fishSizeOffset.X);
	std::uniform_int_distribution<> distrYScale(-fishSizeOffset.Y, fishSizeOffset.Y);
	std::uniform_int_distribution<> distrZScale(-fishSizeOffset.Z, fishSizeOffset.Z);


	for (size_t i = 0; i < amountOfFish; i++)
	{
		Fish* fish = new Fish(new vector3df(distrXPosition(eng), distrYPosition(eng), distrZPosition(eng)), new vector3df(1 + distrXScale(eng), 1 + distrYScale(eng), 1 + distrZScale(eng)), new vector3df(0, 0, 0),
			0, GameManager::smgr, -500 + i, GameManager::smgr->getMesh("../media/Fish.obj"),
			GameManager::driver->getTexture("../media/RockTexture.jpg"));
		fish->tag = GameObject::CREATURE;
		fish->movementSpeed = 5;
		fish->direction = goalPosition;
		flockOfFishes.push_back(fish);
		fish->fishID = i;
		GameManager::gameObjects.push_back(fish);
	}
}

