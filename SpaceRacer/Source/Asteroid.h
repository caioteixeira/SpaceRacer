#pragma once
#include "Actor.h"
#include "AudioComponent.h"

class Asteroid : public Actor		
{
	DECL_ACTOR(Asteroid, Actor);

public:
	Asteroid(Game & game);
	AudioComponentPtr GetAudio(){ return mAudio; };
private:
	AudioComponentPtr mAudio;
	SoundPtr mExplosionSound;
};
DECL_PTR(Asteroid);