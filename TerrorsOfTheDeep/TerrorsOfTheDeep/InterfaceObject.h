#pragma once
#include "irrlicht.h"
#include "DynamicUpdater.h"
#include <vector>

/* Defines an interface object, meant to display interface-level elements, such
as UI, buttons or other graphical representations. */
class InterfaceObject : public irr::scene::ISceneNode, public DynamicUpdater
{
public:
	InterfaceObject(irr::core::vector2df* startPosition,
		irr::core::vector2df* startScale,
		irr::core::vector2df* startRotation,
		irr::scene::ISceneNode* parent, irr::scene::ISceneManager* mgr, irr::s32 id);
	~InterfaceObject();

	int elementSpacing = 8;
	float elementWidth = 100.0f;
	float elementHeight = 100.0f;
	bool elementHidden = false;
	bool hasScreenBorder = false;
	float alpha = 1.0f;
	float alphaDisabled = 0.6f;
	irr::core::aabbox3d<irr::f32> Box;

	// All interface objects linked with this
	std::vector<InterfaceObject*> components;

	virtual void Update();
	virtual void Draw();
	virtual void DrawGUI() = 0;
	virtual void render() {};
	const irr::core::aabbox3d<irr::f32>& getBoundingBox() const override { return Box; };
	virtual Tag GetTag();
	virtual void setTag(Tag tagPar);

private:
	irr::core::vector2df* position;
	irr::gui::IGUIElement* test;
};

