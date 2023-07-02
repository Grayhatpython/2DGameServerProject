#include "pch.h"
#include "GUIButton.h"
#include "GUILable.h"
#include "Texture.h"
#include "AssetManager.h"

GUIButton::GUIButton()
	:	GUIScreen(L"Button")
{
	_background = GAssetManager->GetTexture("../Assets/GUI/Button_Normal2.png");
	_mouseOverTexture = GAssetManager->GetTexture("../Assets/GUI/Button_Active2.png");
}

GUIButton::~GUIButton()
{
	if (_text)
	{
		delete _text;
		_text = nullptr;
	}
}

void GUIButton::Initialize()
{
	_text = new GUILable(L"ButtonLable");
	//_titleText->Initialize();
	_text->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
	_text->SetText(L"", _fontSize);
	
}

void GUIButton::Render(const Vector2Int& cameraOffset)
{
	if (_background)
		GUIScreen::DrawTexture(_background, _position);

	if (_mouseReleasedTexture && _isMouseReleased)
		GUIScreen::DrawTexture(_mouseReleasedTexture, _position);
	else
	{
		if (_mouseOverTexture && _isMouseOvered)
			GUIScreen::DrawTexture(_mouseOverTexture, _position);
	}

	if (_text)
		_text->Render(cameraOffset);
}

void GUIButton::Update(float deltaTime)
{
}

bool GUIButton::IsInRange(const Vector2Int& position)
{
	if (_background)
		return GUIScreen::IsInRange(position);
	else
	{
		Texture* texture = nullptr;
		if (_mouseReleasedTexture)
			texture = _mouseReleasedTexture;
		else
			texture = _mouseOverTexture;

		auto width = static_cast<int32>(texture->GetWidth() * _scale) / 2;
		auto height = static_cast<int32>(texture->GetHeight() * _scale) / 2;

		auto leftTopX = _position.x - width;
		auto leftTopY = _position.y - height;
		auto rightBottomX = _position.x + width;
		auto rightBottomY = _position.y + height;

		return leftTopX < position.x&& position.x < rightBottomX&& leftTopY < position.y&& position.y < rightBottomY;
	}
}

void GUIButton::OnMouseReleased(const Vector2Int& position)
{
	if (IsInRange(position))
		_isMouseReleased = true;
	else
		_isMouseReleased = false;
}

void GUIButton::OnMouseOver(const Vector2Int& position)
{
	if (IsInRange(position))
		_isMouseOvered = true;
	else
		_isMouseOvered = false;
}

void GUIButton::SetText(const std::wstring& text)
{
	_text->SetText(text, _fontSize);
}

void GUIButton::SetTextPosition(const Vector2Int& position)
{
	Texture* fontTexture = _text->GetFontTexture();

	if (fontTexture)
		_text->SetPosition(position);

}
