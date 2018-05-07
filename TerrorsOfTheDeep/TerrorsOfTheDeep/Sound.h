#pragma once
#include <irrlicht.h>
#pragma once
#include <irrKlang.h>
#include "SoundEngine.h"

using namespace irr;

// Class responsible for managing Sounds.
class Sound
{
public:
	Sound();
	~Sound();
	static void SoundShutdown();
	static void BackGroundMusic(
		const char* soundFile,
		float volume);
private:
	static irrklang::ISound* backGroundMusic;
};