#include "EventManager.h"

#pragma region Namespaces
using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;
#pragma endregion

EventManager::EventManager()
{
	for (u32 i = 0; i<KEY_KEY_CODES_COUNT; ++i)
		KeyIsDown[i] = false;
}

// This is the one method that we have to implement
bool EventManager::OnEvent(const SEvent& event)
{
	// Remember whether each key is down or up
	if (event.EventType == irr::EET_KEY_INPUT_EVENT) 
	{
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;
	}

	// Remember the mouse state
	if (event.EventType == irr::EET_MOUSE_INPUT_EVENT)
	{
		switch (event.MouseInput.Event)
		{
		case EMIE_LMOUSE_PRESSED_DOWN:
			MouseState.LeftButtonDown = true;
			break;

		case EMIE_LMOUSE_LEFT_UP:
			MouseState.LeftButtonDown = false;
			break;

		case EMIE_MOUSE_MOVED:
			MouseState.Position.X = event.MouseInput.X;
			MouseState.Position.Y = event.MouseInput.Y;
			break;

		default:
			// We won't use the wheel
			break;
		}
	}

	return false;
}

// This is used to check whether a key is being held down
bool EventManager::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

const EventManager::SMouseState & EventManager::GetMouseState(void) const
{
	return MouseState;
}

// To handle events:
//
// Check if key is down:
// if (gameManager.receiver.IsKeyDown(irr::KEY_KEY_P))
//
// Check if mouse button is down:
// if (gameManager.receiver.GetMouseState().LeftButtonDown)
//
// Get mouse position:
// gameManager.receiver.GetMouseState().Position.X
