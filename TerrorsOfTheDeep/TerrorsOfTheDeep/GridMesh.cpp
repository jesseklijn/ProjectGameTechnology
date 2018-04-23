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
	GenerateField();
	GenerateMesh();
}

GridMesh::~GridMesh()
{
}

void GridMesh::GenerateField()
{
	AssignSize(50, 50, 0);
	Generate();
}

void GridMesh::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}


void GridMesh::GenerateMesh()
{
	xSizeGrid = xWidth;
	ySizeGrid = yHeight;
 
	SAnimatedMesh* meshGrid = new SAnimatedMesh();
	SMesh* sMesh = new SMesh();
	SMeshBuffer* buffer = new SMeshBuffer();

	sMesh->addMeshBuffer(buffer);
	buffer->drop();

	for (size_t y = 0; y <= ySizeGrid; y++)
	{
		for (size_t x = 0; x <= xSizeGrid; x++)
		{			
			if ((float)xSizeGrid/1.3f < x && (float)ySizeGrid/1.3f < y) {
				// Random landmark (high mountains)
				if ((float)xSizeGrid / 1.1f < x && (float)ySizeGrid / 1.1f < y) 
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % highMountainConstantHeight + maxHighMountainHeight, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 1.2f < x && (float)ySizeGrid / 1.2f < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % highMountainConstantHeight / 2 + maxHighMountainHeight / 2, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else {
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % highMountainConstantHeight / 4 + maxHighMountainHeight / 4, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			else if ((float)xSizeGrid / 2 > x && (float)ySizeGrid / 2 > y && (float)xSizeGrid / 6 < x && (float)ySizeGrid / 6 < y ) 
			{
				// Random landmark (ruins)
				if ((float)xSizeGrid / 3 > x && (float)ySizeGrid / 3 > y && (float)xSizeGrid / 3.5 < x && (float)ySizeGrid / 3.5 < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, -200, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 2.5f > x && (float)ySizeGrid / 2.5f > y && (float)xSizeGrid / 4.5f < x && (float)ySizeGrid / 4.5f < y)
				{
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, -100, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else {
					buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, -50, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			else 
			{
				// Default ground
				buffer->Vertices.push_back(irr::video::S3DVertex(x * cellSize, rand() % 50, y * cellSize, 0, 0, 0, irr::video::SColor(255, 255, 255, 255), x, y));
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

	// Creates the bounding box of the mesh
	Box.reset(buffer->Vertices[0].Pos);
	for (s32 i = 1; i < buffer->Vertices.size(); ++i)
	{
		Box.addInternalPoint(buffer->Vertices[i].Pos);
	}

	meshGrid->addMesh(sMesh);
	mesh = GameManager::smgr->addAnimatedMeshSceneNode(meshGrid, GameManager::smgr->getRootSceneNode());
	mesh->setPosition(getAbsolutePosition());	

	IVideoDriver* driver = SceneManager->getVideoDriver();
	//Material.setTexture(0, driver->getTexture("../media/SandTexture.jpg"));
	mesh->setAutomaticCulling(EAC_OFF);
	mesh->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	mesh->setMaterialFlag(EMF_LIGHTING, true);
	mesh->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	mesh->setMaterialTexture(0, driver->getTexture("../media/SandTexture.jpg"));
	//mesh->setPosition();

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

void GridMesh::render()
{

	IVideoDriver* driver = SceneManager->getVideoDriver();
	driver->setTransform(video::ETS_WORLD, AbsoluteTransformation);
}