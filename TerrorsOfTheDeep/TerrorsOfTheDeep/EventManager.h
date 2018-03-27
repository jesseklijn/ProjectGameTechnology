#pragma once
#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;

class EventManager : public IEventReceiver
{
private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];

public:
	EventManager();

	// We'll create a struct to record info on the mouse state
	struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(false) { }
	} MouseState;

	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event);

	// This is used to check whether a key is being held down
	virtual bool IsKeyDown(EKEY_CODE keyCode) const;

	const SMouseState & GetMouseState(void) const;
};
