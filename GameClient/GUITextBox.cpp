#include "pch.h"
#include "GUITextBox.h"
#include "Font.h"
#include "Texture.h"
#include "AssetManager.h"
#include "RenderManager.h"

#include "GUIChatBox.h"
#include "GUIManager.h"

GUITextBox::GUITextBox()
	: GUIScreen(L"TextBox")
{
	_fontTexture = new Texture();
	_cursorTexture = new Texture();
	_font = GAssetManager->GetFont("../Assets/Font/font_Regular.ttf");
}

GUITextBox::~GUITextBox()
{
	_font = nullptr;
	_owner = nullptr;
	if (_fontTexture)
	{
		delete _fontTexture;
		_fontTexture = nullptr;
	}

	if (_cursorTexture)
	{
		delete _cursorTexture;
		_cursorTexture = nullptr;
	}
}

void GUITextBox::Initialize()
{
	_cursorTick = 0;
	_cursorText = L'|';
	SetCursorText(_cursorText, _fontSize);
}

void GUITextBox::Render(const Vector2Int& cameraOffset)
{
	if (_state == GUIState::Active)
	{
		if (_cursorTick < ::GetTickCount64())
		{
			if (_toggle)
				_cursorText = L' ';
			else
				_cursorText = L'|';

			_toggle = !_toggle;
			_cursorTick = ::GetTickCount64() + 500;
			SetCursorText(_cursorText, _fontSize);
		}

		Vector2Int textPosition{ _position.x, _position.y };
		DrawTexture(_fontTexture, textPosition);

		if (_isInputEnabled)
		{
			auto offset = _fontTexture->GetWidth();
			if (offset > _dimension.x)
				offset = _dimension.x;

			Vector2Int cursorPosition{ _position.x + offset, _position.y };
			DrawTexture(_cursorTexture, cursorPosition);
		}
	}
}

void GUITextBox::Update(float deltaTime)
{

}

bool GUITextBox::IsInRange(const Vector2Int& position)
{
	auto leftTopX = _position.x;
	auto leftTopY = _position.y - _dimension.y;
	auto rightBottomX = _position.x + _dimension.x;
	auto rightBottomY = _position.y + _dimension.y;

	return leftTopX < position.x&& position.x < rightBottomX&& leftTopY < position.y&& position.y < rightBottomY;
}

void GUITextBox::OnKeyBoardHeld(SDL_Scancode keyCode)
{
	if (keyCode == SDL_SCANCODE_BACKSPACE)
	{
		if (_backspaceEraseTick < ::GetTickCount64())
		{
			if (_text.size() > 0)
			{
				_text.pop_back();
				if (_sendText.back() == L'\n')
				{
					_isLineWrapped = false;
					_sendText.pop_back();
				}
				_sendText.pop_back();
				SetText(_text, _fontSize);
			}
			_backspaceEraseTick = ::GetTickCount64() + 50;
		}
	}
}

void GUITextBox::OnKeyBoardReleased(SDL_Scancode keyCode)
{
	ASSERT(_owner);

	if (_owner->GetName() == L"ChatBox")
	{
		//	메세지 패킷 전송..
		if (keyCode == SDL_SCANCODE_KP_ENTER || keyCode == SDL_SCANCODE_RETURN)
		{
			_isLineWrapped = false;

			if (_text.empty())
				return;

			if (_sendText.size() > 0)
			{
				if (_sendText.back() != L'\n')
					_sendText += L'\n';
			}

			auto chatBox = static_cast<GUIChatBox*>(_owner);
			ASSERT(chatBox);

			chatBox->OnSendChatMessage();
	
			_text.clear();
			_sendText.clear();
			SetText(_text, _fontSize);
		}
	}
}

void GUITextBox::OnTextInput(const std::wstring& text)
{
	if (_text.length() > _limitTextLength)
		return;

	if (_isHide)
		_text += L'*';
	else
		_text += text;

	_sendText += text;
	SetText(_text, _fontSize);

	if (_isLineWrapped == false && _fontTexture->GetWidth() + _SendNameTextLength >= _wrapLength)
	{
		_sendText += L'\n';
		_isLineWrapped = true;
	}
}

void GUITextBox::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	auto offsetX = texture->GetWidth() - _dimension.x;
	if (offsetX < 0)
		offsetX = 0;

	SDL_Rect srcrect;
	srcrect.w = static_cast<int32>(texture->GetWidth() * _scale);
	srcrect.h = static_cast<int32>(texture->GetHeight() * _scale);
	srcrect.x = offsetX;
	srcrect.y = 0;

	SDL_Rect dstrect;
	auto width = texture->GetWidth();
	if (width > _dimension.x)
		width = _dimension.x;

	dstrect.w = static_cast<int32>(width * _scale);
	dstrect.h = static_cast<int32>(texture->GetHeight() * _scale);
	dstrect.x = drawPosX;
	dstrect.y = drawPosY - dstrect.h / 2;

	::SDL_RenderCopyEx(
		GRenderManager->GetRenderer(),
		texture->GetSDLTexture(),
		&srcrect,
		&dstrect,
		0,
		nullptr,
		SDL_FLIP_NONE);
}

void GUITextBox::SetText(const std::wstring& text, int32 fontSize, SDL_Color color)
{
	_font->SetRenderTexture(_fontTexture, text, color, fontSize);
}

void GUITextBox::SetCursorText(const std::wstring& text, int32 fontSize, SDL_Color color)
{
	_font->SetRenderTexture(_cursorTexture, text, color, fontSize);
}
