#pragma once
#include <vector>
#include "Tile.h"

class Grid
{
public:
	// Constructor and destructor
	Grid();
	~Grid();

	// Variables
	int xWidth, zLength;
	Tile **grid;

	// Functions
	virtual void AssignSize(int xSize, int zSize);
	virtual bool Generate();
	virtual void Delete();
};