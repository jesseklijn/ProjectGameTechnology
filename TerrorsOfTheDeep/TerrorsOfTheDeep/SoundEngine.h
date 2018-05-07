#pragma once
#include <irrlicht.h>
#pragma once
#include <irrKlang.h>

//A single instance of the soundEngine created
//according to the singleton design pattern
class SoundEngine {
	public:
	static irrklang::ISoundEngine* _SoundEngine();
	static void DropSoundEngine();

	private:
	static irrklang::ISoundEngine* pSoundEngine;
};