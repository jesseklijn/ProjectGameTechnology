#include <irrlicht.h>;

using namespace irr; 
using namespace core;
using namespace scene;
using namespace video; 
using namespace gui; 

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

	int DetectCollision() {

		IrrlichtDevice *device =
			createDevice(video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
				false, false, false, 0);

		IVideoDriver* video = device->getVideoDriver();
		ISceneManager* smgr = device->getSceneManager();


		// creating some cubes to test the collision.
		ISceneNode* cube = smgr->addCubeSceneNode();
		cube->setPosition(vector3df(0, 0, 5));




		while (device->run() && device) {
			smgr->drawAll();
			video->endScene();
		}



		return 0;
	}
