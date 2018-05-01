#include "GridMesh.h"
#include "GameManager.h"

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

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
	startpos = *startPosition;
	GenerateField();
	GenerateMesh();
}

GridMesh::~GridMesh()
{
}
/* Generate the x size and y size of the grid depending on the worldRadius (defined in the GameManager class) so it will be always fit the level boundaries
*/
void GridMesh::GenerateField()
{
	// Assign the grid size for the vertices to be generated
	AssignSize(GameManager::worldRadiusX / cellSize, GameManager::worldRadiusZ / cellSize, 0);
}

void GridMesh::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

/* The grid generation makes use of a seed which makes it possible to get different vertices every time the playing mesh gets generated.
The output is an IAnimatedMesh which is the mesh component for the GameObject class.
*/
//NOTE: still needs to be refactored!
void GridMesh::GenerateMesh()
{
	xSizeGrid = xWidth * 2 + gridOffset;
	ySizeGrid = yHeight * 2 + gridOffset;

	// Components (buffers + meshes)
	IMeshManipulator* meshManipulator = GameManager::smgr->getMeshManipulator();
	SAnimatedMesh* meshGrid = new SAnimatedMesh();
	SMesh* sMesh = new SMesh();
	SMeshBuffer* buffer = new SMeshBuffer();

	sMesh->addMeshBuffer(buffer);
	buffer->drop();

	for (size_t y = 0; y <= ySizeGrid; y++)
	{
		for (size_t x = 0; x <= xSizeGrid; x++)
		{	
			// Use a part of the grid to create a landmark
			if ((float)xSizeGrid/1.7f < x && (float)ySizeGrid/1.7f < y) {
				// Random landmark (high mountains)
				if ((float)xSizeGrid / 1.3f < x && (float)ySizeGrid / 1.3f < y) 
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % highMountainConstantHeight + maxHighMountainHeight, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 1.5f < x && (float)ySizeGrid / 1.5f < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, (rand() % highMountainConstantHeight + maxHighMountainHeight) / 2, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else 
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % highMountainConstantHeight / 4 + maxHighMountainHeight / 4, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			// Use a part of the grid to create a landmark
			else if ((float)xSizeGrid / 2 > x && (float)ySizeGrid / 2 > y && (float)xSizeGrid / 6 < x && (float)ySizeGrid / 6 < y ) 
			{
				// Random landmark (ruins)
				if ((float)xSizeGrid / 3 > x && (float)ySizeGrid / 3 > y && (float)xSizeGrid / 3.5 < x && (float)ySizeGrid / 3.5 < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, ruinsConstantDepthLevel3, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 2.5f > x && (float)ySizeGrid / 2.5f > y && (float)xSizeGrid / 4.5f < x && (float)ySizeGrid / 4.5f < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, ruinsConstantDepthLevel2, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else {
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, ruinsConstantDepthLevel1, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			else 
			{
				// Default ground
				buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % maxHeightNormalGround + constantHeightNormalGround, y * cellSize, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
			}
		}
	}
	// Create triangles for the mesh
	for (int i = 0, ri = 0; i < ySizeGrid; i++, ri += xSizeGrid) {

		for (int j = 0; j < xSizeGrid; j++) {
			buffer->Indices.push_back(j + ri + i);
			buffer->Indices.push_back(xSizeGrid + ri + j + 1 + i);
			buffer->Indices.push_back(j + ri + 1 + i);

			buffer->Indices.push_back(j + ri + 1 + i);
			buffer->Indices.push_back(xSizeGrid + ri + j + 1 + i);
			buffer->Indices.push_back(xSizeGrid + ri + j + 2 + i);
		};
	}
	meshManipulator->recalculateNormals(sMesh);

	// Recalculate the bounding box of the mesh
	buffer->recalculateBoundingBox();

	// Convert the SMesh into a SAnimatedMesh 
	meshGrid->addMesh(sMesh);

	// Adds the SAnimatedMesh to the mesh of gameObject
	mesh = GameManager::smgr->addAnimatedMeshSceneNode(meshGrid, GameManager::smgr->getRootSceneNode());
	mesh->setPosition(startpos);

	// Set the material flags
	IVideoDriver* driver = SceneManager->getVideoDriver();
	mesh->setAutomaticCulling(EAC_OFF);
	mesh->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	mesh->setMaterialFlag(EMF_LIGHTING, true);
	mesh->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	mesh->setMaterialFlag(EMF_FOG_ENABLE,true);
	mesh->setMaterialTexture(0, driver->getTexture("../media/SandTexture.jpg"));
	return;
}

const aabbox3d<f32>&  GridMesh::getBoundingBox() const
{
	return Box;
}

u32 GridMesh::getMaterialCount() const
{
	return 1;
}

SMaterial& GridMesh::getMaterial(u32 i)
{
	return Material;
}

void GridMesh::Update()
{
	// Inherit base class Update
	GameObject::Update();

	//UpdatePos();
}