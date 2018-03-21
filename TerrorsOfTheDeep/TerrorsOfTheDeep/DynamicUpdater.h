#pragma once
class DynamicUpdater
{
public:
	DynamicUpdater();
	virtual ~DynamicUpdater();

	virtual void Update() = 0;
	virtual void Draw() = 0;
};

