#pragma once
#include "GUIScreen.h"

class Font;
class GUILable : public GUIScreen
{
public:
	GUILable();
	GUILable(const std::wstring& name);
	virtual ~GUILable();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);
	virtual bool			IsInRange(const Vector2Int& position) override;
public:
	void					SetText(const std::wstring& text, int32 fontSize = 24, SDL_Color color = { 255,255,255,255 });
	void					SetFont(Font* font) { _font = font; }

	Texture*				GetFontTexture() { return _fontTexture; }

private:
	Font*					_font = nullptr;
	Texture*				_fontTexture = nullptr;
	std::wstring			_text;
};

