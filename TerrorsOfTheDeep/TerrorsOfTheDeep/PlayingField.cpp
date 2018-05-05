
#include "PlayingField.h"


PlayingField::PlayingField()
{

}


PlayingField::~PlayingField()
{

}

bool PlayingField::Generate()
{
	return Grid::Generate();
}

void PlayingField::AssignSize(int xSize, int ySize, int zSize)
{
	Grid::AssignSize(xSize, ySize, zSize);
}
