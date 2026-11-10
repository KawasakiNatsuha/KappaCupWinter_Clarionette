#pragma once
#include "sound/SoundSource.h"

class Sound :public IGameObject
{
public:
	Sound();
	void PlayBGM(int number);
	void PlaySE(int number);
private:
};

