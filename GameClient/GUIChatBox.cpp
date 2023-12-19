#include "pch.h"
#include "GUIChatBox.h"
#include "GUITextBox.h"
#include "GUIScrollBar.h"

#include "Font.h"
#include "Texture.h"
#include "AssetManager.h"
#include "RenderManager.h"

#include "NetworkManager.h"
#include "ServerPacketHandler.h"

GUIChatBox::GUIChatBox()
	: GUIScreen(L"ChatBox")
{
	_font = GAssetManager->GetFont("../Assets/Font/font_Regular.ttf");
	_fontTexture = new Texture(); 

	_background = GAssetManager->GetTexture("../Assets/GUI/ChatBox.png");

	auto screenHeight = GRenderManager->GetScreenHeight();
	_position = Vector2Int{ _background->GetWidth() / 2 + 20, screenHeight - _background->GetHeight() / 2 - 20 };
}

GUIChatBox::~GUIChatBox()
{
	if (_scrollbar)
	{
		delete _scrollbar;
		_scrollbar = nullptr;
	}
	if (_textBox)
	{
		delete _textBox;
		_textBox = nullptr;
	}
	if (_fontTexture)
	{
		delete _fontTexture;
		_fontTexture = nullptr;
	}
}

void GUIChatBox::Initialize()
{
	_textBox = new GUITextBox();
	_textBox->Initialize();
	_textBox->SetPosition(Vector2Int{ _position.x - _background->GetWidth() / 2 + 5, _position.y + _background->GetHeight() / 2 - 14});
	_textBox->SetDimension(Vector2Int{ _background->GetWidth() - 10, 10 });
	_textBox->SetOwner(this);
	_textBox->SetState(GUIState::Active);

	_scrollbar = new GUIScrollBar();
	_scrollbar->Initialize();
	_scrollbar->SetPosition(Vector2Int{ _position.x + _background->GetWidth() / 2 - 19, _position.y - _background->GetHeight() / 2 + 39 });
	_scrollbar->SetState(GUIState::Active);
}

void GUIChatBox::Render(const Vector2Int& cameraOffset)
{
	if (_background)
		GUIScreen::DrawTexture(_background, _position);
	
	if(_textBox)
		_textBox->Render(cameraOffset);

	if(_scrollbar)
		_scrollbar->Render(cameraOffset);

	if(_fontTexture)
		DrawTexture(_fontTexture, _position);
}

void GUIChatBox::Update(float deltaTime)
{

}

void GUIChatBox::OnMousePressed(const Vector2Int& position)
{
	_prevMousePressedPosition = position;
}

void GUIChatBox::OnMouseReleased(const Vector2Int& position)
{
	if (_textBox->IsInRange(position))
		_textBox->SetInputEnabled(true);
	else
	{
		//	TEMP
		SHORT hangeulKeyState = GetKeyState(VK_HANGEUL);
		bool isDown = hangeulKeyState & 1;

		if (isDown)
		{
			keybd_event(VK_HANGEUL, 0, 0, 0);
			keybd_event(VK_HANGEUL, 0, KEYEVENTF_KEYUP, 0);
		}
		_textBox->SetInputEnabled(false);
	}
}

void GUIChatBox::OnMouseHeld(const Vector2Int& position)
{
	if (_textBox->IsInRange(_prevMousePressedPosition))
		return;
	else
	{
		if (IsInRange(_prevMousePressedPosition))
		{
			if (_prevMousePressedPosition.x != position.x || _prevMousePressedPosition.y != position.y)
			{
				auto diff = position - _prevMousePressedPosition;

				_position += diff;

				auto scrollbarPosition = _scrollbar->GetPosition();
				scrollbarPosition += diff;
				_scrollbar->SetPosition(scrollbarPosition);

				auto textBoxPosition = _textBox->GetPosition();
				textBoxPosition += diff;
				_textBox->SetPosition(textBoxPosition);

				_prevMousePressedPosition = position;
			}
		}
	}
}

void GUIChatBox::OnMouseWheel(const Vector2Int& position)
{
	if (_scrollbar->GetState() == GUIState::Active)
		_scrollbar->OnMouseWheel(position);
}

void GUIChatBox::OnKeyBoardHeld(SDL_Scancode keyCode)
{
	if (_textBox->GetInputEnabled())
		_textBox->OnKeyBoardHeld(keyCode);
}

void GUIChatBox::OnKeyBoardReleased(SDL_Scancode keyCode)
{
	if (_textBox->GetInputEnabled())
		_textBox->OnKeyBoardReleased(keyCode);
}

void GUIChatBox::OnTextInput(const std::wstring& text)
{
	if (_textBox->GetInputEnabled())
		_textBox->OnTextInput(text);
}

void GUIChatBox::OnSendChatMessage()
{
	Protocol::C_CHAT chatSendPacket;
	chatSendPacket.set_chat(UtilityHelper::ConvertUnicodeToUTF8(_textBox->GetText()));
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(chatSendPacket);
	GNetworkManager->Send(chatSendPacket);
}

void GUIChatBox::DrawTexture(Texture* texture, const Vector2Int& position, const Vector2Int& cameraOffset)
{
	int32 drawPosX = static_cast<int32>(position.x - cameraOffset.x);
	int32 drawPosY = static_cast<int32>(position.y - cameraOffset.y);

	auto ratio = _scrollbar->GetRation();
	auto offset = _offset;
	if (ratio > 0 && _offset > 0)
		offset = static_cast<int32>(_offset * _scrollbar->GetRation());

	SDL_Rect srcrect;
	srcrect.w = static_cast<int32>(texture->GetWidth() * _scale);
	srcrect.h = static_cast<int32>(texture->GetHeight() * _scale) - _offset;
	srcrect.x = 0;
	srcrect.y = offset;

	SDL_Rect dstrect;
	dstrect.w = static_cast<int32>(texture->GetWidth() * _scale);
	dstrect.h = static_cast<int32>(texture->GetHeight() * _scale) - _offset;
	dstrect.x = drawPosX - _background->GetWidth() / 2 + 4;
	dstrect.y = drawPosY - _background->GetWidth() / 2 + 15;

	::SDL_RenderCopyEx(
		GRenderManager->GetRenderer(),
		texture->GetSDLTexture(),
		&srcrect,
		&dstrect,
		0,
		nullptr,
		SDL_FLIP_NONE);

}

void GUIChatBox::SetText(const std::wstring& text, int32 fontSize, SDL_Color color)
{
	_font->SetRenderTexture(_fontTexture, text,  color, fontSize);
}

//	14 line
void GUIChatBox::AddChatMessage(const std::wstring& name, const std::wstring& message)
{
	_lines = 0;

	std::wstring chatLog;
	chatLog += L'[';
	chatLog += name;
	chatLog += L"] : ";
	chatLog += message;

	_chatLogText.clear();

	if (_currentChatLogCount > _MaxChatLogCount)
	{
		_chatLogs.erase(_removeChatLogIndex);
		_removeChatLogIndex++;
	}

	_chatLogs.insert(std::make_pair(_currentChatLogCount, chatLog));
	_currentChatLogCount++;

	for (auto& log : _chatLogs)
	{
		auto found = log.second.find(L'\n');
		auto lastIndex = log.second.length() - 1;

		if (found != lastIndex)
			_lines += 2;
		else
			_lines++;

		_chatLogText += log.second;
	}

	SetText(_chatLogText, 12);

	auto diff = _lines * 16 - (_background->GetHeight() - 80);
	if (diff > 0)
	{
		_offset = diff;
		_scrollbar->SetMaxValue();
	}
}

bool GUIChatBox::GetInputEnabled()
{
	return _textBox->GetInputEnabled();
}
