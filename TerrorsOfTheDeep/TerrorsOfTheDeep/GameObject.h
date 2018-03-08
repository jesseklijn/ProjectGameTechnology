#pragma once
#include "Transform.h"



class GameObject :
	public Transform
{
public:
	GameObject();
	~GameObject();
	virtual void Update();
	virtual void Draw();
};

