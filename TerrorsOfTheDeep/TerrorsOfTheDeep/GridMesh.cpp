#include "GridMesh.h"
#include "GameManager.h"
#include "NoiseGenerator.h"
#include "SceneManager.h"

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

/// The grid generation makes use a heightmap which makes it possible to get different vertices every time the playing mesh gets generated. <br/>
/// <b>Components</b><br/>
/// SMesh - The grid mesh will be put in a SMesh which is a static mesh <br/>
/// SMeshBuffer - Makes it possible to modify the SMesh  <br/>
/// IMeshManipulator - Allows the mesh to be modified by more built-in irrlicht functions (such as recalculating normals) <br/>
/// SAnimatedMesh - Used to make a SMesh into a mesh without animation (GameObject uses SAnimatedMeshes to draw its mesh thus a SMesh cannot be assigned to the GameObject) <br/>
/// <b>Usage</b><br/>
/// This function is currently used to create a mesh out of a grid which uses a heightmap to create different and natural feeling heights to the vertices.
/// <b>How does it work?</b><br/>
/// After assigning the size of the grid, it also gets multiplied by 2 (to get the grid in the center for positioning purposes) and the offset gets added. <br/>
/// All the components of the grid has to be instantiated and added to their corresponding components. <br/>
/// After that, the vertices and triangles are drawn by giving the grid size as parameters. These functions will return a list of vertices and triangles to the caller. </br>
/// The vertices and triangles are added to the SMesh </br>
/// The bounding box and normals are recalculated to make the lighting and collisions work as they should.  </br>
/// This SMesh will be added to the SAnimatedMesh so it will be compatible with GameObject.  </br>
/// Then the mesh's position will be set to their parent position and the texture will be added to the mesh. </br>

void GridMesh::GenerateMesh()
{
	// Sets the total grid size to be generated (including the offset)
	xSizeGrid = xWidth * 2 + GRID_OFFSET;
	ySizeGrid = yHeight * 2 + GRID_OFFSET;

	// Components (buffers + meshes)
	IMeshManipulator* meshManipulator = GameManager::smgr->getMeshManipulator();
	SAnimatedMesh* meshGrid = new SAnimatedMesh();
	SMesh* sMesh = new SMesh();
	SMeshBuffer* buffer = new SMeshBuffer();

	// Draw the vertices and triangles of the grid
	buffer->Vertices = DrawVertices(xSizeGrid, ySizeGrid, true);
	buffer->Indices = DrawTriangles(xSizeGrid, ySizeGrid);

	buffer->recalculateBoundingBox();
	sMesh->addMeshBuffer(buffer);
	meshManipulator->recalculateNormals(sMesh);
	sMesh->recalculateBoundingBox();

	// Convert the SMesh into a SAnimatedMesh (GameObjects uses SAnimatedMesh only and not 1 static mesh which is this sMesh)
	// So by putting the sMesh into a SAnimatedMesh; it creates a mesh with no animation
	meshGrid->addMesh(sMesh);

	// Adds the SAnimatedMesh to the mesh of gameObject 
	mesh = GameManager::smgr->addAnimatedMeshSceneNode(meshGrid, 0);
	mesh->setPosition(*startPos);

	// Set the material flags
	IVideoDriver* driver = SceneManager->getVideoDriver();
	mesh->setAutomaticCulling(EAC_OFF);
	mesh->setMaterialFlag(EMF_BACK_FACE_CULLING, false);
	mesh->setMaterialFlag(EMF_LIGHTING, true);
	mesh->setMaterialFlag(EMF_NORMALIZE_NORMALS, true);
	mesh->setMaterialFlag(EMF_FOG_ENABLE,true);
	mesh->setMaterialTexture(0, driver->getTexture("../media/WorldDetail/SandTexture.jpg"));
	return;
}

const aabbox3d<f32>&  GridMesh::getBoundingBox() const
{
	return box;
}

/// Places objects on a random vertex of the mesh. It can use mesh and texture vectors to give the object random meshes and textures.  <br/>
/// Either use a list with GameObjects or a single GameObject. One of the 2 must be given as a parameter. <br/>
/// <b>Parameters</b><br/>
/// meshBuffer - Buffer of the mesh you wish to place the GameObjects on <br/>
/// rootPosition - Parent position of the mesh<br/>
/// gameObjectList - A list of GameObjects (this is used when multiple GameObjects needs to be placed on the mesh)<br/>
/// singleGameObject - A single GameObject (this is used when only one GameObject needs to be placed on the mesh)<br/>
/// positionOffset - Offset of the placement <br/>
/// excludeOffset - Should the GameObject(s) spawn within the offset (level boundaries)? <br/>
/// resetOffsetPlacement - Don't track the placements outside the offset (this is used for spawning key items / chests so the GameObjects in the next call can spawn outside the level boundaries) <br/>
/// <b>Usage</b><br/>
/// This function is currently used to place GameObject(s) on the given mesh. In this case it's a grid mesh. <br/>
/// <b>How does it work?</b><br/>
/// The function will check if a list or a single GameObject is given. If it's a list then the amount variable will be the same as the size of the list. If it's a single then the amount is 1. <br/>
/// This is used for determining the amount of loops in the for-loop later. A list of bool will be made for keeping track of what vertices already has a GameObject spawned on them. <br/>
/// The amount of vertices is the same as the size of this list; the list represents the vertices of the mesh. <br/>
/// If the excludeOffset parameter is true then the vertices outside the level boundaries will be excluded (will be set to true in the list).<br/>
/// After this, a check will be made if there's a previous call (from the same grid) made so there can't be two GameObjects spawning on a single vertex. <br/>
/// A while(true) will be used to place all the GameObjects on the vertices by using a random seed generator to choose the vertex. <br/>
/// for-loop is used to loop through all the GameObjects. <br/>
/// There's a break on every successful GameObject placement; this breaks the while(true). <br/>
/// It also sets the bool in the list to true so for the next call. <br/> 
/// there won't be another GameObject spawning on that same vertex. <br/>
/// If the placement has failed X amount of times (determined by "timeOutTries" variable), it will skip the GameObject. <br/> 
/// After every GameObject has placed on the vertex, the bool list will be saved in a global variable so it can be used for the next call. <br/>
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

			// This will be used if a list of objects has been given as a parameter
			// It also checks if the selected vertex hasn't an object spawn on it
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
			// This will be used if a single game object has been given as a parameter
			// It also checks if the selected vertex hasn't an object spawn on it
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
/// Draws the vertices of the grid <br/>
/// <b>Parameters</b><br/>
/// xSizeGrid and ySizeGrid - Determines the size of the grid that it needs to draw the vertices <br/>
/// useHeightmap - Should the vertices make use of a heightmap? Or you wan't a plain grid with vertices where the y coordinates are the same<br/>
/// <b>Usage</b><br/>
/// This function is currently used for drawing the vertices of the gridMesh
/// <b>How does it work?</b><br/>
/// A heightMap will be generated if the bool is true. A NoiseGenerator has to be instantiated in order to generate a height map. <br/>
/// After the height map has been generated, the height map needs to fit the mesh. By dividing the height map with the grid size, <br/>
/// you get even parts of the height map so it will use the whole height map instead of a small part. This value will be saved as float variables "precisionX" and "precisionY" <br/>
/// After this, the vertices are drawn by using the x and y values from the for-loop and multiplying by the CELL_SIZE constant variable to make a bigger grid. <br/>
/// The reason why the y variable is on the z-axis of the vertex is because it is a 2D grid. So you have to rotate it by 90 degrees every time you generate vertex. <br/> 
/// the y-axis defines the height of the vertices and a color picker function is used for this. A multiplier is used to make the color differences bigger 
/// (the height will differ greater between each other as well) <br/>
/// If no height map is used then the vertices are generated without any height differences. <br/>
/// <b>Output</b><br/>
/// A list of vertex.
core::array<S3DVertex> GridMesh::DrawVertices(int xSizeGrid, int ySizeGrid, bool useHeightMap)
{
	// Buffer to put the vertices in it. The vertices will be returned as an array and not the whole buffer.
	SMeshBuffer* bufferMesh = new SMeshBuffer();

	if (useHeightMap) 
	{
		// Generate noiseMap 
		NoiseGenerator* noiseGenerator = new NoiseGenerator();
		noiseGenerator->GenerateHeightMap("../media/heightMap.bmp", 256,256);
		ITexture* texture = GameManager::driver->getTexture("../media/heightMap.bmp");

		// Divide the noise map by the vertices of the mesh to get good color differences
		float precisionX = noiseGenerator->xSizeImage / xSizeGrid;
		float precisionY = noiseGenerator->ySizeImage / ySizeGrid;

		for (size_t y = 0; y <= ySizeGrid; y++)
		{
			for (size_t x = 0; x <= xSizeGrid; x++)
			{
				// Draw the vertices and use the noise map's pixel for the height of the vertex to create natural hill heights
				bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE,
					noiseGenerator->GetPixelColor(texture,
					                              x * precisionX, y * precisionY).getRed() * heightMultiplier,
					y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255),
					x, y));
			}
		}
	} 
	else
	{
		for (size_t y = 0; y <= ySizeGrid; y++)
		{
			for (size_t x = 0; x <= xSizeGrid; x++)
			{
				// Draw the vertices of the grid. This will make a plain playing field with no height differences
				bufferMesh->Vertices.push_back(irr::video::S3DVertex(x * CELL_SIZE,
					0,
					y * CELL_SIZE, 1, 1, 1, irr::video::SColor(255, 255, 255, 255),
					x, y));
			}
		}
	}
	return bufferMesh->Vertices;
}

/// Draws the triangles of the grid <br/>
/// <b>Parameters</b><br/>
/// xSizeGrid and ySizeGrid - Determines the size of the grid that it needs to draw the triangles <br/>
/// <b>Usage</b><br/>
/// This function is currently used for drawing the triangles of the gridMesh
/// <b>How does it work?</b><br/>
/// A simple triangle technique is used for this where:  <br/>
/// ri -Keeps track of the row above the selected vertex. <br/>
/// y - Keeps track of the y vertices of the grid<br/>
/// x - Keeps track of the x vertices of the grid<br/>
/// +1 is used to get the next vertex of the selected vertex
/// <b>Output</b><br/>
/// A list of triangles.
core::array<u16> GridMesh::DrawTriangles(int xSizeGrid, int ySizeGrid)
{
	SMeshBuffer* bufferMesh = new SMeshBuffer();

	// Create triangles for the mesh
	for (int y = 0, ri = 0; y < ySizeGrid; y++, ri += xSizeGrid) 
	{
		for (int x = 0; x < xSizeGrid; x++) 
		{
			// Draws the first triangle
			bufferMesh->Indices.push_back(x + ri + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 1 + y);
			bufferMesh->Indices.push_back(x + ri + 1 + y);

			// Draws the second triangle (to make a quad)
			bufferMesh->Indices.push_back(x + ri + 1 + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 1 + y);
			bufferMesh->Indices.push_back(xSizeGrid + ri + x + 2 + y);
		};
	}

	return bufferMesh->Indices;
}

SMaterial& GridMesh::getMaterial(u32 i)
{
	return material;
}
