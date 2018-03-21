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

	vector<irr::video::S3DVertex> vertices;
	irr::video::SMaterial Material;
	
	void render();
};

