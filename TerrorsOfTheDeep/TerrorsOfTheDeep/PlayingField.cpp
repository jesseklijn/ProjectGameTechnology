
#include "PlayingField.h"


PlayingField::PlayingField()
{

}


PlayingField::~PlayingField()
{

}

bool PlayingField::Generate()
{
	//Generate grid
	if (Grid::Generate() == true)
	{
		//Grid has been generated with success
		return true;
	}
	else 
	{
		//Grid has not been generated
		return false;
	};

	
}

void PlayingField::AssignSize(int xSize, int ySize, int zSize)
{
	Grid::AssignSize(xSize, ySize, zSize);
}
