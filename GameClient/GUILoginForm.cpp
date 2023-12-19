#include "pch.h"
#include "GUILoginForm.h"
#include "RenderManager.h"
#include "Texture.h"
#include "GUIImage.h"
#include "GUILable.h"
#include "GUIButton.h"
#include "GUITextBox.h"
#include "AssetManager.h"

#include "NetworkManager.h"
#include "ServerPacketHandler.h"

GUILoginForm::GUILoginForm()
{
	auto screenWidth = GRenderManager->GetScreenWidth();
	auto screenHeight = GRenderManager->GetScreenHeight();
	_position = Vector2Int{ screenWidth - (screenWidth / 5), screenHeight /2};
}

GUILoginForm::~GUILoginForm()
{
	if (_name)
	{
		delete _name;
		_name = nullptr;
	}
	if (_password)
	{
		delete _password;
		_password = nullptr;
	}
	if (_login)
	{
		delete _login;
		_login = nullptr;
	}
	if (_register)
	{
		delete _register;
		_register = nullptr;
	}
	if (_titleText)
	{
		delete _titleText;
		_titleText = nullptr;
	}
	if (_loginOrRegisterFailedText)
	{
		delete _loginOrRegisterFailedText;
		_loginOrRegisterFailedText = nullptr;
	}
	if (_nameBackgroundImage)
	{
		delete _nameBackgroundImage;
		_nameBackgroundImage = nullptr;
	}
	if (_passwordBackgroundImage)
	{
		delete _passwordBackgroundImage;
		_passwordBackgroundImage = nullptr;
	}
}

void GUILoginForm::Clear()
{
	_name->Initialize();
	_password->Initialize();
}

void GUILoginForm::Initialize()
{
	_titleText = new GUILable();
	_titleText->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
	_titleText->SetText(L"Kingdom");
	Texture* fontTexture = _titleText->GetFontTexture();
	if (fontTexture)
		_titleText->SetPosition(Vector2Int{ _position.x + 5, _position.y - 70});
	fontTexture = nullptr;

	_loginOrRegisterFailedText = new GUILable();
	_loginOrRegisterFailedText->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
	fontTexture = _loginOrRegisterFailedText->GetFontTexture();
	if (fontTexture)
		_loginOrRegisterFailedText->SetPosition(Vector2Int{ _position.x + 5, _position.y + 110});

	_nameBackgroundImage = new GUIImage();
	_nameBackgroundImage->SetBackGround(GAssetManager->GetTexture("../Assets/GUI/loginTextarea.png"));
	_nameBackgroundImage->SetState(GUIState::Active);
	_nameBackgroundImage->SetPosition(Vector2Int{ _position.x + 5, _position.y - 17 });

	_passwordBackgroundImage = new GUIImage();
	_passwordBackgroundImage->SetBackGround(GAssetManager->GetTexture("../Assets/GUI/loginTextarea.png"));
	_passwordBackgroundImage->SetState(GUIState::Active);
	_passwordBackgroundImage->SetPosition(Vector2Int{ _position.x + 5, _position.y + 30 });

	_name = new GUITextBox();
	_name->Initialize();
	_name->SetPosition(Vector2Int{ _position.x - 50, _position.y - 20});
	_name->SetDimension(Vector2Int{ _nameBackgroundImage->GetBackgroundTexture()->GetWidth() - 10, 15});
	_name->SetFontSize(16);
	_name->SetLimitTextLenght(20);
	_name->SetOwner(this);
	_name->SetState(GUIState::Active);

	_password = new GUITextBox();
	_password->Initialize();
	_password->SetPosition(Vector2Int{ _position.x - 50, _position.y + 27 });
	_password->SetDimension(Vector2Int{ _nameBackgroundImage->GetBackgroundTexture()->GetWidth() - 10, 15 });
	_password->SetFontSize(16);
	_password->SetLimitTextLenght(20);
	_password->SetOwner(this);
	_password->SetHide(true);
	_password->SetState(GUIState::Active);

	_login = new GUIButton();
	_login->SetPosition(Vector2Int{ _position.x + 50, _position.y + 75 });
	_login->Initialize();
	_login->SetFontSize(16);
	_login->SetState(GUIState::Active);
	_login->SetText(L"Login");
	_login->SetTextPosition(Vector2Int{ _login->GetPosition().x, _login->GetPosition().y - 2});

	_register = new GUIButton();
	_register->SetPosition(Vector2Int{ _position.x - 40, _position.y + 75 });
	_register->Initialize();
	_register->SetFontSize(16);
	_register->SetState(GUIState::Active);
	_register->SetText(L"Register");
	_register->SetTextPosition(Vector2Int{ _register->GetPosition().x, _register->GetPosition().y - 2 });

}

void GUILoginForm::Render(const Vector2Int& cameraOffset)
{
	if (_titleText)
		_titleText->Render(cameraOffset);

	if (_loginOrRegisterFailedText)
		_loginOrRegisterFailedText->Render(cameraOffset);

	if (_nameBackgroundImage)
		_nameBackgroundImage->Render(cameraOffset);

	if (_passwordBackgroundImage)
		_passwordBackgroundImage->Render(cameraOffset);

	if (_name)
		_name->Render(cameraOffset);

	if (_password)
		_password->Render(cameraOffset);

	if (_login)
		_login->Render(cameraOffset);

	if (_register)
		_register->Render(cameraOffset);
}

void GUILoginForm::Update(float deltaTime)
{

}

bool GUILoginForm::IsInRange(const Vector2Int& position)
{
	if(_name->IsInRange(position))
		return true;

	if (_password->IsInRange(position))
		return true;

	return false;
}

void GUILoginForm::OnMouseReleased(const Vector2Int& position)
{
	if (_name->IsInRange(position))
		_name->SetInputEnabled(true);
	else
		_name->SetInputEnabled(false);

	if (_password->IsInRange(position))
		_password->SetInputEnabled(true);
	else
		_password->SetInputEnabled(false);

	if (_login->IsInRange(position))
		OnClickLoginButton();

	if (_register->IsInRange(position))
		OnClickRegisterButton();
}

void GUILoginForm::OnKeyBoardHeld(SDL_Scancode keyCode)
{
	if (_name->GetInputEnabled())
		_name->OnKeyBoardHeld(keyCode);
	if (_password->GetInputEnabled())
		_password->OnKeyBoardHeld(keyCode);
}

void GUILoginForm::OnTextInput(const std::wstring& text)
{
	if (_name->GetInputEnabled())
		_name->OnTextInput(text);
	if (_password->GetInputEnabled())
		_password->OnTextInput(text);
}

void GUILoginForm::OnMouseOver(const Vector2Int& position)
{
	if (_login->IsInRange(position))
		_login->SetMouseOvered(true);
	else
		_login->SetMouseOvered(false);

	if (_register->IsInRange(position))
		_register->SetMouseOvered(true);
	else
		_register->SetMouseOvered(false);
}

void GUILoginForm::SetLoginOrRegisterFailedText(const std::wstring& text)
{
	_loginOrRegisterFailedText->SetText(text, 16, COLOR_RED);
}

void GUILoginForm::OnClickLoginButton()
{
	if (_name->IsTextEmpty() || _password->IsTextEmpty())
	{
		_loginOrRegisterFailedText->SetText(L"Please enter your login information", 16, COLOR_RED);
		return;
	}

	Protocol::L_C_LOGIN_ACCOUNT loginAccountSendPacket;
	auto name = UtilityHelper::ConvertUnicodeToUTF8(_name->GetText());
	auto password = UtilityHelper::ConvertUnicodeToUTF8(_password->GetText());

	GNetworkManager->SetAccountName(_name->GetText());

	loginAccountSendPacket.set_name(name);
	loginAccountSendPacket.set_password(password);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginAccountSendPacket);
	GNetworkManager->Send(loginAccountSendPacket);
}

void GUILoginForm::OnClickRegisterButton()
{
	if (_name->IsTextEmpty() || _password->IsTextEmpty())
	{
		_loginOrRegisterFailedText->SetText(L"Please enter your register information", 16, COLOR_RED);
		return;
	}

	Protocol::L_C_CREATE_ACCOUNT createAccountSendPacket;
	auto name = UtilityHelper::ConvertUnicodeToUTF8(_name->GetText());
	auto password = UtilityHelper::ConvertUnicodeToUTF8(_password->GetText());
	createAccountSendPacket.set_name(name);
	createAccountSendPacket.set_password(password);
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createAccountSendPacket);
	GNetworkManager->Send(createAccountSendPacket);
}
