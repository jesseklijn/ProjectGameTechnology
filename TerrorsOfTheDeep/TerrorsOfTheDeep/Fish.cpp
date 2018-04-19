#pragma once
#include "Fish.h"
#pragma once
#include "GameManager.h"


// Constructor
Fish::Fish(const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture, bool detectCollision)
	: GameObject(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture, detectCollision)
{


}

Fish::~Fish()
{

}

void Fish::Move()
{
	

	setPosition(getPosition() + (direction.normalize() * (movementSpeed * GameManager::deltaTime)));
	if (mesh)
		mesh->setPosition(getPosition());

	setRotation(direction.getHorizontalAngle());
	if (mesh)
		mesh->setRotation(direction.getHorizontalAngle());
}

void Fish::ApplyFlocking(std::vector<Fish*> flockOfFishes, irr::core::vector3df goalPosition)
{
	irr::core::vector3df posA = getPosition();
	irr::core::vector3df vCentre = irr::core::vector3df(0, 0, 0);
	irr::core::vector3df vAvoid = irr::core::vector3df(0, 0, 0);
	float distance = 0; int groupSize = 0; float groupSpeed = 0;
	for (size_t i = 0; i < flockOfFishes.size(); i++)
	{

		irr::core::vector3df posB = flockOfFishes[i]->getPosition();
		//If the fish isn't this fish
		if (fishID != flockOfFishes[i]->fishID) {
			//Calculate distance
			distance = CalculateDistance(posA, posB);
			//If distance is within accepted range
			if (distance <= neighbourDistance) {

				vCentre += posB;
				groupSize++;

				if (distance < avoidDistance) {
					vAvoid += posA - posB;
				}

				groupSpeed += flockOfFishes[i]->movementSpeed;


			}
		}
	}
	//Apply behaviour if group is found
	if (groupSize > 0)
	{
		vCentre = vCentre / groupSize + (goalPosition - posA);
		movementSpeed = groupSpeed / groupSize;

		irr::core::vector3df dir = (vCentre + vAvoid) - posA;
		if (dir != irr::core::vector3df(0, 0, 0)) {

			direction = GameManager::Lerp(direction, dir, (rotationSpeed) * GameManager::deltaTime).normalize();
		}
	}
}

float Fish::CalculateDistance(irr::core::vector3df A, irr::core::vector3df B)
{
	//distance is (b.x-a.x)^2 + (b.y-a.y)^2
	auto distX = (B.X - A.X) * (B.X - A.X);
	auto distY = (B.Y - A.Y) * (B.Y - A.Y);
	auto distZ = (B.Z - A.Z) * (B.Z - A.Z);
	auto lengthRoot = distX + distY + distZ;


	return sqrt(lengthRoot);

}


