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
	irr::core::vector3df* startPosition,
	irr::core::vector3df* startScale,
	irr::core::vector3df* startRotation,
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
	startPos = startPosition;
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
	AssignSize(GameManager::WORLD_RADIUS_X / CELL_SIZE, GameManager::WORLD_RADIUS_Z / CELL_SIZE, 0);

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

	// Convert the SMesh into a SAnimatedMesh (GameObjects uses SAnimatedMesh only and not 1 static mesh)
	meshGrid->addMesh(sMesh);

	// Adds the SAnimatedMesh to the mesh of gameObject 
	mesh = GameManager::smgr->addAnimatedMeshSceneNode(meshGrid, GameManager::smgr->getRootSceneNode());
	mesh->setPosition(*startPos);

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
// Either use a list with GameObjects or a single GameObject. One of the 2 must be given as a parameter.
void GridMesh::RandomObjectPlacementOnVertex(IMeshBuffer* meshBuffer,vector3df rootPosition, vector<GameObject*> gameObjectList, GameObject* singleGameObject, vector3df
                                             positionOffset, bool excludeOffset, bool resetOffsetPlacement)
{
	bool useSingleGameObject = false;
	bool useMultipleGameObjects = false;

	// Amount of game objects that needs to be placed on the vertex
	int amount;

	// Error prevention if both parameters are default and checks if it needs to use a list or a single game object
	if (!gameObjectList.empty())
	{
		amount = gameObjectList.size();
		useMultipleGameObjects = true;
	}

	else if (singleGameObject != nullptr) {
		amount = 1;
		useSingleGameObject = true;
	}
	else
	{
		puts("Error - Function GridMesh::RandomObjectPlacementOnVertex contains both default parameters (gameObjectList and singleGameObject)");
		return;
	}

	// Amount of tries before it skips the current object and go to the next object
	int currentTimeOutTries = 0;
	const int timeOutTries = 10;

	// Spawn random objects on grid;
	IMeshBuffer* buffer = meshBuffer;

	// Get the vertices of the playingField 
	S3DVertex* mb_vertices = (S3DVertex*)buffer->getVertices();

	// Amount of objects to be spawn on the grid
	int verticesGrid = buffer->getVertexCount();

	// Tracks what vertices has an object spawned on them 
	vector<bool> placementTracker(verticesGrid);

	// Checks if this function has been called more than one time so it will assign the same data of the previous call to prevent multiple object placements on the same vertex
	if (!previousPlacementTracker.empty())
	{
		placementTracker = previousPlacementTracker;
	}

	// Exclude the vertices that are in the offset
	if (excludeOffset)
	{
		for (int i = 0; i < verticesGrid; i++)
		{
			S3DVertex currentVertex = mb_vertices[i];
			if (currentVertex.Pos.X + startPos->X > GameManager::WORLD_RADIUS_X || currentVertex.Pos.X + startPos->X < -GameManager::WORLD_RADIUS_X ||
				currentVertex.Pos.Z + startPos->Z > GameManager::WORLD_RADIUS_Z || currentVertex.Pos.Z + startPos->Z < -GameManager::WORLD_RADIUS_Z)
			{
				// Exclude vertex from object placement
				placementTracker[i] = true;
			}
		}
	} 

	for (int i = 0; i < amount; i++)
	{
		while (true) 
		{
			// Generate a random number for the selection of a vertex so an object can get spawned on it
			int randomizer = rand() % buffer->getVertexCount();

			if (!placementTracker[randomizer] && useMultipleGameObjects)
			{
				// Sets the position of the game object to the position of the vertices (+ offset)
				gameObjectList[i]->setPosition(vector3df(mb_vertices[randomizer].Pos.X + rootPosition.X + positionOffset.X, mb_vertices[randomizer].Pos.Y + rootPosition.Y + positionOffset.Y,
					mb_vertices[randomizer].Pos.Z + rootPosition.Z + positionOffset.Z));

				// Sets the position of the game object mesh to the same position
				gameObjectList[i]->mesh->setPosition(vector3df(mb_vertices[randomizer].Pos.X + rootPosition.X + positionOffset.X, mb_vertices[randomizer].Pos.Y + rootPosition.Y + positionOffset.Y,
					mb_vertices[randomizer].Pos.Z + rootPosition.Z + positionOffset.Z));

				// Set the vertex bool on true so no other object can be placed on the same vertex
				placementTracker[randomizer] = true;

				// Saves the placements of the vertices so it can be used for the next call
				previousPlacementTracker = placementTracker;
				break;
			} 
			else if (!placementTracker[randomizer] && useSingleGameObject)
			{
				// Sets the position of the game object to the position of the vertices (+ offset)
				singleGameObject->setPosition(vector3df(mb_vertices[randomizer].Pos.X + rootPosition.X + positionOffset.X,
				                                             mb_vertices[randomizer].Pos.Y + rootPosition.Y + positionOffset.Y +
				                                             100,
					mb_vertices[randomizer].Pos.Z + rootPosition.Z + positionOffset.Z));

				// Sets the position of the game object mesh to the same position
				singleGameObject->mesh->setPosition(vector3df(mb_vertices[randomizer].Pos.X + rootPosition.X + positionOffset.X,
					mb_vertices[randomizer].Pos.Y + rootPosition.Y + positionOffset.Y +
					100,
					mb_vertices[randomizer].Pos.Z + rootPosition.Z + positionOffset.Z));

				// Set the vertex bool on true so no other object can be placed on the same vertex
				placementTracker[randomizer] = true;

				break;
			}

			// Skips the current gameObject due to multiple failed tries
			if (currentTimeOutTries == timeOutTries)
			{
				break;
			}
			currentTimeOutTries++;
		}

		// Resets the offset vertices for the next use 
		if (resetOffsetPlacement)
		{
			for (int i = 0; i < verticesGrid; i++)
			{
				S3DVertex currentVertex = mb_vertices[i];
				if (currentVertex.Pos.X + startPos->X > GameManager::WORLD_RADIUS_X || currentVertex.Pos.X + startPos->X < -GameManager::WORLD_RADIUS_X ||
					currentVertex.Pos.Z + startPos->Z > GameManager::WORLD_RADIUS_Z || currentVertex.Pos.Z + startPos->Z < -GameManager::WORLD_RADIUS_Z)
				{
					// reset vertex for the next object placement on the offset
					placementTracker[i] = false;

				}
			}
		}

		// Saves the placements of the vertices so it can be used for the next call
		previousPlacementTracker = placementTracker;
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
