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
		int fps = driver->getFPS();
		core::stringw str = L"HUD - Test [";
		str += driver->getName();
		str += "] Stamina = ";
		str += stamina;
		str += " frames = ";
		str += fps;

		guienv->addStaticText(str.c_str(),
			rect<s32>(10, 10, 520, 22), true);
		//driver->draw2DImage(driver->getTexture("../media/smoke.bmp"), core::position2d<s32>(10, 20), rect<s32>(10, 10, 50, 50));
		for (size_t i = 0; i < sizeof(itemPickedUpArray); i++)
		{
			if (itemPickedUpArray[i])
			{
				switch (i)
				{
				case 0:
					driver->draw2DImage(driver->getTexture("../media/axe.jpg"), core::position2d<s32>(10, 30), rect<s32>(10, 10, 50, 50));
					break;
				case 1:
					driver->draw2DImage(driver->getTexture("../media/smoke.bmp"), core::position2d<s32>(70, 30), rect<s32>(10, 10, 50, 50));
					break;
				case 2:
					driver->draw2DImage(driver->getTexture("../media/portal1.bmp"), core::position2d<s32>(130, 30), rect<s32>(10, 10, 50, 50));
					break;
				default:
					break;
				}
			}
		}
	}
