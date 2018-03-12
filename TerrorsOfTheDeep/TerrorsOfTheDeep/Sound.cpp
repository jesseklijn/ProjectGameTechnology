#include "sound.h"

#define USE_IRRKLANG

#ifdef USE_IRRKLANG

#include <irrKlang.h>

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

	if (backMusic)
	{
		backMusic->stop();
		backMusic->drop();
	}

	backMusic = engine->play2D(file, true, false, true);

	if (backMusic)
	{
		backMusic->setVolume(0.5f);
	}
}

void sound_shutdown()
{
	if (backMusic)
		backMusic->drop();

	if (engine)
		engine->drop();
}

#else

void sound_init(IrrlichtDevice *device) {}
void sound_shutdown() {}
void background_music(const c8 * file) {}

#endif