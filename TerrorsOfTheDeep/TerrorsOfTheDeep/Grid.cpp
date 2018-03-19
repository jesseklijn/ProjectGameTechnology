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
	if (xWidth == NULL || zLength == NULL) {
		//If dimensions have not been declared, return false
		return false;
	}

	//Initialize the size of the tiles
	grid.resize(xWidth);
	for (size_t x = 0; x < grid.size(); x++)
	{
		grid[x].resize(yHeight);
		for (size_t y = 0; y < grid[x].size(); y++)
		{
			grid[x][y].resize(zLength);
			//For the z positions of the tiles
			for (size_t z = 0; z < zLength; z++)
			{
				grid[x][y][z].xPos = x;
				grid[x][y][z].yPos = y;
				grid[x][y][z].zPos = z;
			}
		}

	}

	//Iterate to assign positions to tiles of the grid

	//Assign to the x position of the tiles
//	for (size_t x = 0; x < xWidth; x++)
//	{
//		//For y 
//		for (size_t y = 0; y < yHeight; y++)
//		{
//			
//				
//			
//			}
//		}
//	}
//
//	//If everything has been assigned with success 
//	return true;
//
}
