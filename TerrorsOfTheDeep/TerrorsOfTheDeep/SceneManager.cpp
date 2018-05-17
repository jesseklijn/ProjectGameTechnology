
#pragma once
#include "SceneManager.h"
#include "GridMesh.h"

using namespace std;
static const int NO_PARENT = 0;
static const float KEYLIGHT_RADIUS = 50.f;
static const float CHESTLIGHT_RADIUS = 90.f;
static const float FLASHLIGHT_RANGE = 1500.f;

SceneManager::SceneType SceneManager::scene = SceneManager::NONE;
SceneManager::SceneType SceneManager::scenePrevious = SceneManager::scene;
bool SceneManager::sceneIsPaused = false;

// Light data
//irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(0.3f, 0.3f, 0.4f, 1.0f);
irr::video::SColorf SceneManager::ambientColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::flashlightColor = irr::video::SColorf(1.0f, 1.0f, 1.0f, 1.0f);
irr::video::SColorf SceneManager::sharkEyesColor = irr::video::SColorf(0.5f, 0.0f, 0.0f, 1.0f);
vector3df SceneManager::chestLightOffset = vector3df(40, 30, 0);
vector3df SceneManager::keyLightOffset = vector3df(0, 20, 0);

Camera* SceneManager::camera;
HUD* SceneManager::hud = new HUD();
bool SceneManager::disableHud = false;

// Define default parameters
 vector3df* SceneManager::vectorZero = new vector3df(0,0,0);
 int SceneManager::noID = -1111;
 GameObject* SceneManager::defaultGameObject;
 vector<GameObject*> SceneManager::defaultGameObjectList;
 float SceneManager::distanceKeyChest = 1500;

// Constructor
SceneManager::SceneManager()
{
	scene = NONE;
	scenePrevious = scene;
	sceneIsPaused = false;


}

// Destructor
SceneManager::~SceneManager()
{

}

/* Updates every frame.
Happens before any drawing.*/
void SceneManager::Update()
{
	if (camera)
		camera->updatePos();

	if (GameManager::eventManager.IsKeyPressed(KEY_ESCAPE) && scene == LEVEL) 
		SceneManager::PauseScene(!sceneIsPaused);
}

/* Updates every frame.
Happens after scene updates.*/
void SceneManager::Draw()
{
	if (!disableHud && hud)
		hud->HudDraw(GameManager::driver, GameManager::guienv);
}

SceneManager::Tag SceneManager::GetTag()
{
	return tag;
}

/* Loads the given scene. Unloads the currently active scene first. */
bool SceneManager::LoadScene(SceneType sceneToLoad)
{
	// Cancel if the scene we're trying to load is the same as the current scene
	if (sceneToLoad == scene)
		return false;

	// Unload current scene
	GameManager::smgr->clear();
	for (GameObject* gameObj : GameManager::gameObjects)
		delete gameObj;
	GameManager::gameObjects.clear();

	// Load the new scene
	scene = sceneToLoad;
	switch (scene) 
	{
		case TITLE_SCREEN:
		{

		} break;

		case LEVEL:
		{
			#pragma region Setup
			GameManager::device->getCursorControl()->setVisible(false);

			// Set our skydome
			ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);
			skydome->setMaterialFlag(EMF_FOG_ENABLE, true);

			// Initialize our background music
			sound_init();
			background_music("../media/AmbientUnderwaterMaddnes.ogg");

			// Adds the camera and binds the keys to the camera's movement
			camera = new Camera(GameManager::smgr);
			Lighting lighting = Lighting(GameManager::smgr);
			lighting.SetSceneLight(ambientColor);
			#pragma endregion

			#pragma region World Generation
			std::vector<io::path> meshDirectories;
			std::vector<io::path> meshTextures;

			// Spawn critters
			meshDirectories.push_back("../media/dolphin.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Fish1.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Fish2.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Fish3.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/MantaRay.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/GoldenFish.obj"); meshTextures.push_back("../media/naranjaojo.png");
			meshDirectories.push_back("../media/Rudd_Fish.obj"); meshTextures.push_back("../media/Rudd-Fish_Colourmap.png");
			for (int critterIndex = 0; critterIndex < GameManager::critterCount; critterIndex++)
			{
				int graphicsIndex = rand() % meshDirectories.size();
				Critter* critter = new Critter(new vector3df(rand() % (GameManager::WORLD_RADIUS_X * 2) - GameManager::WORLD_RADIUS_X,
					rand() % GameManager::WORLD_RADIUS_Y,
					rand() % (GameManager::WORLD_RADIUS_Z * 2) - GameManager::WORLD_RADIUS_Z),
					new vector3df(1, 1, 1),
					new vector3df(0, 0, 0),
					0,
					GameManager::smgr,
					-1111,
					GameManager::smgr->getMesh(meshDirectories[graphicsIndex]),
					meshTextures[graphicsIndex] != "" ? GameManager::driver->getTexture(meshTextures[graphicsIndex]) : 0,
					false);
				GameManager::gameObjects.push_back(critter);
			}

			// Spawn world objects
			IAnimatedMesh* playerMesh = GameManager::smgr->getMesh("../media/FPSArms.obj");


			// Spawn player
			Player* player = new Player(new vector3df(0, 0, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111);
			GameManager::gameObjects.push_back(player);

			// Attach flashlight to player
			ISceneNode* newPlayer = player;
			ILightSceneNode* flashlight = lighting.CreateSpotLight(flashlightColor, player->getAbsolutePosition(), GameManager::smgr->getActiveCamera()->getTarget(), FLASHLIGHT_RANGE, true, player);

			// Spawn shark
			Shark* shark = new Shark(new vector3df(4000, 500, 0), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				0, GameManager::smgr, -1111,
				GameManager::smgr->getMesh("../media/Shark.obj"),
				0, false);
			GameManager::gameObjects.push_back(shark);

			FlockingEntity* flockOfFish = new FlockingEntity(new vector3df(100, -80, 100), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				GameManager::smgr->getRootSceneNode(), GameManager::smgr, -500, GameManager::smgr->getMesh("../media/FishSpawn.obj"),
				GameManager::driver->getTexture("../media/GoldTexture.jpg"));
			flockOfFish->tag = GameObject::CREATURE;
			GameManager::gameObjects.push_back(flockOfFish);

			// Make a playingField (mesh out of grid)
			GridMesh* playingField = new GridMesh(new vector3df(-GameManager::WORLD_RADIUS_X - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE) / 2), -200, -GameManager::WORLD_RADIUS_Z - ((GridMesh::GRID_OFFSET * GridMesh::CELL_SIZE) / 2)), new vector3df(1, 1, 1), new vector3df(0, 0, 0),
				GameManager::smgr->getRootSceneNode(), GameManager::smgr, -100, 0, 0);

			// Grab the buffer of the playingfield mesh to be used for putting random objects on the vertex
			IMeshBuffer* planeBuffer = playingField->mesh->getMesh()->getMeshBuffer(0);

			// Key collectible object
			GameObject* key = new GameObject(new vector3df(0,50,0), new vector3df(0.5, 0.5, 0.5), SceneManager::vectorZero,
				0, GameManager::smgr, 4,
				GameManager::smgr->getMesh("../media/key.obj"),
				GameManager::driver->getTexture("../media/RustTexture.jpg"));
			key->setTag(GameObject::KEY);
			GameManager::gameObjects.push_back(key);
			ILightSceneNode* keyLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), key->getPosition() + keyLightOffset, KEYLIGHT_RADIUS, false, nullptr);

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, key, {}, true, true);

			// Win condition trigger object
			GameObject* chest = new GameObject(new vector3df(-200, -100, 150), new vector3df(13, 13, 13), new vector3df(0, 0, 0),
				0, GameManager::smgr, 5,
				GameManager::smgr->getMesh("../media/ChestCartoon.obj"),
				GameManager::driver->getTexture("../media/GoldTexture.jpg"));
			chest->mesh->setMaterialFlag(irr::video::EMF_LIGHTING, true);
			chest->setTag(GameObject::CHEST);
			ILightSceneNode* chestLight = lighting.CreatePointLight(video::SColorf(0.5f, 0.5f, 0.2f, 1.0f), chest->getPosition() + chestLightOffset, CHESTLIGHT_RADIUS, false, nullptr);
			GameManager::gameObjects.push_back(chest);

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, chest, {}, true, true);

			// Keeps placing the key and chest on a different vertex if it's too close
			while (true) 
			{
				float distance = key->getPosition().getDistanceFrom(chest->getPosition());
				if (distance < SceneManager::distanceKeyChest)
				{
					playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, key, {}, true, true);
					playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), {}, chest, {}, true, true);
				} else
				{
					break;
				}
			}

			// Used for generating random meshes when creating game objects
			int meshSelector;

			// Uses the meshDirectionaries and meshTexture list to create game objects (rock)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Rock.obj"); meshTextures.push_back("");

			// List contains rock game objects
			vector<GameObject*> rockList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::rockCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* rock = new GameObject(SceneManager::vectorZero, new vector3df(50, 50, 50), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < rock->mesh->getMaterialCount(); mIndex++)
				{
					rock->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				rockList.push_back(rock);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), rockList, {}, {});

			// Uses the meshDirectionaries and meshTexture list to create game objects (ruins)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/ruinsArc.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsCathedral.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsFoundation.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsPillar.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsTempleRuin1.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/ruinsTempleRuin2.obj"); meshTextures.push_back("");

			// List contains ruins game objects
			vector<GameObject*> ruinsList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::ruinsCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* ruins = new GameObject(SceneManager::vectorZero, new vector3df(0.2f, 0.2f, 0.2f), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < ruins->mesh->getMaterialCount(); mIndex++)
				{
					ruins->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				ruinsList.push_back(ruins);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), ruinsList, {}, {});

			// Uses the meshDirectionaries and meshTexture list to create game objects (coral)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Coral/coralBrain1.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralBrain2.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralBrain3.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Coral/coralPillar.obj"); meshTextures.push_back("");

			// List contains coral game objects
			vector<GameObject*> coralList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::coralCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* coral = new GameObject(SceneManager::vectorZero, new vector3df(1, 1, 1), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < coral->mesh->getMaterialCount(); mIndex++)
				{
					coral->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				coralList.push_back(coral);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), coralList, {}, {});

			// Uses the meshDirectionaries and meshTexture list to create game objects (vines)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Plants/tiny_weed_03_01.obj"); meshTextures.push_back("");

			// List contains coral game objects
			vector<GameObject*> vinesList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::plantCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* vines = new GameObject(SceneManager::vectorZero, new vector3df(1, 1, 1), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < vines->mesh->getMaterialCount(); mIndex++)
				{
					vines->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				vinesList.push_back(vines);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), vinesList, {}, {});

			// Uses the meshDirectionaries and meshTexture list to create game objects (skull)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/Bones/skullBig.obj"); meshTextures.push_back("");

			// List contains skull game objects
			vector<GameObject*> skullList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::skullCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* skull = new GameObject(SceneManager::vectorZero, new vector3df(0.25f, 0.25f, 0.25f), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < skull->mesh->getMaterialCount(); mIndex++)
				{
					skull->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				skullList.push_back(skull);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), skullList, {}, {});

			// Uses the meshDirectionaries and meshTexture list to create game objects (ship)
			meshDirectories.clear();
			meshTextures.clear();
			meshDirectories.push_back("../media/ship.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/SailShip.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/Boat.obj"); meshTextures.push_back("");
			meshDirectories.push_back("../media/BoatWSail.obj"); meshTextures.push_back("");

			// List contains ship game objects
			vector<GameObject*> shipList;

			// Creates x amount gameObjects 
			for (int i = 0; i < GameManager::shipCount; i++) {
				meshSelector = rand() % meshDirectories.size();
				GameObject* ship = new GameObject(SceneManager::vectorZero, new vector3df(0.5f, 0.5f, 0.5f), SceneManager::vectorZero,
					0, GameManager::smgr, SceneManager::noID,
					GameManager::smgr->getMesh(meshDirectories[meshSelector]));

				// Needs to be included to prevent textures being half transparent
				for (int mIndex = 0; mIndex < ship->mesh->getMaterialCount(); mIndex++)
				{
					ship->mesh->getMaterial(mIndex).MaterialType = EMT_TRANSPARENT_ALPHA_CHANNEL_REF;
				}

				shipList.push_back(ship);
			}

			// Adds objects on the vertices of the playingfield mesh 
			playingField->RandomObjectPlacementOnVertex(planeBuffer, playingField->getPosition(), shipList, {}, {});

			
		#pragma endregion
		} break;

		case GAME_OVER:
		{

		} break;

		default: 
		{
			return false; 
		} break;
	}

	SceneManager::OnSceneChange();
	return true;
}

/* Pauses the current scene by setting the game speed multiplier to 0. */
void SceneManager::PauseScene(bool mode)
{
	// Cancel if our new mode is the same as our current paused state
	if (mode == sceneIsPaused)
		return;

	// Pause / unpause game
	GameManager::gameSpeed = mode ? 0.0f : 1.0f;
	GameManager::smgr->getActiveCamera()->setInputReceiverEnabled(!mode);
	sceneIsPaused = mode;

	// Toggle pause menu
	if (sceneIsPaused)
	{
		Menu* pauseMenu = new Menu(new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
			Menu::PAUSE_MENU, 0, GameManager::smgr, 10000);
		pauseMenu->elementWidth = 200.0f;
		pauseMenu->elementHeight = 250.0f;
		pauseMenu->windowTitle = "Pause Menu";
		pauseMenu->setPosition(vector3df(GameManager::screenDimensions.Width / 2.0f - pauseMenu->elementWidth / 2.0f,
			GameManager::screenDimensions.Height / 2.0f - pauseMenu->elementHeight / 2.0f, 0.0f));
		GameManager::interfaceObjects.push_back(pauseMenu);

		int buttonCount = 2;
		float buttonWidth = pauseMenu->elementWidth - pauseMenu->elementSpacing * 2.0f;
		float buttonHeight = 50.0f;
		float buttonStartX = pauseMenu->getPosition().X + pauseMenu->elementWidth / 2.0f - buttonWidth / 2.0f;
		float buttonStartY = pauseMenu->getPosition().Y + pauseMenu->elementHeight - 1 -
			((buttonHeight + pauseMenu->elementSpacing) * buttonCount);

		for (int bIndex = 0; bIndex < buttonCount; bIndex++)
		{
			Button* button = new Button(new vector2df(buttonStartX, buttonStartY + ((buttonHeight + pauseMenu->elementSpacing) * bIndex)), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
				(Button::ButtonType)bIndex, 0, GameManager::smgr, 15000);
			button->creator = pauseMenu;
			button->elementWidth = buttonWidth;
			button->elementHeight = buttonHeight;
			GameManager::interfaceObjects.push_back(button);
		}
	}
	else
	{
		// Delete the pause menu
		Menu* pauseMenu = (Menu*)GameManager::FindGameObjectWithTag<InterfaceObject>(DynamicUpdater::INTERFACE_MENU, GameManager::interfaceObjects);
		if (pauseMenu)
			pauseMenu->~Menu();
	}
}

/* Triggers whenever a scene switch happens.*/
void SceneManager::OnSceneChange()
{
	std::cout << "OnSceneChange, from " << (int)scenePrevious << " to " << (int)scene << std::endl;
	scenePrevious = scene;
}

/* Creates a loading screen effect.
Used to smoothly load a scene before showing it.*/
void SceneManager::StartLoadingScreen()
{
	// TODO: Loading screen!
}

/* Ends the currently active loading screen.*/
void SceneManager::EndLoadingScreen()
{
	// TODO: Loading screen!
}