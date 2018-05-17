#pragma once
#include "Menu.h"
#include "GameManager.h"


Menu::Menu(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation, 
	MenuType menuType, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id, bool showCursor)
	: InterfaceObject(startPosition, startRotation, startScale, parent, mgr, id)
{
	tag = INTERFACE_MENU;
	this->menuType = menuType;
	positionWindowTitle = vector2df(startPosition->X + elementWidth / 2.0f, startPosition->Y + elementSpacing);

	if (showCursor)
	{
		GameManager::device->getCursorControl()->setVisible(true);
		shownCursor = showCursor;
	}
}

Menu::~Menu()
{
	if (shownCursor)
		GameManager::device->getCursorControl()->setVisible(false);
}

void Menu::DrawGUI()
{
	if (elementHidden)
		return;

	// Draw background if applicable (stretched fullscreen behind menu)
	if (hasBackground && background != nullptr)
	{
		GameManager::driver->draw2DImage(background, 
			rect<s32>(0, 0, GameManager::screenDimensions.Width, GameManager::screenDimensions.Height),
			rect<s32>(0, 0, background->getOriginalSize().Width, background->getOriginalSize().Height));
	}
	// Draw menu window
	if (hasWindow)
	{
		GameManager::driver->draw2DRectangle(windowColor, rect<s32>(getPosition().X, getPosition().Y,
			getPosition().X + elementWidth, getPosition().Y + elementHeight));
	}
	if (hasWindowBorder)
	{
		GameManager::driver->draw2DRectangleOutline(rect<s32>(getPosition().X, getPosition().Y,
			getPosition().X + elementWidth, getPosition().Y + elementHeight), windowBorderColor);
	}
	// Draw menu title
	if (hasWindowTitle)
	{
		IGUIStaticText* title = GameManager::guienv->addStaticText(windowTitle.c_str(),
			rect<s32>(getPosition().X + elementSpacing, getPosition().Y + elementSpacing, 
				getPosition().X + elementWidth - 1 - elementSpacing, getPosition().Y + elementHeight - 1 - elementSpacing));
		title->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);
		title->setOverrideColor(windowTitleColor);
	}
}