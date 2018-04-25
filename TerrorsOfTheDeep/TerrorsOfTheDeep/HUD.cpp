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
void HUD::HudDraw(int stamina, IVideoDriver* driver, IGUIEnvironment* guienv)
{
	const irr::core::dimension2du& screenSize = driver->getScreenSize();
	//array for hud strings
	array<stringw> amountStrings;


	//creating the strings for 
	auto fps = driver->getFPS();
	stringw str = L"HUD - Test [";
	str += driver->getName();
	str += "] Stamina = ";
	str += stamina;
	str += " frames = ";
	str += fps;

	stringw str2 = "this is string 2";

	//add the strings to the array
	amountStrings.push_back(str);
	amountStrings.push_back(str2);
	amountStrings.push_back(str);

	//put string in the hud
	for (size_t i = 0; i < amountStrings.size(); i++)
	{
		guienv->addStaticText(amountStrings[i].c_str(),
			rect<s32>(10, 10 + (i * spacingText), 520, 22 + (i * spacingText)), true, true, 0, -1, true);
	}

	//Conditions for HUD to show
	if (GameManager::keyPickedUp)
	{
		//Key
		driver->draw2DImage(driver->getTexture("../media/Key.png"), core::position2d<s32>(screenSize.Width-135,0), rect<s32>(0, 0,125,125));
	}
	if (GameManager::escaped) {
		//Escape
		driver->draw2DImage(driver->getTexture("../media/Background.png"), core::position2d<s32>(0, 0), rect<s32>(0, 0, 1920, 1080));
		driver->draw2DImage(driver->getTexture("../media/Youwin.png"), core::position2d<s32>((screenSize.Width - 550) / 2, (screenSize.Height - 314) / 2), rect<s32>(0, 0, 550, 314));

	}
	if (GameManager::hasDied) {
		//Death
		driver->draw2DImage(driver->getTexture("../media/Background.png"), core::position2d<s32>(0, 0), rect<s32>(0, 0, 1920, 1080));
		driver->draw2DImage(driver->getTexture("../media/Overlay.png"), core::position2d<s32>((screenSize.Width - 275) / 2, (screenSize.Height - 183) / 2), rect<s32>(0, 0, 275, 183));
	}
}
