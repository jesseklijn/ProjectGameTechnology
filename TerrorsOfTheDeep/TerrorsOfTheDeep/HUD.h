#pragma once
#include <irrlicht.h>

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

// Defines a Heads Up Display (HUD) for the player.
class HUD
{
public:
	// Constructor and destructor
	HUD();
	~HUD();

	// Functions
	void HudDraw(int stamina, bool itemPickedUpArray[], IVideoDriver* driver, IGUIEnvironment* guienv);
};