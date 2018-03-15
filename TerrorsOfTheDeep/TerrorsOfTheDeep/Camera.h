#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

class Camera
{
public:
	Camera(irr::scene::ISceneManager* smgr);
	~Camera();
};
