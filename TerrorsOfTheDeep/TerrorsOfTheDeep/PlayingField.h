#pragma once
#include "Grid.h"
class PlayingField :
	public Grid
{
public:
	PlayingField();
	~PlayingField();
	bool Generate();
	void AssignSize(int xSize, int ySize, int zSize);
};

