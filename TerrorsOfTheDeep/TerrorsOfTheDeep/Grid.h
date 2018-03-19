#pragma once
#include <vector>
#include "Tile.h"
#include <iostream>

using namespace std;

class Grid
{
public:
	// Constructor and destructor
	Grid();
	~Grid();
	int xWidth, yHeight, zLength;
	vector<vector<vector<Tile>>> grid;
	virtual void AssignSize(int xSize, int ySize, int zSize);
	virtual bool Generate();

};
