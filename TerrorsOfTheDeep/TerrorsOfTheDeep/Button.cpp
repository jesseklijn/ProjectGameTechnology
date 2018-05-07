#include "Button.h"
#include "GameManager.h"


Button::Button(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation, 
	ButtonType buttonType, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id)
	: InterfaceObject(startPosition, startScale, startRotation, parent, mgr, id)
{
	tag = INTERFACE_BUTTON;
	this->buttonType = buttonType;

	switch (buttonType)
	{
		case PM_RESUME:
		{
			buttonTitle = "Resume Game";
		} break;

		case PM_BACK_TO_MAIN:
		{
			buttonTitle = "Main Menu";
		} break;

		case PM_QUIT:
		{
			buttonTitle = "Quit";
		} break;
	}
}

Button::~Button()
{
	std::cout << buttonType << ", creator: " << creator << std::endl;
}

void Button::Update()
{
	buttonMousedOver = false;
	float mouseX = EventManager::MouseState.Position.X;
	float mouseY = EventManager::MouseState.Position.Y;

	if (mouseX >= getPosition().X && mouseX < getPosition().X + elementWidth - 1)
	{
		if (mouseY >= getPosition().Y && mouseY < getPosition().Y + elementHeight - 1)
		{
			buttonMousedOver = true;
			if (EventManager::MouseState.LeftButtonDown)
			{
				ButtonPressed();
			}
		}
	}

	if (buttonMousedOver != buttonMousedOverPrevious)
	{
		if (buttonMousedOver)
			ButtonMouseEnter();
		else
			ButtonMouseLeave();

		buttonMousedOverPrevious = buttonMousedOver;
	}
}

void Button::DrawGUI()
{
	if (elementHidden)
		return;

	// Draw window
	GameManager::driver->draw2DRectangle(buttonColor, rect<s32>(getPosition().X, getPosition().Y,
		getPosition().X + elementWidth, getPosition().Y + elementHeight));

	IGUIStaticText* title = GameManager::guienv->addStaticText(buttonTitle.c_str(),
		rect<s32>(getPosition().X, getPosition().Y + elementSpacing, getPosition().X + elementWidth - 1, getPosition().Y + elementHeight - 1));
	title->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);
	title->setOverrideColor(buttonTitleColor);
}

void Button::ButtonMouseEnter()
{

}

void Button::ButtonMouseLeave()
{

}

void Button::ButtonPressed()
{
	switch (buttonType)
	{
		case PM_RESUME:
		{
			if (creator)
				delete creator;
		} break;

		case PM_BACK_TO_MAIN:
		{
			SceneManager::LoadScene(SceneManager::TITLE_SCREEN);
		} break;

		case PM_QUIT:
		{
			GameManager::device->closeDevice();
		} break;
	}
}
