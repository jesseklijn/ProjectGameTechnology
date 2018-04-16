#include "GridMesh.h"

GridMesh::GridMesh(
	const irr::core::vector3df* startPosition,
	const irr::core::vector3df* startScale,
	const irr::core::vector3df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
	irr::scene::IAnimatedMesh* relatedMesh, irr::video::ITexture* relatedTexture) : GameObject(
		startPosition, 
		startScale, 
		startRotation,
		parent,
		mgr,
		id,
		relatedMesh,
		relatedTexture)
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
				vertices.push_back(irr::video::S3DVertex(x,y,z, 1, 1, 0, irr::video::SColor(255, 0, 255, 255), 0, 1));
			
			}
		}
	}
	return;
}

void GridMesh::render()
{
	irr::u16 indices[] = { 0,2,3, 2,1,3 };

	irr::video::IVideoDriver* driver = SceneManager->getVideoDriver();

	driver->setMaterial(Material);
	driver->setTransform(irr::video::ETS_WORLD, AbsoluteTransformation);
	driver->drawVertexPrimitiveList(&vertices[0], 8, &indices[0], 8, irr::video::EVT_STANDARD, irr::scene::EPT_TRIANGLES, irr::video::EIT_16BIT);
}