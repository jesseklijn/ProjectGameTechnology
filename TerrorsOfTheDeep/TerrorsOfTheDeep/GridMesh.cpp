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
	startPos = *startPosition;
	GenerateField();
}

GridMesh::~GridMesh()
{
}

/* Generate the x size and y size of the grid depending on the worldRadius (defined in the GameManager class) and the cell size so it will be always fit within the level boundaries
*/
void GridMesh::GenerateField()
{
	// Assign the grid size for the vertices to be generated
	AssignSize(GameManager::worldRadiusX / CELL_SIZE, GameManager::worldRadiusZ / CELL_SIZE, 0);

	// Generates the vertices and triangles for the playing field mesh
	GenerateMesh();
}

void GridMesh::OnRegisterSceneNode()
{
	if (IsVisible)
		SceneManager->registerNodeForRendering(this);

	ISceneNode::OnRegisterSceneNode();
}

/* The grid generation makes use of a seed which makes it possible to get different vertices every time the playing mesh gets generated.
The output (not from the function) is an IAnimatedMeshSceneNode which is the mesh component for the GameObject class. This mesh will be added to the GameObject of the playing field.
*/
//NOTE: still needs to be refactored!
void GridMesh::GenerateMesh()
{
	xSizeGrid = xWidth * 2 + GRID_OFFSET;
	ySizeGrid = yHeight * 2 + GRID_OFFSET;

	// Components (buffers + meshes)
	IMeshManipulator* meshManipulator = GameManager::smgr->getMeshManipulator();
	SAnimatedMesh* meshGrid = new SAnimatedMesh();
	SMesh* sMesh = new SMesh();
	SMeshBuffer* buffer = new SMeshBuffer();

	sMesh->addMeshBuffer(buffer);
	buffer->drop();

	buffer->Vertices = DrawVertices(xSizeGrid, ySizeGrid);
	buffer->Indices = DrawTriangles(xSizeGrid, ySizeGrid);

	meshManipulator->recalculateNormals(sMesh);

	// Recalculate the bounding box of the mesh
	buffer->recalculateBoundingBox();

	// Convert the SMesh into a SAnimatedMesh 
	meshGrid->addMesh(sMesh);

	// Adds the SAnimatedMesh to the mesh of gameObject
	mesh = GameManager::smgr->addAnimatedMeshSceneNode(meshGrid, GameManager::smgr->getRootSceneNode());
	mesh->setPosition(startPos);

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
	return box;
}

u32 GridMesh::getMaterialCount() const
{
	return 1;
}

// Places objects on a random vertex of the mesh. It can use mesh and texture vectors to give the object random meshes and textures.
void GridMesh::RandomObjectPlacementOnVertex(int amount, vector3df position, vector3df scale,
                                           vector3df rotation,
                                           irr::s32 id, std::vector<irr::io::path> meshDirectories,std::vector<irr::io::path> textureDirectories, IMeshBuffer* playingFieldMesh)
{
	// Amount of tries before it skips the current object and go to the next object
	int currentTimeOutTries = 0;
	int TimeOutTries = 10;

	// Spawn random objects on grid;
	IMeshBuffer* planeBuffer = playingFieldMesh;

	// Get the vertices of the playingField 
	S3DVertex* mb_vertices = (S3DVertex*)planeBuffer->getVertices();

	// Amount of objects to be spawn on the grid
	int verticesGrid = planeBuffer->getVertexCount();

	// Tracks what vertices has an object spawned on them 
	vector<bool> spawnTracker(verticesGrid);

	for (int i = 0; i < amount; i++)
	{
		while (true) {
			currentTimeOutTries++;
			// Generate a random number for the selection of a vertice so an object can get spawned on it
			int randomizer = rand() % planeBuffer->getVertexCount();
			// Checks if the vertice is free (no object has been drawn on the vertex)
			if (!spawnTracker[randomizer])
			{
				int typeIndex = rand() % meshDirectories.size();
				GameObject* object = new GameObject(new vector3df(mb_vertices[randomizer].Pos.X + position.X, mb_vertices[randomizer].Pos.Y + position.Y,
					mb_vertices[randomizer].Pos.Z + position.Z),
					new vector3df(scale.X, scale.Y, scale.Z),
					new vector3df(rotation.X, rotation.Y, rotation.Z),
					0, GameManager::smgr, id,
					GameManager::smgr->getMesh(meshDirectories[typeIndex]),
					0);
				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < object->mesh->getMaterialCount(); mIndex++)
				{
					object->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}
				// Checks if the object can use a texture if there's any in the vector
				!textureDirectories[typeIndex].empty() ? GameManager::driver->getTexture(textureDirectories[typeIndex]) : 0;
				GameManager::gameObjects.push_back(object);
				spawnTracker[randomizer] = true;
				break;
				// Checks if the function needs to time out due to the failed attempts of the object placements
			} else if (currentTimeOutTries == TimeOutTries)
			{
				return;
			}
		}
	}
	return;
}

core::array<S3DVertex> GridMesh::DrawVertices(int xSizeGrid, int ySizeGrid)
{
	SMeshBuffer* bufferMesh = new SMeshBuffer();

	for (size_t y = 0; y <= ySizeGrid; y++)
	{
		for (size_t x = 0; x <= xSizeGrid; x++)
		{
			// Use a part of the grid to create a landmark
			if ((float)xSizeGrid / 1.7f < x && (float)ySizeGrid / 1.7f < y) {
				// Random landmark (high mountains)
				if ((float)xSizeGrid / 1.3f < x && (float)ySizeGrid / 1.3f < y)
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, rand() % highMountainConstantHeight + maxHighMountainHeight, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 1.5f < x && (float)ySizeGrid / 1.5f < y)
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, (rand() % highMountainConstantHeight + maxHighMountainHeight) / 2, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, rand() % highMountainConstantHeight / 4 + maxHighMountainHeight / 4, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			// Use a part of the grid to create a landmark
			else if ((float)xSizeGrid / 2 > x && (float)ySizeGrid / 2 > y && (float)xSizeGrid / 6 < x && (float)ySizeGrid / 6 < y)
			{
				// Random landmark (ruins)
				if ((float)xSizeGrid / 3 > x && (float)ySizeGrid / 3 > y && (float)xSizeGrid / 3.5 < x && (float)ySizeGrid / 3.5 < y)
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, ruinsConstantDepthLevel3, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else if ((float)xSizeGrid / 2.5f > x && (float)ySizeGrid / 2.5f > y && (float)xSizeGrid / 4.5f < x && (float)ySizeGrid / 4.5f < y)
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, ruinsConstantDepthLevel2, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
				else 
				{
					bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, ruinsConstantDepthLevel1, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
				}
			}
			else
			{
				// Default ground
				bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE, rand() % maxHeightNormalGround + constantHeightNormalGround, y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255), x, y));
			}
		}
	}
	return bufferMesh->Vertices;
}

core::array<u16> GridMesh::DrawTriangles(int xSizeGrid, int ySizeGrid)
{
	SMeshBuffer* bufferMesh = new SMeshBuffer();

	// Create triangles for the mesh
	for (int i = 0, ri = 0; i < ySizeGrid; i++, ri += xSizeGrid) 
	{
		for (int j = 0; j < xSizeGrid; j++) 
		{
			bufferMesh->Indices.push_back(j + ri + i);
			bufferMesh->Indices.push_back(xSizeGrid + ri + j + 1 + i);
			bufferMesh->Indices.push_back(j + ri + 1 + i);

			bufferMesh->Indices.push_back(j + ri + 1 + i);
			bufferMesh->Indices.push_back(xSizeGrid + ri + j + 1 + i);
			bufferMesh->Indices.push_back(xSizeGrid + ri + j + 2 + i);
		};
	}

	return bufferMesh->Indices;
}

SMaterial& GridMesh::getMaterial(u32 i)
{
	return Material;
}
