#pragma once
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
		KeyIsDown[event.KeyInput.Key] = event.KeyInput.PressedDown;

	return false;
}

// This is used to check whether a key is being held down
bool EventManager::IsKeyDown(EKEY_CODE keyCode) const
{
	return KeyIsDown[keyCode];
}

// To handle events:
//
// if (gameManager.receiver.IsKeyDown(irr::KEY_KEY_P)) {
//	*code you want to happen*
//}
