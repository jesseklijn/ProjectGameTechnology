#include "Sound.h"

#define USE_SOUND
#ifdef USE_SOUND

Sound::Sound()
{
}

Sound::~Sound()
{
}	

irrklang::ISound* Sound::backGroundMusic = 0;

void Sound::SoundShutdown()
{
	if (backGroundMusic)
	{
		backGroundMusic->drop();
	}		

	if (SoundEngine::_SoundEngine())
	{
		SoundEngine::DropSoundEngine();
	}
}


void Sound::BackGroundMusic(const char* soundFile, const float volume)
{
	if (backGroundMusic)
	{
		backGroundMusic->stop();
		backGroundMusic->drop();
	}

	backGroundMusic = SoundEngine::_SoundEngine()->play2D(soundFile, true, false, true);

	if (backGroundMusic)
	{
		backGroundMusic->setVolume(volume);
	}
}

#else

void Sound::SoundShutdown();
void Sound::BackGroundMusic(const char * soundFile, const irrklang::ik_f32 volume);

#endif