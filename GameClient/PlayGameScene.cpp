#include "pch.h"
#include "PlayGameScene.h"

#include "MapManager.h"
#include "ActorManager.h"
#include "GUIComponent.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "AnimationManager.h"
#include "InventoryManager.h"

#include "Texture.h"
#include "GUILable.h"
#include "GUIInventory.h"

#include "GUIButton.h"
#include "GUITextBox.h"
#include "GUIChatBox.h"

//	GameScene Change -> Collision , Render Clear
//	TODO : Texture Remain -> ?
//	TEMP
PlayGameScene::PlayGameScene()
	: GameScene(GameSceneType::Play)
{

}

PlayGameScene::~PlayGameScene()
{

}

void PlayGameScene::Initialize()
{
#ifdef _DEBUG
	GMapManager->LoadMap(L"../Common/GameData/Field/TileMap.tmx");
#else
	GMapManager->LoadMap(L"../Common/GameData/Field/TileMap.tmx");
#endif // _DEBUG

	auto inventory = GGUIManager->AddGui<GUIInventory>();
	ASSERT(inventory);

	//auto textBox = GGUIManager->AddGui<GUITextBox>();
	//ASSERT(textBox);
	//textBox->SetPosition(Vector2Int{ 300, 300 });

	auto chatBox = GGUIManager->AddGui<GUIChatBox>();
	ASSERT(chatBox);
	chatBox->SetState(GUIState::Active);

	/*auto button = GGUIManager->AddGui<GUIButton>();
	ASSERT(button);
	button->SetState(GUIState::Active);
	button->SetPosition(Vector2Int{ 300, 300 });*/
	//inventory->SetState(GUIState::Active);

	//GAssetManager->LoadText("../Assets/Test11/English.gptext");
}

void PlayGameScene::ProcessInput(const struct InputState& state)
{
	if (_state == GameState::Play)
	{
		auto gui = GGUIManager->GetGui(L"ChatBox");
		ASSERT(gui);
		auto chatBox = static_cast<GUIChatBox*>(gui);

		if (chatBox->GetInputEnabled() == false)
		{
			for (auto& actor : GActorManager->GetActors())
				actor.second->ProcessInput(state);
		}
	}
}


void PlayGameScene::Update(float deltaTime)
{
	//Performance performance;
	//	Packet Process

	GActorManager->Update(deltaTime);
	GGUIManager->Update(deltaTime);

	GActorManager->LateUpdate();
	GGUIManager->LateUpdate();
}

void PlayGameScene::Render()
{
	GRenderManager->Render();
}

void PlayGameScene::Clear()
{
	GGUIManager->Clear();
	GInputManager->Clear();
	//GAssetManager->Clear();
	//GAnimationManager->Clear();
	GRenderManager->Clear();
	GInventoryManager->Clear();
	GActorManager->Clear();
}
