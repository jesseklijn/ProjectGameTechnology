#pragma once
#include "GameObject.h"
#include "Grid.h"

class GridMesh :
	public GameObject, public Grid
{
public:
	GridMesh(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~GridMesh();

	void GenerateField();
	void GenerateMesh();
	irr::core::aabbox3d<irr::f32> box;

	// Rendering function
	void OnRegisterSceneNode();

	// Variables
	static const int GRID_OFFSET = 30; //grid part that ísn't accessible by the player (field beyond level boundaries)
	static const int CELL_SIZE = 500; //Define the size of 1 cell (1 unit)
	int xSizeGrid, ySizeGrid;
	int maxHighMountainHeight = 1000; //defines the max mountain differences (landmark)
	int highMountainConstantHeight = 400;
	int ruinsConstantDepthLevel1 = -100; //defines the 3 depth levels of the ruin part of grid (landmark)
	int ruinsConstantDepthLevel2 = -300;
	int ruinsConstantDepthLevel3 = -500;
	int maxHeightNormalGround = 200;  //defines the max ground differences (no landmark)
	int constantHeightNormalGround = 50;
	irr::core::vector3df startPos;

	// Functions 
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;
	static void RandomObjectPlacementOnVertex(int amount, 
		irr::core::vector3df position, 
		irr::core::vector3df scale,
		irr::core::vector3df rotation, irr::s32 id,
		std::vector<irr::io::path> meshDirectories,
		std::vector<irr::io::path> textureDirectories, 
		irr::scene::IMeshBuffer* playingFieldMesh,
		bool detectCollision = true,
		bool castsShadows = false,
		float scaleVariationX = 0.0f,
		float scaleVariationY = 0.0f,
		float scaleVariationZ = 0.0f,
		float rotationVariationX = 0.0f,
		float rotationVariationY = 0.0f,
		float rotationVariationZ = 0.0f);

private:
	irr::core::array<irr::video::S3DVertex> DrawVertices(int xSizeGrid, int ySizeGrid);
	irr::core::array<unsigned short> DrawTriangles(int xSizeGrid, int ySizeGrid);
};

