#pragma once

class GameScene;
class GameSceneManager
{
public:
	GameSceneManager() = default;
	~GameSceneManager();

public:
	std::shared_ptr<GameScene>	GetGameScene() { return _currentGameScene; }
	void						SetGameScene(std::shared_ptr<GameScene> gameScene);

public:
	void ProcessInput(const struct InputState& state);
	void Update(float deltaTime);
	void Render();

private:
	const GameSceneManager& operator = (const GameSceneManager&) = delete;
	GameSceneManager(const GameSceneManager&) = delete;

private:
	std::shared_ptr<GameScene> _currentGameScene = nullptr;
};

extern GameSceneManager* GGameSceneManager;