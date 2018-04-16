#include "GameObject.h"
#include <vector>
#include <iostream>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif
using namespace std;


class Fish :
	public GameObject
{
public:
	Fish(const irr::core::vector3df* startPosition,
		const irr::core::vector3df* startScale,
		const irr::core::vector3df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id,
		irr::scene::IAnimatedMesh* relatedMesh = 0, irr::video::ITexture* relatedTexture = 0);
	~Fish();

	virtual void Update();

	irr::core::vector3df velocity = irr::core::vector3df(0, 0, 0);
	int fishSize = 25;
	int fishSizeOffset;

};

