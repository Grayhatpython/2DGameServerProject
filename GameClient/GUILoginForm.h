#pragma once
#include "GUIScreen.h"

class GUILable;
class GUITextBox;
class GUIButton;
class GUIImage;
class GUILoginForm : public GUIScreen
{
public:
	GUILoginForm();
	virtual ~GUILoginForm();

public:
	void					Clear();

public:
	virtual void			Initialize() override;
	virtual void			Render(const Vector2Int& cameraOffset) override;
	virtual void			Update(float deltaTime) override;
	virtual bool			IsInRange(const Vector2Int& position) override;

public:
	virtual void			OnMouseReleased(const Vector2Int& position) override;
	virtual void			OnKeyBoardHeld(SDL_Scancode keyCode) override;
	virtual void			OnTextInput(const std::wstring& text) override;
	virtual void			OnMouseOver(const Vector2Int& position) override;

private:
	void					OnClickLoginButton();
	void					OnClickRegisterButton();

private:
	GUILable*	_titleText = nullptr;
	GUITextBox* _name = nullptr;
	GUITextBox* _password = nullptr;
	GUIButton*	_login = nullptr;
	GUIButton*	_register = nullptr;
	GUIImage*	_nameBackgroundImage = nullptr;
	GUIImage*	_passwordBackgroundImage = nullptr;
};