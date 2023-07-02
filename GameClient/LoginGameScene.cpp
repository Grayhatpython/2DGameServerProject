#include "pch.h"
#include "LoginGameScene.h"
#include "GUIManager.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "RenderManager.h"

#include "GUIImage.h"
#include "GUILoginForm.h"
#include "GUIServerSelectionForm.h"

LoginGameScene::LoginGameScene()
	: GameScene(GameSceneType::Login)
{

}

LoginGameScene::~LoginGameScene()
{

}

void LoginGameScene::Initialize()
{
	auto mainBackground = GGUIManager->AddGui<GUIImage>(L"MainBackground");
	ASSERT(mainBackground);
	auto width = GRenderManager->GetScreenWidth();
	auto height = GRenderManager->GetScreenHeight();
	mainBackground->SetBackGround(GAssetManager->GetTexture("../Assets/GUI/MainBackground2.png"));
	mainBackground->SetState(GUIState::Active);
	mainBackground->SetPosition(Vector2Int{ width / 2, height / 2 });

	auto loginForm = GGUIManager->AddGui<GUILoginForm>(L"LoginForm");
	ASSERT(loginForm);
	loginForm->SetState(GUIState::Active);

	auto serverSelectionForm = GGUIManager->AddGui<GUIServerSelectionForm>(L"ServerSelectionForm");
	ASSERT(serverSelectionForm);
}

void LoginGameScene::ProcessInput(const InputState& state)
{
	/*auto mainBackground = GGUIManager->GetGui(L"MainBackground");
	auto width = GRenderManager->GetScreenWidth();
	auto height = GRenderManager->GetScreenHeight();
	mainBackground->SetPosition(Vector2Int{ width / 2, height / 2 });*/
}

void LoginGameScene::Update(float deltaTime)
{
	//Performance performance;
	//	Packet Process

	GGUIManager->Update(deltaTime);
	GGUIManager->LateUpdate();
}

void LoginGameScene::Render()
{
	GGUIManager->Render(Vector2Int{0,0});
}

void LoginGameScene::Clear()
{
	GGUIManager->Clear();
	GInputManager->Clear();
	GRenderManager->Clear();
}
