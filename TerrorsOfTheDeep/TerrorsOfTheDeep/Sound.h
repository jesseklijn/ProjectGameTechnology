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
	static void DropAllSounds();
	static void StopSounds();
	static void BackGroundMusic2D(
		const char* soundFile,
		float volume);
	static void BackGroundMusic3D(
		const char* soundFile,
		float volume);
	static void SoundEffect2D(
		const char* soundFile,
		float volume);
	static void TestSound3D();
	static void SoundEffect3D(
		const char* soundFile,
		irr::core::vector3df sourcePos,
		float volume);	

	static irrklang::ISound* soundEffect;

private:
	static irrklang::ISound* backGroundMusic;		
};