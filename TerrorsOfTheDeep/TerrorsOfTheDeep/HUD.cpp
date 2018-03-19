#pragma once
#include "HUD.h"


HUD::HUD()
{
}


HUD::~HUD()
{
}

//draws the HUD in the scene
void HUD::HudDraw(int stamina, bool itemPickedUpArray[], IVideoDriver* driver, IGUIEnvironment* guienv)
	{
	//array for hud strings
	array<stringw> amountStrings;

	//adjustable variables
	float spacingText = 22;
	float spacingImage = 60;
	float hudItemCoordinateX = 800;
	float hudItemCoordinateY = 60;

	//creating the strings for 
	int fps = driver->getFPS();
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
			rect<s32>(10, 10 + (i * spacingText), 520, 22 + (i * spacingText)), true,true,0,-1,true);
	}

	//for getting the image on the right spot (prevents invisible spots to appear)
	int imageOrder = 0;
		for (size_t i = 0; i < sizeof(itemPickedUpArray); i++)
		{
			//check if the item (id) got picked up by the player
			if (itemPickedUpArray[i])
			{
				//draw the images on the hud
				switch (i)
				{
				case 0:
					driver->draw2DImage(driver->getTexture("../media/axe.jpg"), core::position2d<s32>(hudItemCoordinateX + imageOrder * spacingImage, hudItemCoordinateY), rect<s32>(0, 0, 50, 50));
					break;
				case 1:
					driver->draw2DImage(driver->getTexture("../media/smoke.bmp"), core::position2d<s32>(hudItemCoordinateX  + imageOrder * spacingImage, hudItemCoordinateY), rect<s32>(0, 0, 50, 50));
					break;
				case 2:
					driver->draw2DImage(driver->getTexture("../media/portal1.bmp"), core::position2d<s32>(hudItemCoordinateX + imageOrder * spacingImage, hudItemCoordinateY), rect<s32>(0, 0, 50, 50));
					break;
				default:
					break;
				}
				imageOrder++;
			}
		}
	}
