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
	virtual void render();
	virtual void OnRegisterSceneNode();

	// Variables
	int xSizeGrid, ySizeGrid;
	int maxHighMountainHeight = 600;
	int highMountainConstantHeight = 400;
	int maxRuinsDepth = 200;
	int ruinsConstantDepth = 200;
	int cellSize = 25;

	// Functions
	virtual irr::video::SMaterial& getMaterial(irr::u32 i);
	virtual const irr::core::aabbox3d<irr::f32>& getBoundingBox() const;
	virtual irr::u32 getMaterialCount() const;	
};

