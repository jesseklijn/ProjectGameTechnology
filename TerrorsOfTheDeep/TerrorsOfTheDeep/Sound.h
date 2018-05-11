// New Sound Header
//#pragma once
//#include <irrlicht.h>
//#pragma once
//#include <irrKlang.h>
//#include "SoundEngine.h"
//
//using namespace irr;
//
//// Class responsible for managing Sounds.
//class Sound
//{
//public:
//	Sound();
//	~Sound();
//	static void SoundShutdown();
//	static void BackGroundMusic(
//		const char* soundFile,
//		float volume);
//private:
//	static irrklang::ISound* backGroundMusic;
//};

// Old Sound Header
#include <irrlicht.h>
#include <stdio.h>

using namespace irr;

void sound_init();
void sound_shutdown();
void background_music(const char * file);

