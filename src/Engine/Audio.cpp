#include "engine.h"
#include "irrKlang/irrKlang.h"
#pragma comment(lib, "irrKlang.lib")
using namespace irrklang;

ENGINE_BEGIN

ISoundEngine* eng = NULL;

Audio::Audio()
{
}

Audio* Audio::GetInstance()
{
	static Audio a;
	return &a;
}

void Audio::InitAudio()
{
#ifdef AUDIO
	eng = createIrrKlangDevice();
#endif
	if (!eng)
		printf("Cannot initialize audio engine\n");
}

Sound::Sound(std::string filename) : Resource(RESOURCE_SOUND, filename)
{
	data = NULL;
#ifdef AUDIO
	data = (void*)eng->addSoundSourceFromFile(filename.c_str());
#endif
}

void Sound::Play()
{
#ifdef AUDIO
	eng->play2D((ISoundSource*)data);
#endif
}

int Sound::GetLength()
{
#ifdef AUDIO
	return ((ISoundSource*)data)->getPlayLength();
#else
	return 0;
#endif
}

ENGINE_END
