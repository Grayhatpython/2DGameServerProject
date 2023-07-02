#include "pch.h"
#include "Button.h"
#include "Font.h"
Button::Button(const std::string& name, std::shared_ptr<Font> font, std::function<void()> onClick, const Vector2& position, const Vector2& dimension)
	: _name(name), _font(font), _onClick(onClick), _position(position), _dimension(dimension)
{
	//	TODO : SetName Must
}

Button::~Button()
{

}

bool Button::IsInside(const Vector2& position) const
{
	bool isInside = 
		position.x < (_position.x - _dimension.x / 2.f) ||
		position.x > (_position.x + _dimension.x / 2.f) ||
		position.y < (_position.y - _dimension.y / 2.f) ||
		position.y > (_position.y + _dimension.y / 2.f);

	return isInside == false;
}

void Button::OnClick()
{
	if (_onClick)
		_onClick();
}

void Button::SetName(const std::string& name)
{
	_nameTexture = _font->GetRenderTexture(_name);
}
