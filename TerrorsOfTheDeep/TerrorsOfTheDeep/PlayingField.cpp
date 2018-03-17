#include "PlayingField.h"
#include <irrlicht.h>

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;       
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

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

void PlayingField::AssignSize(int xSize, int zSize)
{
	Grid::AssignSize(xSize, zSize);
}
