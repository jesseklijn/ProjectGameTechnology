#include "Sound.h"
#include "Camera.h"
#include <Windows.h>
#include<conio.h>
#include<dos.h>


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


//testSoundFunction
bool doOnce = false;
float posOnCircle = 0;
const float radius = 5;

void Sound::DropAllSounds()
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

void Sound::StopSounds()
{
	if (backGroundMusic)
	{
		backGroundMusic->drop();
	}

	if (soundEffect)
	{
		soundEffect->drop();
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

void Sound::TestSound3D()
{
	if (!doOnce)
	{
		soundEffect = SoundEngine::_SoundEngine()->play3D(/*"../media/AmbientUnderwaterMaddnes.ogg"*/"../media/ophelia.ogg",
		irrklang::vec3df(0, 0, 0), true, false, true);

		if (soundEffect)
		{
			soundEffect->setMinDistance(5.0f);
		}			
	}
	doOnce = true;
	

	//add this while loop to the fixed update so t4he position is tracked at all times
	/*while(true)
	{*/
		posOnCircle += 0.04f;
		irrklang::vec3df pos3d(radius * cosf(posOnCircle), 0,
			radius * sinf(posOnCircle * 0.5f));

		SoundEngine::_SoundEngine()->setListenerPosition(irrklang::vec3df(0, 0, 0), irrklang::vec3df(0, 0, 1));

		if (soundEffect)
			soundEffect->setPosition(pos3d);

		//if (_kbhit())
		//{
		//	int key = _getch();
		//	if (key)
		//		break; // user pressed ESCAPE key
		//}
	//}
}


void Sound::SoundEffect3D(const char* soundFile, irr::core::vector3df sourcePos, const float volume)
{
	soundEffect = SoundEngine::_SoundEngine()->play3D(soundFile, irr::core::vector3df(0, 0, 0), false, false, true);

	if (soundEffect)
	{
		soundEffect->setMinDistance(5.0f);
	}

	//set listener direction
	SoundEngine::_SoundEngine()->setListenerPosition(
		GameManager::smgr->getActiveCamera()->getAbsolutePosition(), 
		GameManager::smgr->getActiveCamera()->getTarget() - GameManager::smgr->getActiveCamera()->getAbsolutePosition());

	//get the source position
	if (soundEffect)
	{
		soundEffect->setPosition(sourcePos);
	}	

	//if (soundEffect)
	//{
	//	soundEffect->setVolume(volume);
	//}
}

#else

void Sound::DropAllSounds();
void Sound::BackGroundMusic2D(const char * soundFile, const irrklang::ik_f32 volume);
void Sound::SoundEffect2D(const char* soundFile, const float volume)

#endif

//if (GetKeyState('B') & 0x8000/*check if high-order bit is set (1 << 15)*/)
//{
//	Shark* shark = (Shark*)GameManager::FindGameObjectWithTag<GameObject>(GameObject::MONSTER, GameManager::gameObjects);
//
//	Sound::SoundEffect3D("../media/evilsatanlaugh.ogg", shark->getAbsolutePosition(), 0.5);
//	//Sound::SoundEffect2D("../media/evilsatanlaugh.ogg", 0.5);
//}