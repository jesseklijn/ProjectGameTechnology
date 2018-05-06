#include "Grid.h"



Grid::Grid()
{

}


Grid::~Grid()
{
}

void Grid::AssignSize(int xSize, int ySize, int zSize)
{
	//Assign dimensions to the grid
	xWidth = xSize, yHeight = ySize, zLength = zSize;
}

bool Grid::Generate()
{
	return false;
}
