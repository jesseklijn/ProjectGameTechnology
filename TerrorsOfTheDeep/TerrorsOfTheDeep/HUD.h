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

	//Variables
	float spacingText = 22;
	float spacingImage = 60;
	float hudItemCoordinateX = 800;
	float hudItemCoordinateY = 60;

	// Functions
	void HudDraw(int stamina, IVideoDriver* driver, IGUIEnvironment* guienv);
};