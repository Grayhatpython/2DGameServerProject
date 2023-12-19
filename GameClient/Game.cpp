#include "pch.h"
#include "Game.h"
#include "GUIManager.h"
#include "MapManager.h"
#include "ActorManager.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "RenderManager.h"
#include "NetworkManager.h"
#include "AnimationManager.h"
#include "CollisionManager.h"
#include "GameSceneManager.h"
#include "DataManager.h"
#include "InventoryManager.h"
#include "LoginGameScene.h"

Game::Game()
{
}

Game::~Game()
{
	if (GNetworkManager)
	{
		delete GNetworkManager;
		GNetworkManager = nullptr;
	}
	if (GCollisionManager)
	{
		delete GCollisionManager;
		GCollisionManager = nullptr;
	}
	if (GAnimationManager)
	{
		delete GAnimationManager;
		GAnimationManager = nullptr;
	}
	if (GInventoryManager)
	{
		delete GInventoryManager;
		GInventoryManager = nullptr;
	}
	if (GMapManager)
	{
		delete GMapManager;
		GMapManager = nullptr;
	}
	if (GActorManager)
	{
		delete GActorManager;
		GActorManager = nullptr;
	}
	if (GGUIManager)
	{
		delete GGUIManager;
		GGUIManager = nullptr;
	}
	if (GInputManager)
	{
		delete GInputManager;
		GInputManager = nullptr;
	}
	if (GAssetManager)
	{
		delete GAssetManager;
		GAssetManager = nullptr;
	}
	if (GGameSceneManager)
	{
		delete GGameSceneManager;
		GGameSceneManager = nullptr;
	}
	if (GRenderManager)
	{
		delete GRenderManager;
		GRenderManager = nullptr;
	}

	DataManager::Clear();
}

bool Game::Initialize()
{
	GNetworkManager = new NetworkManager();
	GInputManager = new InputManager();
	GCollisionManager = new CollisionManager();
	GGameSceneManager = new GameSceneManager();
	GAnimationManager = new AnimationManager();
	GAssetManager = new AssetManager();
	GGUIManager = new GUIManager();
	GMapManager = new MapManager();
	GActorManager = new ActorManager();
	GRenderManager = new RenderManager();
	GInventoryManager = new InventoryManager();
	
	if (GRenderManager->Initialize(1024, 768) == false)
		return false;

	ASSERT(DataManager::LoadDatas());

	if (GNetworkManager->Initialize() == false)
		return false;

	GNetworkManager->Start();

	return true;
}

void Game::Run()
{
	_tickCount = SDL_GetTicks();
	_isRunning = true;

	//	TODO
	{
		auto loginGameScene = std::make_shared<LoginGameScene>();
		GGameSceneManager->SetGameScene(loginGameScene);

		/*auto playGameScene = std::make_shared<PlayGameScene>();
		GGameSceneManager->SetGameScene(playGameScene);*/
	}

	while (true)
	{
		GNetworkManager->PrcoessPackets();

		auto gameScene = GGameSceneManager->GetGameScene();	
		if (gameScene && gameScene->GetState() != GameState::Quit)
		{
			//Performance p;
			ProcessInput();
			Update();
			Render();
		}
		else
			break;
	}

	GNetworkManager->Clear();
}

void Game::ProcessInput()
{
	GInputManager->Prepare();

	SDL_Event sdlEvent;
	while (::SDL_PollEvent(&sdlEvent))
	{
		switch (sdlEvent.type)
		{
		case SDL_QUIT:
			{
				auto gameScene = GGameSceneManager->GetGameScene();
				if(gameScene)
					gameScene->SetState(GameState::Quit);
			}
			break;
		case SDL_TEXTINPUT:
			GInputManager->ProcessEvent(sdlEvent);
			break;
		case SDL_MOUSEWHEEL:
			GInputManager->ProcessEvent(sdlEvent);
			break;
		case SDL_WINDOWEVENT:
			if (sdlEvent.window.event == SDL_WINDOWEVENT_RESIZED)
			{
				GRenderManager->SetScreenWidth(sdlEvent.window.data1);
				GRenderManager->SetScreenHeight(sdlEvent.window.data1);
			}
			break;
		default:
			break;
		}
	}

	//	등록된 이벤트 처리 -> UI
	GInputManager->Dispatch();

	const auto& state = GInputManager->GetState();

	//	Input 정보 전달 -> Actor
	GGameSceneManager->ProcessInput(state);
}

void Game::Update()
{
	while (!SDL_TICKS_PASSED(::SDL_GetTicks64(), _tickCount + 16))
		;

	float deltaTime = (::SDL_GetTicks64() - _tickCount) / 1000.0f;
	if (deltaTime > 0.05f)
		deltaTime = 0.05f;

	_tickCount = ::SDL_GetTicks64();

	GGameSceneManager->Update(deltaTime);
}

void Game::Render()
{
	GRenderManager->SetBackBufferColor(SDL_Color{ 0,0,0,255 });
	GGameSceneManager->Render();

	GRenderManager->Present();
}

