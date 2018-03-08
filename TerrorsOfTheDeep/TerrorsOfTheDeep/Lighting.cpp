#include "Lighting.h"
#include "irrlicht.h"

using namespace irr;
using namespace scene;

class Lighting 
{
	ISceneManager* smgr;
public:
	void setSceneLight(float r,float g, float b, float a);
	void createPointLight();
	void createDirectionalLight();
	void createSpotLight();

	Lighting(ISceneManager* tempSmgr);
};

Lighting::Lighting (ISceneManager* tempSmgr)
{
	smgr = tempSmgr;
}

void Lighting::setSceneLight(float r,float g,float b, float a)
{
	smgr->setAmbientLight(video::SColorf(r, g, b, a));
}

void Lighting::createPointLight()
{

}

void Lighting::createDirectionalLight()
{

}

void Lighting::createSpotLight()
{

}
