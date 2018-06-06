#pragma once
#include "InterfaceObject.h"
#include "EventManager.h"
#include "SceneManager.h"


class Button : public InterfaceObject
{
public:
	enum ButtonType
	{
		// MM = Main Menu PM = Pause Menu GO = Game Over
		
		PM_RESUME,
		PM_BACK_TO_MAIN,
		PM_QUIT,
		GO_RETRY,
		GO_BACK_TO_MAIN,
		MM_START,
		MM_QUIT,
	};
	ButtonType buttonType;

	Button(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		ButtonType buttonType,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	~Button();
	
	// Define default and highlighted color when moused over
	irr::video::SColor buttonDefaultColor = irr::video::SColor(255.0f * alpha, 100.0f, 100.0f, 100.0f);
	irr::video::SColor buttonHighlightedColor = irr::video::SColor(255.0f * alpha, 140.0f, 140.0f, 140.0f);
	irr::video::SColor buttonTitleDefaultColor = irr::video::SColor(255.0f * alpha, 240.0f, 240.0f, 240.0f);
	irr::video::SColor buttonTitleHighlightedColor = irr::video::SColor(255.0f * alpha, 255.0f, 255.0f, 255.0f);

	irr::video::SColor buttonColor = buttonDefaultColor;
	irr::video::SColor buttonTitleColor = buttonTitleDefaultColor;

	irr::core::stringw buttonTitle = "<BUTTON>";

	void Update();
	void DrawGUI();
	void ButtonMouseEnter();
	void ButtonMouseLeave();
	void ButtonPressed();

private:
	bool buttonMousedOver = false;
	bool buttonMousedOverPrevious = false;
};

