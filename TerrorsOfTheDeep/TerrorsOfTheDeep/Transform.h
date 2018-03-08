#pragma once
#include <irrlicht.h>
using namespace irr;
using namespace core;

class Transform
{
public:
	Transform();
	~Transform();
	vector3df position;
	vector3df scale;
	vector3df rotation;

};

