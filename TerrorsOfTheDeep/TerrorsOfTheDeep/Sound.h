#pragma once
#include <irrlicht.h>
#pragma once
#include <irrKlang.h>
#include "SoundEngine.h"
#include "Player.h"

using namespace irr;

// Class responsible for managing Sounds.
class Sound
{
public:
	Sound();
	~Sound();
	static void SoundShutdown();
	static void BackGroundMusic2D(
		const char* soundFile,
		float volume);
	static void BackGroundMusic3D(
		const char* soundFile,
		float volume);
	static void SoundEffect2D(
		const char* soundFile,
		float volume);
	static void SoundEffect3D(
		const char* soundFile,
		float volume, irr::core::vector3df sourcePos);	

private:
	static irrklang::ISound* backGroundMusic;	
	static irrklang::ISound* soundEffect;
};