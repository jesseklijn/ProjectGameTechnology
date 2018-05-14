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
irrklang::ISound* Sound::soundEffect = 0;

void Sound::SoundShutdown()
{
	if (backGroundMusic)
	{
		backGroundMusic->drop();
	}		

	if (soundEffect)
	{
		soundEffect->drop();
	}

	if (SoundEngine::_SoundEngine())
	{
		SoundEngine::DropSoundEngine();
	}
}



void Sound::BackGroundMusic2D(const char* soundFile, const float volume)
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

void Sound::BackGroundMusic3D(const char* soundFile, const float volume)
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

void Sound::SoundEffect2D(const char* soundFile, const float volume)
{
	soundEffect = SoundEngine::_SoundEngine()->play2D(soundFile, false, false, true);
	if (soundEffect)
	{
		soundEffect->setVolume(volume);
	}
}

void Sound::SoundEffect3D(const char* soundFile, const float volume, irr::core::vector3df sourcePos)
{
	irr::core::vector3df playerPos = irr::core::vector3df(0, 0, 0);/*get player pos*/
	soundEffect = SoundEngine::_SoundEngine()->play3D(soundFile, irr::core::vector3df(0, 0, 0), false, false, true);


	if (soundEffect)
	{
		soundEffect->setMinDistance(5.0f);
	}

	//????
	SoundEngine::_SoundEngine()->setListenerPosition(irr::core::vector3df(0, 0, 0), irr::core::vector3df(0, 0, 0));

	if (soundEffect)
	{
		soundEffect->setPosition(irr::core::vector3df(0, 0, 0)/*some position*/);
	}	

	if (soundEffect)
	{
		soundEffect->setVolume(volume);
	}
}

#else

void Sound::SoundShutdown();
void Sound::BackGroundMusic2D(const char * soundFile, const irrklang::ik_f32 volume);
void Sound::SoundEffect2D(const char* soundFile, const float volume)

#endif

//if (GetKeyState('B') & 0x8000/*check if high-order bit is set (1 << 15)*/)
//{
//	Sound::SoundEffect2D("../media/evilsatanlaugh.ogg", 0.5);
//}