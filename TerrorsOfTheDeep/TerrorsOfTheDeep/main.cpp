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


class MyEventReceiver : public IEventReceiver
{
public:
	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event)
	{
		// Remember whether each key is down or up
		if (event.EventType == irr::EET_KEY_INPUT_EVENT)
			KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

		return false;
	}

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const
	{
		return KeyIsDown[keyCode];
	}

	MyEventReceiver()
	{
		for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
			KeyIsDown[i] = false;
	}

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
};

int main()
{
	MyEventReceiver receiver;

	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, &receiver);

	if (!device)
		return 1;

	IVideoDriver* driver = device->getVideoDriver();
	ISceneManager* smgr = device->getSceneManager();
	IGUIEnvironment* guienv = device->getGUIEnvironment();

	guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	IAnimatedMesh* mesh = smgr->getMesh("../media/sydney.md2");
	if (!mesh)
	{
		device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );

	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, driver->getTexture("../media/sydney.bmp") );
	}

	float zCameraSpeed = 1;
	float rotationCameraSpeed = 100;
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
	device->getCursorControl()->setVisible(false);

	int lastFPS = -1;

	// In order to do framerate independent movement, we have to know
	// how long it was since the last frame
	u32 then = device->getTimer()->getTime();

	// This is the movemen speed in units per second.
	const f32 MOVEMENT_SPEED = 5.f;

	while(device->run())
	{
		// Work out a frame delta time.
		const u32 now = device->getTimer()->getTime();
		const f32 frameDeltaTime = (f32)(now - then) / 1000.f; // Time in seconds
		then = now;

		vector3df nodePosition = node->getPosition();

		// gets direction camera is pointed in
		ICameraSceneNode *pCamera = smgr->getActiveCamera();
		vector3df pos = pCamera->getAbsolutePosition();
		vector3df target = pCamera->getTarget();
		vector3df dir = target - pos;

		vector3df rot = pCamera->getRotation();

		if (receiver.IsKeyDown(irr::KEY_UP))
			nodePosition += 5 * dir * frameDeltaTime;
		else if (receiver.IsKeyDown(irr::KEY_DOWN))
			nodePosition -= 5 * dir * frameDeltaTime;

		// for left and right directions
		f32 roty_rad = rot.Y;
		roty_rad -= 90;
		roty_rad *= PI / 180;
		vector3df rotPos = vector3df(sin(roty_rad), 0, cos(roty_rad));
		if (receiver.IsKeyDown(irr::KEY_LEFT))
			nodePosition += 10 * rotPos * frameDeltaTime;
		else if (receiver.IsKeyDown(irr::KEY_RIGHT))
			nodePosition -= 10 * rotPos * frameDeltaTime;

		node->setPosition(nodePosition);

		driver->beginScene(true, true, SColor(255,100,101,140));

		smgr->drawAll();
		guienv->drawAll();

		driver->endScene();

		int fps = driver->getFPS();

		if (lastFPS != fps)
		{
			core::stringw tmp(L"Movement Example - Irrlicht Engine [");
			tmp += driver->getName();
			tmp += L"] fps: ";
			tmp += fps;

			device->setWindowCaption(tmp.c_str());
			lastFPS = fps;
		}
	}

	device->drop();

	return 0;
}
