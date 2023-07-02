#pragma once

class Texture
{
public:
	Texture();
	~Texture();

public:
	bool Load(const std::string& fileName);
	void Clear();
	void CreateTextureFromSurface(SDL_Surface* surface);

public:
	SDL_Texture*	GetSDLTexture() { return _texture; }
	int32			GetWidth() { return _width; }
	int32			GetHeight() { return _height; }

private:
	SDL_Texture*	_texture = nullptr;
	int32			_width = 0;
	int32			_height = 0;
};

