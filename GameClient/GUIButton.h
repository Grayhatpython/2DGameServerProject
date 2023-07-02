#pragma once
#include "GUIScreen.h"

class Texture;
class GUILable;
class GUIButton : public GUIScreen
{
public:
	GUIButton();
	virtual ~GUIButton();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);
	virtual bool			IsInRange(const Vector2Int& position) override;

public:
	virtual void			OnMouseReleased(const Vector2Int& position) override;
	virtual void			OnMouseOver(const Vector2Int& position) override;

public:
	void					SetText(const std::wstring& text);
	void					SetTextPosition(const Vector2Int& position);
	void					SetMouseOvered(bool overed) { _isMouseOvered = overed; }
	void					SetFontSize(int32 size) { _fontSize = size; }
	
	void					SetMouseOverTexture(Texture* texture) { _mouseOverTexture = texture; }
	void					SetMouseReleasedTexture(Texture* texture) { _mouseReleasedTexture = texture; }

private:
	GUILable*		_text = nullptr;
	Texture*		_mouseReleasedTexture = nullptr;
	Texture*		_mouseOverTexture = nullptr;

	bool			_isMouseOvered = false;
	bool			_isMouseReleased = false;
	int32			_fontSize = 12;
};
