//All Includes
#include <irrlicht.h>

#pragma once
#include "GameManager.h"
#pragma once
#include "Monster.h"

//Main namespace
using namespace irr;

//Namespaces of Irrlicht
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
using namespace std;

/*
This is the main method. We can now use main() on every platform.
*/
int main()
{
	//Add the Game Manager here
	GameManager gameManager;
	Monster monster;

	/*
	We add a hello world label to the window, using the GUI environment.
	The text is placed at the position (10,10) as top left corner and
	(260,22) as lower right corner.
	*/
	GameManager::guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
		rect<s32>(10,10,260,22), true);

	/*
	To show something interesting, we load a Quake 2 model and display it.
	We only have to get the Mesh from the Scene Manager with getMesh() and add
	a SceneNode to display the mesh with addAnimatedMeshSceneNode(). We
	check the return value of getMesh() to become aware of loading problems
	and other errors.

	Instead of writing the filename sydney.md2, it would also be possible
	to load a Maya object file (.obj), a complete Quake3 map (.bsp) or any
	other supported file format. By the way, that cool Quake 2 model
	called sydney was modelled by Brian Collins.
	*/
	IAnimatedMesh* mesh = GameManager::smgr->getMesh("../media/sydney.md2");
	if (!mesh)
	{
		GameManager::device->drop();
		return 1;
	}
	IAnimatedMeshSceneNode* node = GameManager::smgr->addAnimatedMeshSceneNode( mesh );

	/*
	To let the mesh look a little bit nicer, we change its material. We
	disable lighting because we do not have a dynamic light in here, and
	the mesh would be totally black otherwise. Then we set the frame loop,
	such that the predefined STAND animation is used. And last, we apply a
	texture to the mesh. Without it the mesh would be drawn using only a
	color.
	*/
	if (node)
	{
		node->setMaterialFlag(EMF_LIGHTING, false);
		node->setMD2Animation(scene::EMAT_STAND);
		node->setMaterialTexture( 0, GameManager::driver->getTexture("../media/sydney.bmp") );
	}

	//Start the awake of Game Manager
	gameManager.Awake();
	//Start the start method of Game manager
	gameManager.Start();

	//Update the window
	while(GameManager::device->run())
	{
		/*
		Anything can be drawn between a beginScene() and an endScene()
		call. The beginScene() call clears the screen with a color and
		the depth buffer, if desired. Then we let the Scene Manager and
		the GUI Environment draw their content. With the endScene()
		call everything is presented on the screen.
		*/
		GameManager::driver->beginScene(true, true, SColor(255,100,101,140));

		gameManager.Update();
		gameManager.Draw();

		GameManager::smgr->drawAll();
		GameManager::guienv->drawAll();

		GameManager::driver->endScene();
	}

	/*
	After we are done with the render loop, we have to delete the Irrlicht
	Device created before with createDevice(). In the Irrlicht Engine, you
	have to delete all objects you created with a method or function which
	starts with 'create'. The object is simply deleted by calling ->drop().
	See the documentation at irr::IReferenceCounted::drop() for more
	information.
	*/
	GameManager::device->drop();

	return 0;
}

/*
That's it. Compile and run.
**/
