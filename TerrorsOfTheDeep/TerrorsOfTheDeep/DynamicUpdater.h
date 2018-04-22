#pragma once
#include <string>

/* Superclass defining standard loops and a base tag

NOTE: This is an abstract class, cannot instantiate.
Children of an abstract class can be instantiated.*/
class DynamicUpdater
{
public:
	// Constructor and destructor
	DynamicUpdater();
	virtual ~DynamicUpdater();

	enum Tag
	{
		NONE,
		PLAYER,
		MONSTER,
		CREATURE,
		WORLD_OBJECT,
		KEY,
		CHEST
	};

	/* Pure virtual functions

	Pure virtual is defined by the "= 0;" and makes it an abstract class.
	It means that any child or children of children etc. all have these
	functions they should initialize and use.
	
	A little different from normal virtuals, because now we can make sure
	any child's function (below) can be called without needing a class
	specification, which is what makes our GameManager and this system tick 
	(since we can add different Class types to our GameManager::gameObjects
	list). Otherwise we'd have to use seperate lists per class in GameManager 
	to do this.*/
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual Tag GetTag() = 0;
};

