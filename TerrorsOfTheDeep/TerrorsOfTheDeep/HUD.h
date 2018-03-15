#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

class HUD
{
public:
	HUD();
	~HUD();
	void HudDraw(int stamina, bool itemPickedUpArray[], IVideoDriver* driver, IGUIEnvironment* guienv);
};


