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

GridMesh::GridMesh(const vector3df * startPosition, const vector3df * startScale, const vector3df * startRotation) : GameObject(
	startPosition,
	startScale,
	startRotation)
{
	GenerateField();
	GenerateMesh();
}


GridMesh::~GridMesh()
{
}

void GridMesh::GenerateField()
{
	AssignSize(10, 10, 10);
	Generate();
}

void GridMesh::GenerateMesh()
{
	for (size_t x = 0; x < grid.size(); x++)
	{
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			for (size_t z = 0; z < grid[x][y].size(); z++)
			{
				if(y == 0)
				vertices.push_back(S3DVertex(x,y,z, 1, 1, 0, SColor(255, 0, 255, 255), 0, 1));
			
			}
		}
	}
	return;
}
void GridMesh::render()
{
	u16 indices[] = { 0,2,3, 2,1,3 };

	video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&Vertices[0], 8, &indices[0], 8, video::EVT_STANDARD, scene::EPT_TRIANGLES, video::EIT_16BIT);
}