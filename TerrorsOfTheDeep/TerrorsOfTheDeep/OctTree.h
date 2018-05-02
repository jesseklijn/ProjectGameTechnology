#pragma once
#include "irrlicht.h"
#pragma once
#include "GameManager.h"
#include <queue>

class OctTree
{
private:
	OctTree(irr::core::aabbox3d<f32> box, std::vector<GameObject*> objs);
	void UpdateTree();
	void BuildTree();

public:
	OctTree();
	OctTree(irr::core::aabbox3d<f32> box);
	~OctTree();

	irr::core::aabbox3d<f32> oBox;
	std::vector<GameObject*> oObj;

	// queue of objects to insert into the Octtree.
	std::queue<GameObject*> *oInsert = new std::queue<GameObject*>;

	// child octants
	OctTree* tree[8];

	// indicates which child nodes are used. 
	uint8_t activeNodes = 0;

	// minimum size of a region
	const int MIN_SIZE = 5000;

	int maxLifeSpan = 8;
	int curLife = -1;

	// refrence to parent node
	OctTree* parent;
	bool treeReady = false;
	bool treeBuilt = false;	
};

