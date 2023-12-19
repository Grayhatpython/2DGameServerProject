#include "pch.h"
#include "GUIScrollBar.h"
#include "Texture.h"

#include "AssetManager.h"
#include "RenderManager.h"

GUIScrollBar::GUIScrollBar()
	: GUIScreen(L"ScrollBar")
{
	_background = GAssetManager->GetTexture("../Assets/GUI/Bar.png");
	_sliderTexture = GAssetManager->GetTexture("../Assets/GUI/Slider.png");
}

GUIScrollBar::~GUIScrollBar()
{

}

void GUIScrollBar::Initialize()
{

}

void GUIScrollBar::Render(const Vector2Int& cameraOffset)
{
	if (_background)
		DrawTexture(_background, _position);

	if (_sliderTexture)
		DrawTexture(_sliderTexture, _position);
}

void GUIScrollBar::Update(float deltaTime)
{

}

void GUIScrollBar::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	auto value = 0;
	if (texture == _sliderTexture)
		value = _value;

	SDL_Rect dstrect;
	dstrect.w = static_cast<int32>(texture->GetWidth() * _scale);
	dstrect.h = static_cast<int32>(texture->GetHeight() * _scale);
	dstrect.x = drawPosX;
	dstrect.y = drawPosY - value;

	::SDL_RenderCopyEx(
		GRenderManager->GetRenderer(),
		texture->GetSDLTexture(),
		nullptr,
		&dstrect,
		0,
		nullptr,
		SDL_FLIP_NONE);
}

bool GUIScrollBar::IsInRange(const Vector2Int& position)
{
	auto leftTopX = _position.x;
	auto leftTopY = _position.y;
	auto rightBottomX = _position.x + _background->GetWidth();
	auto rightBottomY = _position.y + _background->GetHeight();

	return leftTopX < position.x&& position.x < rightBottomX&& leftTopY < position.y&& position.y < rightBottomY;
}

void GUIScrollBar::OnMouseWheel(const Vector2Int& position)
{
	_value += ( position.y * 10 );
	if (_value < _MinValue)
		_value = _MinValue;
	else if (_value > _MaxValue)
		_value = _MaxValue;
}

void GUIScrollBar::SetMaxValue()
{
	_value = _MinValue;
}

float GUIScrollBar::GetRation()
{
	return static_cast<float>(_value) / (-_MinValue - 3);
}
