#pragma once
#include <irrlicht.h>
#include "GameManager.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Class that initializes a main camera.
class Camera
{
public:
	explicit Camera(irr::scene::ISceneManager* smgr);	
	~Camera();

	static void updatePos();
};
