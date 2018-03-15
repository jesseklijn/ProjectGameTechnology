
#include <irrlicht.h>
#include "addLighting.h"


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
		createDevice(video::EDT_OPENGL, dimension2d<u32>(640, 480), 16,
			false, true, false, 0);

	if (!device)
		return 1;


	device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");


	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();


	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10, 10, 260, 22), true);


	float zCameraSpeed = 1;
	float rotationCameraSpeed = 100;
	SKeyMap keyMap[8];

	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[3].Action = EKA_MOVE_BACKWARD;
	keyMap[3].KeyCode = KEY_KEY_S;

	keyMap[5].Action = EKA_STRAFE_LEFT;
	keyMap[5].KeyCode = KEY_KEY_A;

	keyMap[7].Action = EKA_STRAFE_RIGHT;
	keyMap[7].KeyCode = KEY_KEY_D;

	smgr->addCameraSceneNodeFPS(0, rotationCameraSpeed, zCameraSpeed, -100, keyMap, 8); //(?, rotation speed, forward speed, ? , keymap, array keys keymap
	device->getCursorControl()->setVisible(false);


	while (device->run())
	{

		driver->beginScene(true, true, SColor(255, 100, 100, 140));

		smgr->drawAll();

		guienv->drawAll();

		driver->endScene();
	}


	device->drop();

	return 0;
}