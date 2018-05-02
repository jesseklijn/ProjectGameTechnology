/*
#include "OctTree.h"
#include "array"

OctTree::OctTree(irr::core::aabbox3d<f32> box, std::vector<GameObject*> objs)
{
	oBox = box;
	oObj = objs;
	curLife = -1;
}

OctTree::OctTree()
{
	oObj = *new std::vector<GameObject*>;
	oBox = *new irr::core::aabbox3d<f32>;
	curLife = -1;
}

OctTree::OctTree(irr::core::aabbox3d<f32> box)
{
	oBox = box;
	oObj = *new std::vector<GameObject*>;
	curLife = -1; 
}

void OctTree::UpdateTree()
{
	if(!treeBuilt)
	{
		while (oInsert->size() != 0)
			oObj.push_back(oInsert->front());
			BuildTree();
	}
	else
	{
		while (oInsert->size() != 0)
			oObj.push_back(oInsert->front());
	}
	treeReady = true;
}

void OctTree::BuildTree()
{
	std::cout << "creating Octtree";
	if (oObj.size() <= 1)
	{
		return;
	}
	vector3df dimensions = oBox.MaxEdge - oBox.MinEdge;
	if (dimensions.getLength() == 0.0f)
	{
		//FindEnclosingCube();
		dimensions = oBox.MaxEdge - oBox.MinEdge;
	}

	if (dimensions.X <= MIN_SIZE && dimensions.Z <= MIN_SIZE && dimensions.Y <= MIN_SIZE)
	{
		return;
	}

	vector3df half = dimensions / 2.0f;
	vector3df center = oBox.MinEdge + half;

	aabbox3d<f32>* octants = new aabbox3d<f32>[8];
	octants[0] = *new aabbox3d<f32>(oBox.MinEdge, center);
	octants[1] = *new aabbox3d<f32>(vector3df(center.X, oBox.MinEdge.Y, oBox.MinEdge.Z), vector3df(oBox.MaxEdge.X, center.Y, center.Z));
	octants[2] = *new aabbox3d<f32>(vector3df(center.X, oBox.MinEdge.Y, center.Z), vector3df(oBox.MaxEdge.X, center.Y, oBox.MaxEdge.Z));
	octants[3] = *new aabbox3d<f32>(vector3df(oBox.MinEdge.X, oBox.MinEdge.Y, center.Z), vector3df(center.X, center.Y, oBox.MaxEdge.Z));
	octants[4] = *new aabbox3d<f32>(vector3df(oBox.MinEdge.X, center.Y, oBox.MinEdge.Z), vector3df(center.X, oBox.MaxEdge.Z, center.Z));
	octants[5] = *new aabbox3d<f32>(vector3df(center.X, center.Y, oBox.MinEdge.Z), vector3df(oBox.MaxEdge.X, oBox.MaxEdge.Y, center.Z));
	octants[6] = *new aabbox3d<f32>(center, oBox.MaxEdge);
	octants[7] = *new aabbox3d<f32>(vector3df(oBox.MinEdge.X, center.Y, center.Z), vector3df(center.X, oBox.MaxEdge.Y, oBox.MaxEdge.Z));


	std::vector<std::vector<GameObject*>> list = *new std::vector<std::vector<GameObject*>>;
	
	for (int i = 0; i < 8; i++) list = new std::vector<GameObject*>;

	std::vector<GameObject*> delist = *new std::vector<GameObject*>;

	for(int k = 0; k < oObj.size(); k++)
	{
		if (oObj[k]->getBoundingBox().MinEdge != oObj[k]->getBoundingBox().MaxEdge)
		{
			for (int a = 0; a < 8; a++)
			{
				if (octants[a].intersectsWithBox(oObj[k]->getBoundingBox()))
				{
					list[a].push_back(oObj[a]);
					delist.push_back(oObj);
				}
			}
		}
	}


}

OctTree::~OctTree()
{

}
*/


