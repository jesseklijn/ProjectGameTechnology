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
#pragma once
#include <irrlicht.h>
#include <stdio.h>
#include "sound.h"

#define USE_IRRKLANG
#ifdef USE_IRRKLANG
#include <irrKlang.h>

using namespace irr;

void sound_init();
void sound_shutdown();
void background_music(const char * file);
void SetSoundVolume(irrklang::ISound* sound, float volume);
irrklang::ISound* GetBackgroundSound();

#else

void sound_init(IrrlichtDevice *device) {}
void sound_shutdown() {}
void background_music(const c8 * file) {}

#endif