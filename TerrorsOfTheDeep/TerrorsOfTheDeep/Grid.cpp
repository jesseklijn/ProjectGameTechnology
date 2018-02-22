#include "Grid.h"



Grid::Grid()
{
	
}


Grid::~Grid()
{
}

void Grid::AssignSize(int xSize, int zSize)
{
	//Assign dimensions to the grid
	xWidth = xSize, zLength = zSize;
}

bool Grid::Generate()
{
	if(xWidth == NULL || zLength == NULL){
		//If dimensions have not been declared, return false
		return false;
	}

	//Initialize the size of the tiles
	grid = new Tile*[xWidth];
	for (int i = 0; i < zLength; ++i)
		grid[i] = new Tile[xWidth];

	//Iterate to assign positions to tiles of the grid

	//Assign to the x position of the tiles
	for (size_t x = 0; x < xWidth; x++)
	{
		//For the z positions of the tiles
		for (size_t z = 0; z < zLength; z++)
		{
			grid[x][z].xPos = x;
			grid[x][z].zPos = z;
		}
	}

	//If everything has been assigned with success 
	return true;

}

void Grid::Delete()
{
	//Iterates through the grid, deletes all elements to clean up memory
	//Call this method after you are done with tiles.
	for (int i = 0; i < xWidth; ++i)
		delete[] grid[i];
	delete[] grid;
}
