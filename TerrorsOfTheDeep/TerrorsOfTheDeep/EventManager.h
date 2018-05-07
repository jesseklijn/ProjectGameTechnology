#pragma once
#include <irrlicht.h>

#ifdef _MSC_VER
#pragma comment(lib, "Irrlicht.lib")
#endif

using namespace irr;

class EventManager : public IEventReceiver
{
public:
	EventManager();

	// We'll create a struct to record info on the mouse state
	static struct SMouseState
	{
		core::position2di Position;
		bool LeftButtonDown;
		SMouseState() : LeftButtonDown(false) { }
	} MouseState;

	void Update();

	// This is the one method that we have to implement
	virtual bool OnEvent(const SEvent& event);

	// This is used to check whether a key is being held down
	bool IsKeyDown(EKEY_CODE keyCode);
	bool IsKeyPressed(EKEY_CODE keyCode);
	bool IsKeyReleased(EKEY_CODE keyCode);

	const SMouseState & GetMouseState(void) const;

private:
	// We use this array to store the current state of each key
	bool KeyIsDown[KEY_KEY_CODES_COUNT];
	bool KeyIsPressed[KEY_KEY_CODES_COUNT];
	bool KeyIsReleased[KEY_KEY_CODES_COUNT];
};
