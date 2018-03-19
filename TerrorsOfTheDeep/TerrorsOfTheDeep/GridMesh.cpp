#include "GridMesh.h"



GridMesh::GridMesh(IAnimatedMeshSceneNode* relatedNode, ITexture* relatedTexture,
	const vector3df* startPosition,
	const vector3df* startScale,
	const vector3df* startRotation) : GameObject(relatedNode, relatedTexture,
		startPosition,
		startScale,
		startRotation)
{

}


GridMesh::~GridMesh()
{
}
