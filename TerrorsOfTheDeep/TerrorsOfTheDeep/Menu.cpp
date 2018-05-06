#pragma once
#include "Menu.h"
#include "GameManager.h"


Menu::Menu(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation, 
	MenuType menuType, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id)
	: InterfaceObject(startPosition, startRotation, startScale, parent, mgr, id)
{
	tag = INTERFACE_MENU;
	this->menuType = menuType;
	positionWindowTitle = vector2df(startPosition->X + elementWidth / 2.0f, startPosition->Y + elementSpacing);

	/*Button* button = new Button(new vector2df(getPosition().X, getPosition().Y), new vector2df(0.0f, 0.0f), new vector2df(0.0f, 0.0f),
		Button::PM_RESUME, this, GameManager::smgr, 15000);
	components.push_back(button);
	GameManager::interfaceObjects.push_back(button);*/
}

Menu::~Menu()
{
	// When this element is deleted, delete all children
	//for (InterfaceObject* component : components)
		//delete component;
}

void Menu::DrawGUI()
{
	if (elementHidden)
		return;

	// Draw window
	if (hasWindow)
	{
		GameManager::driver->draw2DRectangle(windowColor, rect<s32>(getPosition().X, getPosition().Y,
			getPosition().X + elementWidth, getPosition().Y + elementHeight));
	}
	// Draw window title
	if (hasWindowTitle)
	{
		IGUIStaticText* title = GameManager::guienv->addStaticText(windowTitle.c_str(),
			rect<s32>(getPosition().X, getPosition().Y + elementSpacing, getPosition().X + elementWidth - 1, getPosition().Y + elementHeight - 1));
		title->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);
		title->setOverrideColor(windowTitleColor);
	}
}