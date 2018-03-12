#include "sound.h"
#include <irrlicht.h>

using namespace irr;

using namespace core;
using namespace scene;
using namespace video;

#ifdef _IRR_WINDOWS_
#pragma comment(lib, "Irrlicht.lib")
#pragma comment(linker, "/subsystem:windows /ENTRY:mainCRTStartup")
#endif

int main()
{

	IrrlichtDevice *device =
		createDevice( video::EDT_SOFTWARE, dimension2d<u32>(640, 480), 16,
			false, false, false, 0);

	if (!device)
		return 1;

	sound_init();
	background_music("../media/JawsTheme.ogg");

	
	while(device->run())
	{
		
	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	device->drop();
	sound_shutdown();

	return 0;
}

/*
That's it. Compile and run.
**/
