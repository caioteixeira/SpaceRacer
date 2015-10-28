#include "Texture.h"
#include "Game.h"
#include <SOIL/SOIL.h>
#include <SDL/SDL.h>

Texture::Texture()
	:mTextureID(0)
	,mWidth(-1)
	,mHeight(-1)
{

}

Texture::~Texture()
{
	glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive()
{
	glBindTexture(GL_TEXTURE_2D, mTextureID);
}

bool Texture::Load(const char* fileName, class AssetCache* cache)
{
	int channels;
	unsigned char* image =
		SOIL_load_image(fileName, &mWidth, &mHeight, &channels, SOIL_LOAD_AUTO);
	
	if (image == nullptr)
	{
		SDL_Log("Failed to load Texture %s", fileName);
		return false;
	}
	
	int mode = GL_RGB;

	if (channels == 4)
	{
		mode = GL_RGBA;
	}

	// Generate a GL texture
	glGenTextures(1, &mTextureID);
	glBindTexture(GL_TEXTURE_2D, mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, mode, mWidth, mWidth, 0, mode,
		GL_UNSIGNED_BYTE, image);

	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate mip maps, just in case
	glGenerateMipmap(GL_TEXTURE_2D);

	return true;
}

std::shared_ptr<Texture> Texture::CreateFromSurface(SDL_Surface* surface)
{
	TexturePtr tex = std::make_shared<Texture>();
	tex->mWidth = surface->w;
	tex->mHeight = surface->h;

	// Generate a GL texture
	glGenTextures(1, &(tex->mTextureID));
	glBindTexture(GL_TEXTURE_2D, tex->mTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex->mWidth, tex->mHeight, 0, GL_BGRA,
		GL_UNSIGNED_BYTE, surface->pixels);

	// Use linear filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Generate mip maps, just in case
	glGenerateMipmap(GL_TEXTURE_2D);

	return tex;
}
