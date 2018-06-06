#pragma once
#include "Fader.h"
#include "GameManager.h";
#include "SceneManager.h"


Fader::Fader(irr::core::vector2df * startPosition, irr::core::vector2df * startScale, irr::core::vector2df * startRotation, 
	irr::scene::ISceneNode * parent, irr::scene::ISceneManager * mgr, irr::s32 id)
	: InterfaceObject(startPosition, startScale, startRotation, parent, mgr, id)
{
	std::cout << "Testtt";
}

Fader::~Fader()
{

}

void Fader::Update()
{
	std::cout << alpha << std::endl;
	if (isFading)
	{
		

		switch (faderMode)
		{
		case FADE_IN:
		{
			alpha = GameManager::Clamp(alpha - alphaIncrement * GameManager::deltaTime, 0.0f, 1.0f);
			if (alpha == 0.0f)
			{
				faderMode = FADE_OUT;
				isFading = false;
			}
		} break;

		case FADE_OUT:
		{
			alpha = GameManager::Clamp(alpha + alphaIncrement * GameManager::deltaTime, 0.0f, 1.0f);
			if (alpha == 1.0f)
			{
				faderMode = FADE_IN;
				SceneManager::TriggerFaderAction();
			}
		} break;

		default: break;
		}
	}
}

void Fader::DrawGUI()
{
	IGUIImage* faderOverlay = GameManager::guienv->addImage(GameManager::driver->getTexture("../media/UI/Background.png"), 
		core::position2d<s32>(0, 0), true, 0, -1);
	faderOverlay->setColor(SColor(alpha * 255.0f, 0, 0, 0));
}