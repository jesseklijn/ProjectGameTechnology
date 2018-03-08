#include <irrlicht.h>

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

class Player
{
public:
	float speed;
	float sidewaysSpeed;
	// int oxygen
	Player()
	{
		speed = (float)0.5;
		sidewaysSpeed = (float)0.1;
	}
};

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

	Player player = Player();

	//float zCameraSpeed = 0.1;
	float zCameraSpeed = player.speed;
	float rotationCameraSpeed = 75;
	SKeyMap keyMap[8];

	keyMap[1].Action = EKA_MOVE_FORWARD;
	keyMap[1].KeyCode = KEY_KEY_W;

	keyMap[2].Action = EKA_MOVE_BACKWARD;
	keyMap[2].KeyCode = KEY_KEY_S;

	keyMap[3].Action = EKA_STRAFE_LEFT;
	keyMap[3].KeyCode = KEY_KEY_A;

	keyMap[4].Action = EKA_STRAFE_RIGHT;
	keyMap[4].KeyCode = KEY_KEY_D;

	smgr->addCameraSceneNodeFPS(0, rotationCameraSpeed, zCameraSpeed, -100, keyMap, 8); //(?, rotation speed, forward speed, ? , keymap, array keys keymap

	ISceneNode * playerNode = smgr->addCubeSceneNode();
	if (playerNode)
	{
		playerNode->setMaterialTexture(0, driver->getTexture("../../media/t351sml.jpg"));
		playerNode->setMaterialFlag(EMF_LIGHTING, false);
		smgr->getActiveCamera()->addChild(playerNode);
	}
	
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 5.f;

	while(device->run())
	{
		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

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
