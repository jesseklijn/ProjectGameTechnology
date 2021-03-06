#pragma once
#include "InterfaceObject.h"
#include "Button.h"


class Menu : public InterfaceObject
{
public:
	enum MenuType
	{
		MAIN_MENU,
		PAUSE_MENU,
		LOADING_SCREEN,
		END_MENU,
		OVERLAY,
		FADER
	};
	MenuType menuType;

	Menu(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		MenuType menuType,
		irr::scene::ISceneNode* parent, 
		irr::scene::ISceneManager* mgr, 
		irr::s32 id,
		bool showCursor = true);
	~Menu();	

	// All interface objects linked with this menu
	std::vector<InterfaceObject*> components;

	bool hasWindow = true;
	bool hasWindowBorder = true;
	bool hasWindowTitle = true;
	bool hasBackground = false;
	irr::video::ITexture* background = nullptr;
	irr::core::stringw windowTitle = "<MENU>";
	irr::video::SColor windowColor = irr::video::SColor(255.0f, 0.0f, 0.0f, 0.0f);
	irr::video::SColor windowBorderColor = irr::video::SColor(255.0f, 100.0f, 100.0f, 100.0f);
	irr::video::SColor windowTitleColor = irr::video::SColor(255.0f, 255.0f, 255.0f, 255.0f);
	irr::core::vector2df positionWindowTitle;

	virtual void DrawGUI();

private:
	bool shownCursor = true;
};

