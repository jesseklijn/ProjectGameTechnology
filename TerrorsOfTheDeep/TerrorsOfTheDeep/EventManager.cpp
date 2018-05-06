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
	// Reset all key states
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		KeyIsDown[i] = false;
		KeyIsPressed[i] = false;
		KeyIsReleased[i] = false;
	}
}

void EventManager::Update()
{
	// Reset just the pressed and released states
	for (u32 i = 0; i < KEY_KEY_CODES_COUNT; ++i)
	{
		KeyIsPressed[i] = false;
		KeyIsReleased[i] = false;
	}
}

// This is the one method that we have to implement
bool EventManager::OnEvent(const SEvent& event)
{
	switch (event.EventType)
	{
		// Remember whether each key is down or up
		case irr::EET_KEY_INPUT_EVENT:
		{
			irr::EKEY_CODE targetKey = event.KeyInput.Key;
			bool targetKeyState = event.KeyInput.PressedDown;

			// Apply key states
			KeyIsDown[targetKey] = targetKeyState;
			if (targetKeyState)
			{
				if (!KeyIsPressed[targetKey])
					KeyIsPressed[targetKey] = true;
			}
			else
			{
				if (!KeyIsReleased[targetKey])
					KeyIsReleased[targetKey] = true;
			}			
		} break;

		// Remember the mouse state
		case irr::EET_MOUSE_INPUT_EVENT:
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
		} break;

		default: break;
	}

	return false;
}

// This is used to check whether a key is being held down
bool EventManager::IsKeyDown(EKEY_CODE keyCode)
{
	return KeyIsDown[keyCode];
}

// This is used to check whether a key was pressed this frame
bool EventManager::IsKeyPressed(EKEY_CODE keyCode)
{
	return KeyIsPressed[keyCode];
}

// This is used to check whether a key was released this frame
bool EventManager::IsKeyReleased(EKEY_CODE keyCode)
{
	return KeyIsReleased[keyCode];
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
