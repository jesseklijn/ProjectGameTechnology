//All Includes
#include <irrlicht.h>
#include "addLighting.h"
#include "HUD.h"
#include "Player.h"
#include "Camera.h"
#include "Sound.h"
#include "GameManager.h"
#include "Monster.h"

//Main namespace
using namespace irr;

//Namespaces of Irrlicht
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	//Add the Game Manager here
	GameManager gameManager;
	Monster monster;



	GameManager::device->setWindowCaption(L"Terrors of the Deep - Vertical Slice");
	
	sound_init();
	background_music("../media/JawsTheme.ogg");

	// array of SceneNode's, Don't need this for now. 
	//core::array<scene::ISceneNode *> objects;

	IAnimatedMesh* sharkMesh = GameManager::smgr->getMesh("../media/shark.obj");
	IAnimatedMesh* rockMesh = GameManager::smgr->getMesh("../media/rock.obj");
	if (!sharkMesh || !rockMesh)
	{
		GameManager::device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* shark = GameManager::smgr->addAnimatedMeshSceneNode(sharkMesh);
	IAnimatedMeshSceneNode* rock = GameManager::smgr->addAnimatedMeshSceneNode(rockMesh);

	if (shark)
	{
		shark->setMaterialFlag(EMF_LIGHTING, false);
		shark->setMD2Animation(scene::EMAT_STAND);
		shark->setMaterialTexture( 0, GameManager::driver->getTexture("../media/Shark_Texture.jpg") );
	}

	if (rock)
	{
		rock->setMaterialFlag(EMF_LIGHTING, false);
		rock->setMD2Animation(scene::EMAT_STAND);
		rock->setMaterialTexture(0, GameManager::driver->getTexture("../media/RockTexture.jpg"));
		rock->setScale(vector3df(20, 20, 20));
		rock->setPosition(vector3df(0, 20, 0));
	}

	scene::ISceneNode* skybox = GameManager::smgr->addSkyBoxSceneNode(
		GameManager::driver->getTexture("../media/irrlicht2_up.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_dn.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_lf.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_rt.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_ft.jpg"),
		GameManager::driver->getTexture("../media/irrlicht2_bk.jpg"));

	scene::ISceneNode* skydome = GameManager::smgr->addSkyDomeSceneNode(GameManager::driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);

	gui::IGUIFont* font = GameManager::device->getGUIEnvironment()->getBuiltInFont();
	GameManager::guienv->getSkin()->setFont(font);

	// stamina variable for swimming faster etc
	int stamina = 0;

	//check if the items are picked up
	bool itemPickedUp[3] = { true,true,true };

	//create hud object
	HUD* hud = new HUD;
	bool disableHud = false;

	// adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(GameManager::smgr);

	Player player = Player(GameManager::smgr->getActiveCamera(), GameManager::smgr, -1111, GameManager::device);
	// makes the player object, which is also added to smgr to be drawn


	GameManager::device->getCursorControl()->setVisible(false);

	gameManager.Awake();
	gameManager.Start();

	while (GameManager::device->run())
	{
		GameManager::driver->beginScene(true, true, SColor(255, 100, 101, 140));
		gameManager.Update();
		gameManager.Draw();
		//if (InRange(*player, shark)) {
			//font->draw(L"In range", );
			//font->draw(stringw(InRange(cube, cube2)).c_str(), rect<s32>(12, 80, 100, 100), SColor(1, 255, 255, 255));


			// WATCH OUT!! This causes the cube to be added to the array 60 times per second!
			// Add this functionality later, if necessary. 
			//objects.push_back(cube);
			//font->draw(stringw(length).c_str(), rect<s32>(12, 80, 100, 100), SColor(1, 255, 255, 255));
	//}

	// collision code.
	/*if (Col(player, shark, 10)) {
		player->setPosition(player->getPosition() + vector3df(rand() % 1 - 1,0,0));
	}
	else { player->setPosition(player->getPosition() + vector3df(0, 0, 0.05)); }

	if (Col(shark, player, 10)) {
		shark->setPosition(shark->getPosition() + vector3df(0, 0, 0));
	}
	else { shark->setPosition(shark->getPosition() + vector3df(0, 0, -0.05)); }*/

		GameManager::smgr->drawAll();

		GameManager::guienv->clear();
		//Hud(stamina, itemPickedUp, driver,guienv);
		if (!disableHud) {
			hud->HudDraw(stamina, itemPickedUp, GameManager::driver, GameManager::guienv);
		}
		//stamina recovery
		if (stamina >= 0 && stamina < 1000) {
			stamina++;
		}
		GameManager::guienv->drawAll();
		GameManager::driver->endScene();
		player.updatePos();


	}
	GameManager::device->drop();
	sound_shutdown();

	return 0;
}
