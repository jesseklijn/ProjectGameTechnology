#include <irrlicht.h>
#include "Player.h"

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

const float MOVEMENT_SPEED = (float)0.5;
//const float sidewaysSpeed = (float)0.1;
const float ROTATION_SPEED = (float)25;

int main()
{
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hi kids! This is what your parents don't want you to see!",
		rect<s32>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh("../media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}

	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode(mesh);
	if (node)
	{
		node->setPosition(vector3df(0, 0, 60));
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../media/sydney.bmp") );
	}

	
	SKeyMap keyMap[4];

	keyMap[0].Action = EKA_MOVE_FORWARD;
	keyMap[0].KeyCode = KEY_KEY_W;

	keyMap[1].Action = EKA_MOVE_BACKWARD;
	keyMap[1].KeyCode = KEY_KEY_S;

	keyMap[2].Action = EKA_STRAFE_LEFT;
	keyMap[2].KeyCode = KEY_KEY_A;

	keyMap[3].Action = EKA_STRAFE_RIGHT;
	keyMap[3].KeyCode = KEY_KEY_D;

	smgr->addCameraSceneNodeFPS(0, ROTATION_SPEED, MOVEMENT_SPEED, -100, keyMap, 4); 
	// parent, rotation speed, move speed, id, keymaparray, keymapsize (+ noVerticalMovement, jumpSpeed, invertMouse, makeActive)

	// makes the player object, which is also added to smgr to be drawn
	Player player = Player(smgr->getActiveCamera(), smgr, -1111);
	
	device->getCursorControl()->setVisible(false);


	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		// to display the fps in the window caption
		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			stringw tmp(L"TERRORS OF THE DEEP. fps: ");
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}
