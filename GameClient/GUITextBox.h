#pragma once
#include "GUIScreen.h"

class Font;
class GUITextBox : public GUIScreen
{
public:
	GUITextBox();
	virtual ~GUITextBox();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);
	virtual bool			IsInRange(const Vector2Int& position) override;

public:
	virtual void			OnKeyBoardHeld(SDL_Scancode keyCode) override;
	virtual void			OnKeyBoardReleased(SDL_Scancode keyCode) override;
	virtual void			OnTextInput(const std::wstring& text) override;
	virtual void			DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{ 0, 0 }, float scale = 1.f);

public:
	void					SetText(const std::wstring& text, int32 fontSize = 24, SDL_Color color = WHITE);
	std::wstring			GetText() { return _sendText; }

	void					SetCursorText(const std::wstring& text, int32 fontSize = 24, SDL_Color color = WHITE);

	Texture*				GetFontTexture() { return _fontTexture; }

	void					SetDimension(const Vector2Int& dimension) { _dimension = dimension; }

	void					SetOwner(GUIScreen* owner) { _owner = owner; }
	void					SetLimitTextLenght(int32 length) { _limitTextLength = length; }

	void					SetFontSize(int32 size) { _fontSize = size; }

	void					SetHide(bool hide) { _isHide = hide; }
	
	void					SetInputEnabled(bool enabled) { _isInputEnabled = enabled; }
	bool					GetInputEnabled() { return _isInputEnabled;}

	bool					IsTextEmpty() { return _text.empty(); }

private:
	GUIScreen*				_owner = nullptr;
	Font*					_font = nullptr;
	Texture*				_fontTexture = nullptr;
	Texture*				_cursorTexture = nullptr;

	std::wstring			_text;
	std::wstring			_sendText;
	std::wstring			_cursorText;

	uint64					_cursorTick = 0;
	bool					_toggle = false;

	Vector2Int				_dimension{};

	uint64					_backspaceEraseTick = 0;

	int32					_limitTextLength = 50;
	const int32				_SendNameTextLength = 60;
	const int32				_wrapLength = 210;

	bool					_isLineWrapped = false;
	int32					_fontSize = 12;

	bool					_isHide = false;
	bool					_isInputEnabled = false;
};
