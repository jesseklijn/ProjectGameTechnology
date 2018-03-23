#pragma once
#include "Shark.h"
#pragma once
#include "GameManager.h"

// Constructor
Shark::Shark(const irr::core::vector3df* startPosition,
				const irr::core::vector3df* startScale,
				const irr::core::vector3df* startRotation,
				irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
				irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture)
				: Monster(startPosition, startScale, startRotation, parent, mgr, id, relatedMesh, relatedTexture)
{
	// Sets the movement speed of our shark for now
	moveSpeed = 1.0f;
}

// Destructor
Shark::~Shark()
{

}

/* Overridden (from GameObject - Monster)

Main player chasing code for vertical slice. Since the player
is parented to the camera, the main camera's position is used.*/
void Shark::Update()
{
	// TODO: Check if player is in sight with raycasting support
	// Monster::IsInSight();

	// For the vertical slice, we simply want the shark to stalk the player always
	switch (state)
	{
		// Idling when no target 
		case Idle:
			break;

		// Chase the player!
		case Chasing:
		{



			///* Add a vector of length moveSpeed in the direction towards our player
			//Adds a small offset downwards since our camera is higher up*/
			//vector3df currentPosition = getPosition();
			//vector3df targetPosition = GameManager::smgr->getActiveCamera()->getPosition() + vector3df(0, -10, 0);
			//vector3df moveDirection = (targetPosition - currentPosition).normalize();
			//vector3df newPosition = currentPosition + moveDirection * moveSpeed;			
			//setPosition(newPosition);

			//// TODO: Rotate 90 degrees extra so the shark faces the player
			//setRotation(moveDirection.getHorizontalAngle());
			//
			//// Add the same displacement to our mesh if we have one
			//if (mesh)
			//{		
			//	mesh->setPosition(newPosition);

			//	// TODO: Rotate 90 degrees extra so the shark faces the player
			//	mesh->setRotation(moveDirection.getHorizontalAngle());			
			//}
			//break;



		}

		// Attacking when in range
		case Attacking:
			break;

		// Seeking for a short while if lost target
		case Seeking:
			break;

		default:
			break;
	}
}
