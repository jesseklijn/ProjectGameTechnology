#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "Grid.h"

#pragma endregion

class GridMesh :
	public GameObject, public Grid
{
public:
	GridMesh(irr::core::vector3df* startPosition,
		irr::core::vector3df* startScale,
		irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~GridMesh();

	void GenerateField();
	void GenerateMesh();
	irr::core::aabbox3d<irr::f32> box;

	// Rendering function
	void OnRegisterSceneNode();

	// Variables
	static const int GRID_OFFSET = 10; //grid part that �sn't accessible by the player (field beyond level boundaries)
	static const int CELL_SIZE = 250; //Define the size of 1 cell (1 unit)
	irr::core::vector3df* startPos;
	int xSizeGrid, ySizeGrid;

	// Ground
	int heightMultiplier = 3;  //defines the max ground differences between the vertices

	// Functions 
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

	// This function allows game objects to be placed on the vertex of the given mesh buffer. 
	// This is needed to create a more random level where the objects are placed randomly in the level.
	// You must give either a GameObject or a list of GameObjects as one of the parameters in order to get this function to work.
	void RandomObjectPlacementOnVertex(irr::scene::IMeshBuffer* meshBuffer,
		                                   vector3df rootPosition, vector<GameObject*> gameObjectList = SceneManager::defaultGameObjectList, GameObject*
		                                   singleGameObject =
		                                   SceneManager::defaultGameObject
	                                   , irr::core::vector3df positionOffset = irr::core::vector3df(0, 0, 0), bool excludeOffset = false, bool resetOffsetPlacement = false);

	// Tracks what vertices has an object spawned on them (include previous calls of the "RandomObjectPlacementOnVertex" function as well)
	vector<bool> previousPlacementTracker;

private:
	irr::core::array<irr::video::S3DVertex> DrawVertices(int xSizeGrid, int ySizeGrid, bool useHeightMap = false);
	irr::core::array<unsigned short> DrawTriangles(int xSizeGrid, int ySizeGrid);
}; 
