#pragma once
#include <SDL_ttf.h>
class Texture;
class Font
{
public:
	Font();
	~Font();

public:
	bool	Load(const std::string& fileName);
	bool	SetRenderTexture(Texture* texture, const std::wstring& text, const SDL_Color& color = SDL_Color{255,255,255,255}, int32 fontSize = 24);

private:
	std::unordered_map<int32, TTF_Font*>	_fontDatas;
};

