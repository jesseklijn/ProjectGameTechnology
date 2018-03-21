#pragma once
#include <irrlicht.h>
#pragma once
#include "GameManager.h"

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

// Class that initializes a main camera.
class Camera
{
public:
	Camera(irr::scene::ISceneManager* smgr);	
	~Camera();
	void updatePos();
};
