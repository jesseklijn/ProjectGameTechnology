#pragma once
#include "InterfaceObject.h"


class Menu : public InterfaceObject
{
public:
	enum MenuType
	{
		MAIN_MENU,
		PAUSE_MENU,
		LOADING_SCREEN,
		END_MENU
	};

	Menu(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		MenuType menuType,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	~Menu();

	MenuType menuType;

	bool hasWindow = true;
	bool hasWindowTitle = true;
	irr::core::stringw windowTitle = "<MENU>";
	irr::video::SColor windowColor = irr::video::SColor(255.0f, 0.0f, 0.0f, 0.0f);
	irr::video::SColor windowTitleColor = irr::video::SColor(255.0f, 255.0f, 255.0f, 255.0f);
	irr::core::vector2df positionWindowTitle;

	virtual void DrawGUI();
};

