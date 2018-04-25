#pragma once
#include "GameObject.h"
#include "PlayingField.h"

class GridMesh :
	public GameObject, public PlayingField
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
	irr::core::aabbox3d<irr::f32> Box;

	// Rendering function
	virtual void Update();
	virtual void OnRegisterSceneNode();

	// Variables
	int xSizeGrid, ySizeGrid;
	int maxHighMountainHeight = 1000; //defines the max mountain differences (landmark)
	int highMountainConstantHeight = 400;
	int ruinsConstantDepthLevel1 = -50; //defines the 3 depth levels of the ruin part of grid (landmark)
	int ruinsConstantDepthLevel2 = -100;
	int ruinsConstantDepthLevel3 = -200;
	int maxHeightNormalGround = 50;  //defines the max ground differences (no landmark)
	int constantHeightNormalGround = 50;
	int cellSize = 250;
	irr::core::vector3df startpos;

	// Functions
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;	
};

