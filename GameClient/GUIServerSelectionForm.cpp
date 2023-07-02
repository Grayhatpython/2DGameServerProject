#include "pch.h"
#include "GUIServerSelectionForm.h"
#include "Texture.h"

#include "GUILable.h"
#include "GUIButton.h"
#include "AssetManager.h"
#include "RenderManager.h"

#include "NetworkManager.h"

#include "PlayGameScene.h"
#include "GameSceneManager.h"
GUIServerSelectionForm::GUIServerSelectionForm()
{
	_background = GAssetManager->GetTexture("../Assets/GUI/ServerSelectionBG.png");

	auto screenWidth = GRenderManager->GetScreenWidth();
	auto screenHeight = GRenderManager->GetScreenHeight();
	_position = Vector2Int{ screenWidth / 2 , screenHeight / 2};
}

GUIServerSelectionForm::~GUIServerSelectionForm()
{
	if (_titleText)
	{
		delete _titleText;
		_titleText = nullptr;
	}

	if (_startBtn)
	{
		delete _startBtn;
		_startBtn = nullptr;
	}

	for (auto serverName : _serverNames)
		delete serverName;

	for (auto serverNameBtn : _serverNamesBtn)
		delete serverNameBtn;

	_serverInfos.clear();
	_serverNames.clear();
	_serverNamesBtn.clear();
}

void GUIServerSelectionForm::Initialize()
{
	_titleText = new GUILable(L"Inventory Title");
	_titleText->Initialize();
	_titleText->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
	_titleText->SetText(L"서버 선택", 14);
	Texture* fontTexture = _titleText->GetFontTexture();

	if (fontTexture)
		_titleText->SetPosition(Vector2Int{ _position.x, _position.y - _background->GetHeight() / 2 + 8 });

	_startBtn = new GUIButton();
	_startBtn->SetPosition(Vector2Int{ _position.x + _background->GetWidth() / 2 - 40, _position.y + _background->GetHeight() / 2 - 15});
	_startBtn->Initialize();
	_startBtn->SetFontSize(16);
	_startBtn->SetState(GUIState::Active);
	_startBtn->SetText(L"시작");
	_startBtn->SetTextPosition(Vector2Int{ _startBtn->GetPosition().x, _startBtn->GetPosition().y - 2 });
}

void GUIServerSelectionForm::Render(const Vector2Int& cameraOffset)
{
	if (_background)
		GUIScreen::DrawTexture(_background, _position);

	if (_titleText)
		_titleText->Render(cameraOffset);

	for (auto serverName : _serverNames)
		serverName->Render(cameraOffset);

	for (auto serverNameBtn : _serverNamesBtn)
		serverNameBtn->Render(cameraOffset);

	if (_startBtn)
		_startBtn->Render(cameraOffset);
}

void GUIServerSelectionForm::OnMouseReleased(const Vector2Int& position)
{
	if (_selectServerIndex >= 0 && _startBtn->IsInRange(position))
	{
		//	게임 서버로 고고씽..
		OnClickGameStartButton();
		return;
	}

	_selectServerIndex = -1;

	for (int32 i = 0; i < _serverNamesBtn.size(); i++)
	{
		if(_serverNamesBtn[i]->IsInRange(position))
			_selectServerIndex = i;

		_serverNamesBtn[i]->OnMouseReleased(position);
	}

}

void GUIServerSelectionForm::OnMouseOver(const Vector2Int& position)
{
	for (auto btn : _serverNamesBtn)
		btn->OnMouseOver(position);

	if(_startBtn)
		_startBtn->OnMouseOver(position);
}

void GUIServerSelectionForm::SetServerInfos(const std::vector<Protocol::ServerInfo> serverInfos)
{
	int32 i = 1;
	int32 offset = 30;

	for (auto serverInfo : serverInfos)
	{
		auto name = new GUILable();
		name->Initialize();
		name->SetFont(GAssetManager->GetFont("../Assets/Font/font_Bold.ttf"));
		auto serverName = UtilityHelper::ConvertUTF8ToUnicode(serverInfo.name());
		name->SetText(serverName, 16);
		Texture* fontTexture = name->GetFontTexture();

		if (fontTexture)
			name->SetPosition(Vector2Int{ _position.x, _position.y + 10 -(_background->GetHeight() / 2) + (offset * i) });

		auto btn = new GUIButton();
		btn->SetPosition(Vector2Int{ _position.x, _position.y + 10 - (_background->GetHeight() / 2) + (offset * i) });
		btn->SetBackGround(nullptr);
		btn->SetMouseOverTexture(GAssetManager->GetTexture("../Assets/GUI/Border_Hover.png"));
		btn->SetMouseReleasedTexture(GAssetManager->GetTexture("../Assets/GUI/Border_Active.png"));
		btn->SetState(GUIState::Active);

		_serverInfos.push_back(serverInfo);
		_serverNames.push_back(name);
		_serverNamesBtn.push_back(btn);

		i++;
	}
}

void GUIServerSelectionForm::OnClickGameStartButton()
{
	std::wcout << L"게임 서버 접속!" << std::endl;

	auto ip = UtilityHelper::ConvertUTF8ToUnicode(_serverInfos[_selectServerIndex].ip());
	auto port = _serverInfos[_selectServerIndex].port();

	auto playGameScene = std::make_shared<PlayGameScene>();
	GGameSceneManager->SetGameScene(playGameScene);
	GNetworkManager->ConnectToGameServer(ip, port);
}
