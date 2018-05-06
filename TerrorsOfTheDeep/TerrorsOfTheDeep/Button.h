#pragma once
#include "InterfaceObject.h"
#include "EventManager.h"
#include "SceneManager.h"


class Button : public InterfaceObject
{
public:
	enum ButtonType
	{
		PM_RESUME,
		PM_BACK_TO_MAIN,
		PM_QUIT
	};
	ButtonType buttonType;

	Button(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		ButtonType buttonType,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	~Button();

	irr::video::SColor buttonColor = irr::video::SColor(255.0f * alpha, 100.0f, 100.0f, 100.0f);
	irr::video::SColor buttonTitleColor = irr::video::SColor(255.0f * alpha, 255.0f, 255.0f, 255.0f);
	irr::core::stringw buttonTitle = "<BUTTON>";
	irr::core::vector2df positionWindowTitle;

	void Update();
	void DrawGUI();
	void ButtonMouseEnter();
	void ButtonMouseLeave();
	void ButtonPressed();

private:
	bool buttonMousedOver = false;
	bool buttonMousedOverPrevious = false;
};

