// New Sound Class, causes nullptr exception
//#include "Sound.h"
//
//#define USE_SOUND
//#ifdef USE_SOUND
//
//Sound::Sound()
//{
//}
//
//Sound::~Sound()
//{
//}	
//
//irrklang::ISound* Sound::backGroundMusic = 0;
//
//void Sound::SoundShutdown()
//{
//	if (backGroundMusic)
//	{
//		backGroundMusic->drop();
//	}		
//
//	if (SoundEngine::_SoundEngine())
//	{
//		SoundEngine::DropSoundEngine();
//	}
//}
//
//
//void Sound::BackGroundMusic(const char* soundFile, const float volume)
//{
//	if (backGroundMusic)
//	{
//		backGroundMusic->stop();
//		backGroundMusic->drop();
//	}
//
//	backGroundMusic = SoundEngine::_SoundEngine()->play2D(soundFile, true, false, true);
//
//	if (backGroundMusic)
//	{
//		backGroundMusic->setVolume(volume);
//	}
//}
//
//#else
//
//void Sound::SoundShutdown();
//void Sound::BackGroundMusic(const char * soundFile, const irrklang::ik_f32 volume);
//
//#endif

// Old Sound, works
#pragma once
#include "Sound.h"
#include "GameManager.h"


#pragma comment (lib, "irrKlang.lib")

irrklang::ISoundEngine *engine = 0;
irrklang::ISound *backMusic = 0;

void sound_init()
{
	engine = irrklang::createIrrKlangDevice();
	if (0 == engine)
	{
		return;
	}
}

void background_music(const char * file)
{
	if (0 == engine)
	{
		return;
	}

	if (backMusic != nullptr)
	{
		backMusic->stop();
		backMusic->drop();
		backMusic = nullptr;
	}

	backMusic = engine->play2D(file, true, false, true);

	if (backMusic != nullptr)
		SetSoundVolume(backMusic, 0.5f);
}

void SetSoundVolume(irrklang::ISound* sound, float volume)
{
	if (sound == nullptr)
		return;

	sound->setVolume(GameManager::Clamp(volume, 0.0f, 5.0f));
}

irrklang::ISound * GetBackgroundSound()
{
	return backMusic;
}

void sound_shutdown()
{
	if (backMusic != nullptr)
		backMusic->drop();

	if (engine)
		engine->drop();
}