#include "Monster.h"
#include "main.cpp"
using namespace irr;

// TODO: transfer main components that are now in main, into GameManager.
// Then include GameManager here, see if double ref is fixed

Monster::Monster()
{
	device->setWindowCaption(L"Monster Test");
}

Monster::~Monster()
{
}

scene::ISceneNode* Monster::PerformRaycast(core::vector3df startPosition, core::vector3df endPosition)
{
	core::line3d<f32> ray;
	ray.start = startPosition;
	ray.end = endPosition;

	// This call is all you need to perform ray/triangle collision on every scene node
	// that has a triangle selector, including the Quake level mesh.  It finds the nearest
	// collision point/triangle, and returns the scene node containing that point.
	// Irrlicht provides other types of selection, including ray/triangle selector,
	// ray/box and ellipse/triangle selector, plus associated helpers.
	// See the methods of ISceneCollisionManager
	scene::ISceneNode* selectedSceneNode =
		collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			intersection,    // This will be the position of the collision
			hitTriangle,    // This will be the triangle hit in the collision
			IDFlag_IsPickable,  // This ensures that only nodes that we have set up to be pickable are considered
			0);          // Check the entire scene (this is actually the implicit default)

	return selectedSceneNode;
}
