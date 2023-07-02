#pragma once

class Font;
class Texture;
class Button
{
public:
	Button(const std::string& name, std::shared_ptr<Font> font, std::function<void()> onClick,
		const Vector2& position, const Vector2& dimension);
	~Button();

public:
	bool						IsInside(const Vector2& position) const;
	void						OnClick();

public:
	void						SetName(const std::string& name);

	std::shared_ptr<Texture>	GetNameTexture() { return _nameTexture; }
	const Vector2&				GetPosition() const { return _position; }
	void						SetHighlighted(bool highlighted) { _isHighlighted = highlighted; }
	bool						GetHighlighted() const { return _isHighlighted; }

	void						SetOnClickFunction(std::function<void()> func) { _onClick = func; }

private:
	std::function<void()>		_onClick;
	std::string					_name;
	std::shared_ptr<Texture>	_nameTexture;
	std::shared_ptr<Font>		_font;
	Vector2						_position{};
	Vector2						_dimension{};
	bool						_isHighlighted;
};

