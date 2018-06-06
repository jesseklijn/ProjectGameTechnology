#pragma once
#include "Fader.h"
#include "GameManager.h";
#include "SceneManager.h"


Fader::Fader(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation, 
	irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id)
	: InterfaceObject(startPosition, startScale, startRotation, parent, mgr, id)
{

}

Fader::~Fader()
{

}

void Fader::Update()
{
	switch (faderMode)
	{
		case FADE_IN:
		{
			alpha = GameManager::Clamp(alpha - alphaIncrement * GameManager::deltaTime, 0.0f, 1.0f);
			isFading = alpha != 0.0f;
		} break;

		case FADE_OUT:
		{
			alpha = GameManager::Clamp(alpha + alphaIncrement * GameManager::deltaTime, 0.0f, 1.0f);
			isFading = alpha != 1.0f;
			if (alpha == 1.0f)
			{
				faderMode = FADE_IN;
				SceneManager::TriggerFaderAction();
			}
		} break;

		default: break;
	}
}

void Fader::DrawGUI()
{
	GameManager::driver->draw2DRectangle(SColor(alpha * 255.0f, 0, 0, 0), rect<s32>(0, 0,
		elementWidth, elementHeight));
}
