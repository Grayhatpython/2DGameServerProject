#include "pch.h"
#include "InputManager.h"

InputManager* GInputManager = nullptr;

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const
{
	return _currentState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const
{
	if (_prevState[keyCode] == 0)
	{
		if (_currentState[keyCode] == 0)
			return ButtonState::None;
		else
			return ButtonState::Pressed;
	}
	else
	{
		if (_currentState[keyCode] == 0)
			return ButtonState::Released;
		else
			return ButtonState::Held;
	}
}

ButtonState KeyboardState::GetKeyState(uint16 keyCode) const
{
	if (_prevState[keyCode] == 0)
	{
		if (_currentState[keyCode] == 0)
			return ButtonState::None;
		else
			return ButtonState::Pressed;
	}
	else
	{
		if (_currentState[keyCode] == 0)
			return ButtonState::Released;
		else
			return ButtonState::Held;
	}
}

bool MouseState::GetButtonValue(int32 button) const
{
	return (SDL_BUTTON(button) & _currentButtons) == 1;
}

ButtonState MouseState::GetButtonState(int32 button) const
{
	int32 mask = SDL_BUTTON(button);
	if ((mask & _prevButtons) == 0)
	{
		if ((mask & _currentButtons) == 0)
		{
			return ButtonState::None;
		}
		else
		{
			return ButtonState::Pressed;
		}
	}
	else
	{
		if ((mask & _currentButtons) == 0)
		{
			return ButtonState::Released;
		}
		else
		{
			return ButtonState::Held;
		}
	}
}

InputManager::InputManager()
{

}

InputManager::~InputManager()
{

}

bool InputManager::Initialize()
{
	Clear();

	return true;
}

void InputManager::Clear()
{
	_state.keyboard._currentState = ::SDL_GetKeyboardState(nullptr);
	::memset(_state.keyboard._prevState, 0, SDL_NUM_SCANCODES);

	_state.mouse._mousePosition = Vector2Int{};
	_state.mouse._scrollWheel = Vector2Int{};
	_state.mouse._currentButtons = 0;
	_state.mouse._prevButtons = 0;
	_state.mouse._isRelativeMode = false;
}

void InputManager::Prepare()
{
	::memcpy(_state.keyboard._prevState,_state.keyboard._currentState, SDL_NUM_SCANCODES);
	_state.mouse._prevButtons = _state.mouse._currentButtons;
	_state.mouse._isRelativeMode = false;
	_state.mouse._scrollWheel = Vector2Int{};
}

void InputManager::Dispatch()
{
	int32 x = 0;
	int32 y = 0;
	if (_state.mouse._isRelativeMode)
		_state.mouse._currentButtons = ::SDL_GetRelativeMouseState(&x, &y);
	else
		_state.mouse._currentButtons = ::SDL_GetMouseState(&x, &y);

	_state.mouse._mousePosition.x = x;
	_state.mouse._mousePosition.y = y;

	//	Dispatch Input 
	{
		for (uint16 scanCode = 0; scanCode < SDL_NUM_SCANCODES; scanCode++)
		{
			if (_state.keyboard.GetKeyState(scanCode) == ButtonState::Released)
				_keyBoardReleasedEvent(static_cast<SDL_Scancode>(scanCode));
		}

		for (uint16 scanCode = 0; scanCode < SDL_NUM_SCANCODES; scanCode++)
		{
			if (_state.keyboard.GetKeyState(scanCode) == ButtonState::Held)
				_keyBoardHeldEvent(static_cast<SDL_Scancode>(scanCode));
		}

		if (_state.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::Pressed)
			_mousePressedEvent(_state.mouse._mousePosition);

		if (_state.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::Held)
			_mouseHeldEvent(_state.mouse._mousePosition);

		if (_state.mouse.GetButtonState(SDL_BUTTON_LEFT) == ButtonState::Released)
			_mouseReleasedEvent(_state.mouse._mousePosition);

		_mouseOverEvent(_state.mouse._mousePosition);
	}
}

void InputManager::ProcessEvent(SDL_Event& sdlEvent)
{

	switch (sdlEvent.type)
	{
	case SDL_MOUSEWHEEL:
		{
			_state.mouse._scrollWheel = Vector2Int(
				sdlEvent.wheel.x,
				sdlEvent.wheel.y);
			_mouseWheelEvent(_state.mouse._scrollWheel);
		}
		break;
	case SDL_KEYDOWN:
		break;
	case SDL_TEXTINPUT:
		{
			auto text = UtilityHelper::ConvertUTF8ToUnicode(sdlEvent.text.text);
			_textInputEvent(text);
		}
		break;
	default:
		break;
	}
}

void InputManager::SetRelativeMouseMode(bool enabled)
{
	::SDL_SetRelativeMouseMode(static_cast<SDL_bool>(enabled));
	_state.mouse._isRelativeMode = enabled;
}
