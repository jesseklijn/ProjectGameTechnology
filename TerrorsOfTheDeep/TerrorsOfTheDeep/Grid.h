#pragma once
#include <vector>
#include "Tile.h"

class Grid
{
public:
	Grid();
	~Grid();
	int xWidth, zLength;
	Tile **grid;
	virtual void AssignSize(int xSize, int zSize);
	virtual bool Generate();
	virtual void Delete();

};

