#pragma once
#include "Component.h"
#include "Sound.h"

class SoundCue
{
public:
	SoundCue();
	SoundCue(int value);
	void Pause();
	void Resume();
	void Stop();
	bool IsPlaying();
	bool IsPaused();
private:
	int channel;
};

class AudioComponent : public Component
{
	DECL_COMPONENT(AudioComponent, Component);
public:
	AudioComponent(Actor& owner);
	SoundCue PlaySound(SoundPtr sound, bool looping = false);
};
DECL_PTR(AudioComponent);