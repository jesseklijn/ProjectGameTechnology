#pragma once
#include "InterfaceObject.h"



class Fader : public InterfaceObject
{
public:
	Fader(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	~Fader();

	enum FaderMode
	{
		FADE_IN,
		FADE_OUT
	};
	FaderMode faderMode = FADE_IN;

	bool isFading = true;
	float alphaIncrement = 1.0f;


	virtual void Update();
	virtual void DrawGUI();
};