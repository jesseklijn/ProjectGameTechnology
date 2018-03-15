#include <irrlicht.h>
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

	// adds the camera and binds the keys to the camera's movement
	Camera camera = Camera(smgr);

	// makes the player object, which is also added to smgr to be drawn
	Player player = Player(smgr->getActiveCamera(), smgr, -1111);
	
	device->getCursorControl()->setVisible(false);


	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();
	float delta = 0.01;

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

		vector3df position = player.getPosition();
		u32 now = device->getTimer()->getTime();
		if (now - then > 1000) {
			delta *= -1;
			then = now;
		}
		player.setPosition(position + vector3df(delta, 0, 0));
	}

	device->drop();

	return 0;
}
