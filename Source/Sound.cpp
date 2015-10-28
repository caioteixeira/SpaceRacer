#include "Sound.h"
#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>

Sound::Sound()
{
	mData = nullptr;
}

Sound::~Sound()
{
	if (mData)
	{
		Mix_FreeChunk(mData);
	}
}

bool Sound::Load(const char * filename,  AssetCache * cache)
{
	Mix_Chunk * temp = Mix_LoadWAV(filename);
	if (temp)
	{
		mData = temp;
		return true;
	}
	else
	{
		SDL_Log("Failed to load the file");
		return false;
	}
}

struct Mix_Chunk * Sound::GetData()
{
	return mData;
}
