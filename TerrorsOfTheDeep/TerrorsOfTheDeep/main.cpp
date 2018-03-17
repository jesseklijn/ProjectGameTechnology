#include <irrlicht.h>
#include "addLighting.h"
#include "HUD.h"
#include "Player.h"
#include "Camera.h"

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif


int main()
{

	IrrlichtDevice *device =
		createDevice(video::EDT_DIRECT3D9, dimension2d<u32>(1024, 720), 64,
			false, true, false, 0);

	if (!device)
		return 1;

	device->setWindowCaption(L"Terrors of the Deep - Vertical Slice");
	
	sound_init();
	background_music("../media/JawsTheme.ogg");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	// array of SceneNode's, Don't need this for now. 
	//core::array<scene::ISceneNode *> objects;

	IAnimatedMesh* sharkMesh = smgr->getMesh("../media/shark.obj");
	IAnimatedMesh* rockMesh = smgr->getMesh("../media/rock.obj");
	if (!sharkMesh || !rockMesh)
	{
		device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* shark = smgr->addAnimatedMeshSceneNode(sharkMesh);
	IAnimatedMeshSceneNode* rock = smgr->addAnimatedMeshSceneNode(rockMesh);

	if (shark)
	{
		shark->setMaterialFlag(EMF_LIGHTING, false);
		shark->setMD2Animation(scene::EMAT_STAND);
		shark->setMaterialTexture(1, driver->getTexture("../media/Shark_ Bump.jpg"));
		shark->setScale(vector3df(20, 20, 20));
	}

	if (rock)
	{
		rock->setMaterialFlag(EMF_LIGHTING, false);
		rock->setMD2Animation(scene::EMAT_STAND);
		rock->setMaterialTexture(0, driver->getTexture("../media/RockTexture.jpg"));
		rock->setScale(vector3df(20, 20, 20));
		rock->setPosition(vector3df(0, 20, 0));
	}

	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture("../media/irrlicht2_up.jpg"),
		driver->getTexture("../media/irrlicht2_dn.jpg"),
		driver->getTexture("../media/irrlicht2_lf.jpg"),
		driver->getTexture("../media/irrlicht2_rt.jpg"),
		driver->getTexture("../media/irrlicht2_ft.jpg"),
		driver->getTexture("../media/irrlicht2_bk.jpg"));

	scene::ISceneNode* skydome = smgr->addSkyDomeSceneNode(driver->getTexture("../media/Skydome_LED_BayDarkBlue.psd"), 16, 8, 0.95f, 2.0f);

	gui::IGUIFont* font = device->getGUIEnvironment()->getBuiltInFont();
	guienv->getSkin()->setFont(font);

	// stamina variable for swimming faster etc
	int stamina = 0;

	//check if the items are picked up
	bool itemPickedUp[3] = { true,true,true };

	//create hud object
	HUD* hud = new HUD;
	bool disableHud = false;

	// adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(smgr);

	Player player = Player(smgr->getActiveCamera(), smgr, -1111, device);
	// makes the player object, which is also added to smgr to be drawn


	device->getCursorControl()->setVisible(false);

	while (device->run())
	{
		driver->beginScene(true, true, SColor(255, 100, 101, 140));

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

		smgr->drawAll();

		guienv->clear();
		//Hud(stamina, itemPickedUp, driver,guienv);
		if (!disableHud) {
			hud->HudDraw(stamina, itemPickedUp, driver, guienv);
		}
		//stamina recovery
		if (stamina >= 0 && stamina < 1000) {
			stamina++;
		}
		guienv->drawAll();
		driver->endScene();
		player.updatePos();


	}
	device->drop();
	sound_shutdown();

	return 0;
}
