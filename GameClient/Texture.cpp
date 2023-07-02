#include "pch.h"
#include "Texture.h"
#include "RenderManager.h"
#include <SDL_image.h>
Texture::Texture()
{

}

Texture::~Texture()
{
	if (_texture)
	{
		::SDL_DestroyTexture(_texture);
		_texture = nullptr;
	}
}

bool Texture::Load(const std::string& fileName)
{
	// Load from file
	SDL_Surface* surface = ::IMG_Load(fileName.c_str());
	if (surface == nullptr)
	{
		::SDL_Log("Failed to load texture file %s", fileName.c_str());
		return false;
	}

	// Create texture from surface
	_texture = ::SDL_CreateTextureFromSurface(GRenderManager->GetRenderer(), surface);
	::SDL_FreeSurface(surface);
	if (!_texture)
	{
		::SDL_Log("Failed to convert surface to texture for %s", fileName.c_str());
		return false;
	}

	::SDL_QueryTexture(_texture, nullptr, nullptr, &_width, &_height);

	return true;
}

void Texture::Clear()
{
	if (_texture)
	{
		::SDL_DestroyTexture(_texture);
		_texture = nullptr;
		_width = 0;
		_height = 0;
	}
}

void Texture::CreateTextureFromSurface(SDL_Surface* surface)
{
	_texture = ::SDL_CreateTextureFromSurface(GRenderManager->GetRenderer(), surface);

	if (!_texture)
	{
		::SDL_Log("Failed to convert surface to texture");
		return;
	}

	::SDL_QueryTexture(_texture, nullptr, nullptr, &_width, &_height);
}
