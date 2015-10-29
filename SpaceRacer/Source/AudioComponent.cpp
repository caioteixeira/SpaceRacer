#include "AudioComponent.h"
#include "Actor.h"
#include <SDL/SDL_mixer.h>

IMPL_COMPONENT(AudioComponent, Component);

SoundCue::SoundCue(int value)
{
	channel = value;
}

SoundCue::SoundCue()
{
	channel = -1;
}

void SoundCue::Pause()
{
	Mix_Pause(channel);
}

void SoundCue::Resume()
{
	Mix_Resume(channel);
}

void SoundCue::Stop()
{
	Mix_HaltChannel(channel);
}

bool SoundCue::IsPlaying()
{
	return (bool) Mix_Playing(channel);
}

bool SoundCue::IsPaused()
{
	return (bool) Mix_Paused(channel);
}

AudioComponent::AudioComponent(Actor& owner)
	:Component(owner)
{

}

SoundCue AudioComponent::PlaySound(SoundPtr sound, bool looping)
{
	int loopingVal = looping ? -1 : 0;
	int channel = Mix_PlayChannel(-1, sound->GetData(), loopingVal);
	SoundCue cue(channel);
	return cue;
}