#include "class_declarations.h"

#ifndef AUDIO_H
#define AUDIO_H

ENGINE_BEGIN

class Audio
{
private:
	Audio();
	Audio(Audio const& a);
	Audio operator =(Audio const& a);
public:
	static Audio* GetInstance();
	void InitAudio();
};

ENGINE_END

#endif