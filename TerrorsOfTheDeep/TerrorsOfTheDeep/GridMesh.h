#pragma once
#include "GameObject.h"
#include "PlayingField.h"
class GridMesh :
	public GameObject, public PlayingField
{
public:
	GridMesh(IAnimatedMeshSceneNode* relatedNode, ITexture* relatedTexture,
		const vector3df* startPosition = new vector3df(0, 0, 0),
		const vector3df* startScale = new vector3df(1, 1, 1),
		const vector3df* startRotation = new vector3df(0, 0, 0));
	GridMesh(const vector3df* startPosition = new vector3df(0, 0, 0),
		const vector3df* startScale = new vector3df(1, 1, 1),
		const vector3df* startRotation = new vector3df(0, 0, 0));
	~GridMesh();

	void GenerateField();
	void GenerateMesh();

	vector<S3DVertex> vertices;
	SMaterial Material;
	
	void render();
};

