#pragma once
#include "GUIScreen.h"

class Font;
class Texture;
class GUITextBox;
class GUIScrollBar;
class GUIChatBox : public GUIScreen
{
public:
	GUIChatBox();
	virtual ~GUIChatBox();

public:
	virtual void			Initialize();
	virtual void			Render(const Vector2Int& cameraOffset);
	virtual void			Update(float deltaTime);

public:
	virtual void			OnMousePressed(const Vector2Int& position) override;
	virtual void			OnMouseReleased(const Vector2Int& position) override;
	virtual void			OnMouseHeld(const Vector2Int& position) override;
	virtual void			OnMouseWheel(const Vector2Int& position) override;
	virtual void			OnKeyBoardHeld(SDL_Scancode keyCode) override;
	virtual void			OnKeyBoardReleased(SDL_Scancode keyCode) override;
	virtual void			OnTextInput(const std::wstring& text) override;

public:
	void					OnSendChatMessage();

protected:
	virtual void			DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset = Vector2Int{ 0, 0 }, float scale = 1.f) override;

private:
	void					SetText(const std::wstring& text, int32 fontSize = 24, SDL_Color color = WHITE);

public:
	void					AddChatMessage(const std::wstring& name, const std::wstring& message);

public:
	bool					GetInputEnabled();

private:
	Map<int32, std::wstring>	_chatLogs;

	GUIScrollBar*				_scrollbar = nullptr;
	GUITextBox*					_textBox = nullptr;
	std::wstring				_chatLogText;

	Font*						_font = nullptr;
	Texture*					_fontTexture = nullptr;

	const int32					_MaxChatLogCount = 30;
	int32						_currentChatLogCount = 1;
	int32						_removeChatLogIndex = 1;

	int32						_lines = 0;
	int32						_offset = 0;

	Vector2Int					_prevMousePressedPosition{};
};