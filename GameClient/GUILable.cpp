#include "pch.h"
#include "GUILable.h"
#include "Font.h"
#include "Texture.h"

#include "AssetManager.h"

GUILable::GUILable()
{
	_fontTexture = new Texture();
	_font = GAssetManager->GetFont("../Assets/Font/font_Regular.ttf");
}

GUILable::GUILable(const std::wstring& name)
	: GUIScreen(name)
{
	_fontTexture = new Texture();
	_font = GAssetManager->GetFont("../Assets/Font/font_Regular.ttf");
}

GUILable::~GUILable()
{
	if (_fontTexture)
	{
		delete _fontTexture;
		_fontTexture = nullptr;
	}
}

void GUILable::Initialize()
{
}

void GUILable::Render(const Vector2Int& cameraOffset)
{
	GUIScreen::DrawTexture(_fontTexture, _position);
}

void GUILable::Update(float deltaTime)
{

}
bool GUILable::IsInRange(const Vector2Int& position)
{
	auto width = static_cast<int32>(_fontTexture->GetWidth() * _scale) / 2;
	auto height = static_cast<int32>(_fontTexture->GetHeight() * _scale) / 2;

	auto leftTopX = _position.x - width;
	auto leftTopY = _position.y - height;
	auto rightBottomX = _position.x + width;
	auto rightBottomY = _position.y + height;

	return leftTopX < position.x&& position.x < rightBottomX&& leftTopY < position.y&& position.y < rightBottomY;
}


void GUILable::SetText(const std::wstring& text, int32 fontSize, SDL_Color color)
{
	_font->SetRenderTexture(_fontTexture, text, color, fontSize);
}

