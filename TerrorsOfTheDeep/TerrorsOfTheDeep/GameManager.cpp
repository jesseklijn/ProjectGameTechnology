#pragma once
#include "GameManager.h"
#include <utility>

// DLL files
#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

#pragma region Core Irrlicht Components
// Initialize Irrlicht device
irr::IrrlichtDevice* GameManager::device =
	createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
		false, false, false, 0);

// Initialize Irrlicht components
irr::video::IVideoDriver* GameManager::driver = GameManager::device->getVideoDriver();
irr::scene::ISceneManager* GameManager::smgr = GameManager::device->getSceneManager();
irr::gui::IGUIEnvironment* GameManager::guienv = GameManager::device->getGUIEnvironment();
irr::scene::ISceneCollisionManager* GameManager::collMan = GameManager::smgr->getSceneCollisionManager();
irr::gui::IGUIFont* GameManager::font = GameManager::guienv->getBuiltInFont();

/*
To look at the mesh, we place a camera into 3d space at the position
(0, 30, -40). The camera looks from there to (0,5,0), which is
approximately the place where our md2 model is.
*/
irr::scene::ICameraSceneNode* GameManager::camera = GameManager::smgr->addCameraSceneNode(0, vector3df(0, 30, -40), vector3df(0, 5, 0));
#pragma endregion

#pragma region Raycasting

// Initialize a base selector, used for assigning selection to scene nodes 
// It's dropped after every selector assignment, but it's easily re-usable 
ITriangleSelector* selector;

// Initialize a re-usable ray 
line3d<f32>* ray;

// Tracks the current intersection point with the level or a mesh 
vector3df* intersection;

// Used to show with triangle has been hit 
triangle3df* hitTriangle;

ISceneNode* highlightedSceneNode = 0;
#pragma endregion

GameManager::GameManager()
{
}

GameManager::~GameManager()
{

}

void GameManager::Start()
{
}

void GameManager::Awake()
{
	
}

void GameManager::Update()
{
	
}

void GameManager::Draw()
{
	
}


void GameManager::GameStateTransition(GameState StateToLoad)
{

}

void GameManager::UnloadGameState(GameState StateToCleanUp)
{

}

// Cast a Raycast line between the given start and end positions and return the ISceneNode that was hit.
scene::ISceneNode* GameManager::PerformRaycast(core::vector3df startPosition, core::vector3df endPosition)
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
		GameManager::collMan->getSceneNodeAndCollisionPointFromRay(
			ray,
			*intersection,    // This will be the position of the collision
			*hitTriangle,    // This will be the triangle hit in the collision
			GameManager::IDFlag_IsPickable,  // This ensures that only nodes that we have set up to be pickable are considered
			0);          // Check the entire scene (this is actually the implicit default)

	return selectedSceneNode;
}
