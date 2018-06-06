#pragma once
#include "Button.h"
#include "GameManager.h"
#include "Fader.h"


Button::Button(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation,
	ButtonType buttonType, irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id)
	: InterfaceObject(startPosition, startScale, startRotation, parent, mgr, id)
{
	tag = INTERFACE_BUTTON;
	this->buttonType = buttonType;

	switch (buttonType)
	{
	case MM_START:
	{
		buttonTitle = "Start Game";
	}
	break;

	case PM_RESUME:
	{
		buttonTitle = "Resume Game";
	} break;

	case PM_BACK_TO_MAIN: case GO_BACK_TO_MAIN:
	{
		buttonTitle = "Main Menu";
	} break;

	case PM_QUIT: case MM_QUIT:
	{
		buttonTitle = "Quit Game";
	} break;

	case GO_RETRY:
	{
		buttonTitle = "Retry";
	} break;

	default: break;
	}
}

Button::~Button()
{

}

void Button::Update()
{
	buttonMousedOver = false;
	float mouseX = EventManager::MouseState.Position.X;
	float mouseY = EventManager::MouseState.Position.Y;

	// Detect if moused over button
	if (mouseX >= getPosition().X && mouseX < getPosition().X + elementWidth - 1)
	{
		if (mouseY >= getPosition().Y && mouseY < getPosition().Y + elementHeight - 1)
		{
			buttonMousedOver = true;

			// Trigger if mouse button is then pressed
			if (EventManager::MouseState.LeftButtonDown)
				ButtonPressed();
		}
	}

	// Detect mouse enter/leave changes
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

	// Draw button body
	GameManager::driver->draw2DRectangle(buttonColor, rect<s32>(getPosition().X, getPosition().Y,
		getPosition().X + elementWidth, getPosition().Y + elementHeight));

	// Draw button text
	IGUIStaticText* title = GameManager::guienv->addStaticText(buttonTitle.c_str(),
		rect<s32>(getPosition().X, getPosition().Y + elementSpacing, getPosition().X + elementWidth - 1, getPosition().Y + elementHeight - 1));
	title->setTextAlignment(EGUIA_CENTER, EGUIA_UPPERLEFT);
	title->setOverrideColor(buttonTitleColor);
}

/* Triggers when the mouse enters the bounding box of the button. */
void Button::ButtonMouseEnter()
{
	buttonColor = buttonHighlightedColor;
	buttonTitleColor = buttonTitleHighlightedColor;
}

/* Triggers when the mouse leaves the bounding box of the button. */
void Button::ButtonMouseLeave()
{
	buttonColor = buttonDefaultColor;
	buttonTitleColor = buttonTitleDefaultColor;
}

/* Triggers when the button was pressed and determines what to do
depending on the button's type. */
void Button::ButtonPressed()
{
	//if (SceneManager::fader->isFading)
		//return;

	switch (buttonType)
	{
	case PM_RESUME:
	{
		SceneManager::PauseScene(false);
	} break;

	case PM_BACK_TO_MAIN: case GO_BACK_TO_MAIN:
	{
		SceneManager::fader->faderMode = Fader::FADE_OUT;
		SceneManager::faderAction = SceneManager::FaderAction::SCENE_SWITCH_TO_MAIN;
		SceneManager::fader->isFading = true;
	}
	break;;

	case PM_QUIT: case MM_QUIT:
	{
		GameManager::device->closeDevice();
	} break;

	case GO_RETRY: case MM_START:
	{
		SceneManager::fader->faderMode = Fader::FADE_OUT;
		SceneManager::fader->alpha = 0.0f;
		SceneManager::faderAction = SceneManager::FaderAction::SCENE_SWITCH_TO_LEVEL;
		SceneManager::fader->isFading = true;
	} break;

	default: break;
	}
}
