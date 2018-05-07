#include "SoundEngine.h"

irrklang::ISoundEngine* SoundEngine::pSoundEngine = 0;

//return soundEngine and create it if it doesn't yet exist
irrklang::ISoundEngine* SoundEngine::_SoundEngine()
{
	if (SoundEngine::pSoundEngine == 0)
	{
		SoundEngine::pSoundEngine =irrklang::createIrrKlangDevice();
	}
	return SoundEngine::pSoundEngine;
}


//drop sound engine
void SoundEngine::DropSoundEngine()
{
	SoundEngine::pSoundEngine->drop();
}