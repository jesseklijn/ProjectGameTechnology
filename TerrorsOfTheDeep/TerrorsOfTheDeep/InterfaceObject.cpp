#include "InterfaceObject.h"
#include "GameManager.h" 


InterfaceObject::InterfaceObject(irr::core::vector2df* startPosition,
	irr::core::vector2df* startScale,
	irr::core::vector2df* startRotation,
	irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id)
	: ISceneNode(parent, mgr, id)
{
	tag = INTERFACE_ELEMENT;

	setPosition(vector3df(startPosition->X, startPosition->Y, 0.0f));
	setScale(vector3df(startScale->X, startScale->Y, 0.0f));
	setRotation(vector3df(startRotation->X, startRotation->Y, 0.0f));
}

InterfaceObject::~InterfaceObject()
{

}

void InterfaceObject::Update()
{

}

void InterfaceObject::Draw()
{

}

void InterfaceObject::DrawGUI()
{
}

InterfaceObject::Tag InterfaceObject::GetTag()
{
	return tag;
}

void InterfaceObject::setTag(InterfaceObject::Tag tagPar)
{
	tag = tagPar;
}
