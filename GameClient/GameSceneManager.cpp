#include "pch.h"
#include "GameSceneManager.h"
#include "GameScene.h"

GameSceneManager* GGameSceneManager = nullptr;

GameSceneManager::~GameSceneManager()
{

}

void GameSceneManager::SetGameScene(std::shared_ptr<GameScene> gameScene)
{
	if (_currentGameScene)
		_currentGameScene->Clear();

	if (gameScene)
		gameScene->Initialize();

	_currentGameScene = gameScene;
}

void GameSceneManager::ProcessInput(const InputState& state)
{
	if (_currentGameScene)
		_currentGameScene->ProcessInput(state);
}

void GameSceneManager::Update(float deltaTime)
{
	if (_currentGameScene)
		_currentGameScene->Update(deltaTime);
}

void GameSceneManager::Render()
{
	if (_currentGameScene)
		_currentGameScene->Render();
}
