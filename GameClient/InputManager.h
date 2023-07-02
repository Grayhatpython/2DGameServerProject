#pragma once

#include "EventHandler.h"

enum class ButtonState
{
	None,
	Pressed,
	Released,
	Held,
};

class KeyboardState
{
	friend class InputManager;
public:
	bool			GetKeyValue(SDL_Scancode keyCode) const;
	ButtonState		GetKeyState(SDL_Scancode keyCode) const;
	ButtonState		GetKeyState(uint16 keyCode) const;

private:	
	const uint8*	_currentState = SDL_GetKeyboardState(nullptr);
	uint8			_prevState[SDL_NUM_SCANCODES] = { 0, };
};

class MouseState
{
	friend class InputManager;
public:
	const Vector2Int&	GetPosition() const { return _mousePosition; }
	const Vector2Int&	GetScrollWheel() const { return _scrollWheel; }
	bool				IsRelativeMode() const { return _isRelativeMode; }
	bool				GetButtonValue(int32 button) const;
	ButtonState			GetButtonState(int32 button) const;

private:
	Vector2Int	_mousePosition{};
	Vector2Int	_scrollWheel{};
	uint32		_currentButtons = 0;
	uint32		_prevButtons = 0;
	bool		_isRelativeMode = false;
};

struct InputState
{
	KeyboardState	keyboard;
	MouseState		mouse;
};

class InputManager
{
public:
	InputManager();
	~InputManager();

public:
	bool Initialize();
	void Clear();

public:
	void Prepare();
	void Dispatch();
	void ProcessEvent(union SDL_Event& sdlEvent);

public:
	void SetRelativeMouseMode(bool enabled);

public:
	//	Event Register
	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterMouseReleasedEvent(T* owner, MemberFunc memberFunc)
	{
		return _mouseReleasedEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterMousePressedEvent(T* owner, MemberFunc memberFunc)
	{
		return _mousePressedEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterMouseHeldEvent(T* owner, MemberFunc memberFunc)
	{
		return _mouseHeldEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterMouseOverEvent(T* owner, MemberFunc memberFunc)
	{
		return _mouseOverEvent.Bind(owner, memberFunc);
	}


	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterMouseWheelEvent(T* owner, MemberFunc memberFunc)
	{
		return _mouseWheelEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterKeyBoardReleasedEvent(T* owner, MemberFunc memberFunc)
	{
		return _keyBoardReleasedEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterKeyBoardHeldEvent(T* owner, MemberFunc memberFunc)
	{
		return _keyBoardHeldEvent.Bind(owner, memberFunc);
	}

	template<typename T, typename MemberFunc>
	ConnectedEventHandler RegisterTextInputEvent(T* owner, MemberFunc memberFunc)
	{
		return _textInputEvent.Bind(owner, memberFunc);
	}


public:
	const InputState& GetState() const { return _state; }

private:
	const InputManager& operator = (const InputManager&) = delete;
	InputManager(const InputManager&) = delete;

private:
	InputState _state;

	//	Event 
	EVENT_TYPE(MouseEvent, Vector2Int);
	MouseEvent _mouseReleasedEvent;

	EVENT_TYPE(MouseEvent, Vector2Int);
	MouseEvent _mousePressedEvent;

	EVENT_TYPE(MouseEvent, Vector2Int);
	MouseEvent _mouseHeldEvent;

	EVENT_TYPE(MouseEvent, Vector2Int);
	MouseEvent _mouseOverEvent;

	EVENT_TYPE(MouseEvent, Vector2Int);
	MouseEvent _mouseWheelEvent;

	EVENT_TYPE(keyBoardEvent, SDL_Scancode);
	keyBoardEvent _keyBoardReleasedEvent;

	EVENT_TYPE(keyBoardEvent, SDL_Scancode);
	keyBoardEvent _keyBoardHeldEvent;

	EVENT_TYPE(TextInputEvent, std::wstring);
	TextInputEvent _textInputEvent;
};


extern InputManager* GInputManager;
