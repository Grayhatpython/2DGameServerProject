#include "pch.h"
#include "Font.h"
#include "AssetManager.h"
#include "Texture.h"

Font::Font()
{
}

Font::~Font()
{

	for (auto& font : _fontDatas)
		::TTF_CloseFont(font.second);
}

bool Font::Load(const std::string& fileName)
{
	std::vector<int> fontSizes = {
		8, 9,
		10, 11, 12, 14, 16, 18,
		20, 22, 24, 26, 28,
		30, 32, 34, 36, 38,
		40, 42, 44, 46, 48,
		52, 56,
		60, 64, 68,
		72
	};

	for (auto& size : fontSizes)
	{
		TTF_Font* font = ::TTF_OpenFont(fileName.c_str(), size);
		if (font == nullptr)
		{
			SDL_Log("Failed to load font %s in size %d", fileName.c_str(), size);
			return false;
		}

		_fontDatas.insert(std::make_pair(size, font));
	}

	return true;
}

//	문제 코드!!
bool Font::SetRenderTexture(Texture* texture, const std::wstring& text, const SDL_Color& color, int32 fontSize)
{
	auto iter = _fontDatas.find(fontSize);
	if (iter != _fontDatas.end())
	{
		texture->Clear();

		TTF_Font* font = iter->second;
		SDL_Surface* surface = nullptr;
		surface = ::TTF_RenderUNICODE_Blended_Wrapped(font, reinterpret_cast<const uint16*>(text.c_str()), color, 0);
		if (surface)
		{
			texture->CreateTextureFromSurface(surface);
			::SDL_FreeSurface(surface);
		}
		else
			return false;
	}
	else
	{
		//	TODO
		ASSERT(false);
		return false;
	}

	return true;
}
