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

	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	//guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
	//	rect<s32>(10, 10, 260, 22), true);

	IAnimatedMesh* sharkMesh = smgr->getMesh("../media/shark.obj");
	IAnimatedMesh* rockMesh = smgr->getMesh("../media/rock.obj");
	if (!sharkMesh || !rockMesh)
	{
		device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* shark = smgr->addAnimatedMeshSceneNode( sharkMesh );
	IAnimatedMeshSceneNode* rock = smgr->addAnimatedMeshSceneNode(rockMesh);

	if (shark)
	{
		shark->setMaterialFlag(EMF_LIGHTING, false);
		shark->setMD2Animation(scene::EMAT_STAND);
		shark->setMaterialTexture(1,driver->getTexture("../media/Shark_ Bump.jpg"));
		shark->setScale(vector3df(20,20,20));
	}

	if (rock)
	{
		rock->setMaterialFlag(EMF_LIGHTING, false);
		rock->setMD2Animation(scene::EMAT_STAND);
		rock->setMaterialTexture(0, driver->getTexture("../media/RockTexture.jpg"));
		rock->setScale(vector3df(20, 20, 20));
		rock->setPosition(vector3df(0,20,0));
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
	bool itemPickedUp[3] = {true,true,true};

	//create hud object
	HUD* hud = new HUD;
	bool disableHud = false;

	// adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(smgr);

	// makes the player object, which is also added to smgr to be drawn
	Player player = Player(smgr->getActiveCamera(), smgr, -1111, device);
	
	device->getCursorControl()->setVisible(false);

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

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

	return 0;
}
