#pragma once
#include "HUD.h"
#include "GameManager.h"


HUD::HUD()
{
}


HUD::~HUD()
{
}

//draws the HUD in the scene
void HUD::HudDraw(IVideoDriver* driver, IGUIEnvironment* guienv)
{
	if (!hudIsVisible)
		return;

	//array for hud strings
	array<stringw> amountStrings;


	//creating the strings for 
	auto fps = driver->getFPS();
	stringw str = "Driver: [";
	str += driver->getName();
	str += "]";
	stringw str2 = "FPS: [";
	str2 += fps;
	str2 += "]";

	//add the strings to the array
	amountStrings.push_back(str);
	amountStrings.push_back(str2);

	//put string in the hud
	for (size_t i = 0; i < amountStrings.size(); i++)
	{
		IGUIStaticText* guiText = guienv->addStaticText(amountStrings[i].c_str(),
			rect<s32>(10, 10 + (i * spacingText), 520, 42 + (i * spacingText)), true, true, 0, -1, false);	
		guiText->setOverrideColor(SColor(255, 255, 255, 255));
	}

	//Conditions for HUD to show
	if (GameManager::keyPickedUp)
	{
		//Key
		driver->draw2DImage(driver->getTexture("../media/WinLose/Key.png"), core::position2d<s32>(GameManager::screenDimensions.Width-135,0), rect<s32>(0, 0,125,125));
	}
	if (GameManager::escaped) {
		//Escape
		driver->draw2DImage(driver->getTexture("../media/UI/Background.png"), core::position2d<s32>(0, 0), rect<s32>(0, 0, 1920, 1080));
		driver->draw2DImage(driver->getTexture("../media/UI/Youwin.png"), core::position2d<s32>((GameManager::screenDimensions.Width - 550) / 2, (GameManager::screenDimensions.Height - 314) / 2), rect<s32>(0, 0, 550, 314));

	}
	if (GameManager::hasDied) {
		//Death
		driver->draw2DImage(driver->getTexture("../media/UI/Background.png"), core::position2d<s32>(0, 0), rect<s32>(0, 0, 1920, 1080));
		driver->draw2DImage(driver->getTexture("../media/UI/Overlay.png"), core::position2d<s32>((GameManager::screenDimensions.Width - 275) / 2, (GameManager::screenDimensions.Height - 183) / 2), rect<s32>(0, 0, 275, 183));
	}
}
