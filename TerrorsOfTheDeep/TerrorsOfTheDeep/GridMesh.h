#pragma once
#include "SceneManager.h"
#include "GameObject.h"
#include "Grid.h"

#pragma endregion

/// GridMesh inherits from GameObject to make use of all the functions that are used in GameObject.
/// This makes it easy to modify the mesh like changing the scale, setting a parent, adding additional meshes to the existing mesh.
/// It also allows the class to make use of the draw functions like the Update function in the GameObject class which is responsible for drawing the mesh,
/// collision detection and collision handling.
class GridMesh :
	public GameObject
{
public:
	GridMesh(irr::core::vector3df* startPosition,
		irr::core::vector3df* startScale,
		irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~GridMesh();

	void GenerateMesh();
	irr::core::aabbox3d<irr::f32> box;

	void OnRegisterSceneNode();

	static const int GRID_OFFSET = 20; //grid part that isn't accessible by the player (field beyond level boundaries)
	static const int CELL_SIZE = 250; //Define the size of 1 cell (1 unit)

	/// Standard irrlicht Functions 
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;

private:
	/// Generates the grid size based on the given parameters ,x , y ,z
	bool GenerateGridSize(int x, int y, int z);

	int xWidth, yHeight, zLength;

	/// Ground modifier of the vertices which determines the height differences between the color values
	/// IT's only used inside the vertices draw function so it needs to stay private
	int heightMultiplier = 20; 

	/// Vertices of the grid mesh are drawn inside this class only so it needs to stay as a private function.
	/// If you want to use a height map that is generated through a noise generator, 
	/// you need to give it as a parameter otherwise it will be set to the default value which is false.
	irr::core::array<irr::video::S3DVertex> DrawVertices(int xSizeGrid, int ySizeGrid, bool useHeightMap = false);

	/// Triangles of the grid mesh are drawn inside this class only so it needs to stay as a private function
	irr::core::array<u16> DrawTriangles(int xSizeGrid, int ySizeGrid);

	/// The start position (root of the GameObject) is only being used inside this class since the generated mesh won't be placed on the same position as the GameObject.
	/// This variable basically allows the generated mesh to be set the same position as the root position.
	irr::core::vector3df* startPos;

	/// the x and y size of the grid are being used inside this class only so it stays as a private variable.
	int xSizeGrid, ySizeGrid;

	/// This variable is only used inside this class so it needs to stay private.
	/// Tracks what vertices has an object spawned on them (include previous calls of the "RandomObjectPlacementOnVertex" function as well)
	vector<bool> previousPlacementTracker;

	/// This function allows game objects to be placed on the vertex of the given mesh buffer. 
	/// This is needed to create a more random level where the objects are placed randomly in the level.
	/// You must give either a GameObject or a list of GameObjects as one of the parameters in order to get this function to work.
	/// This function stays private since it's only used inside this class and no where else.
	void RandomObjectPlacementOnVertex(irr::scene::IMeshBuffer* meshBuffer,
	vector3df rootPosition, vector<GameObject*> gameObjectList = SceneManager::defaultGameObjectList, GameObject*
	singleGameObject =
	SceneManager::defaultGameObject
	, irr::core::vector3df positionOffset = irr::core::vector3df(0, 0, 0), bool excludeOffset = false, bool resetOffsetPlacement = false);
}; 
