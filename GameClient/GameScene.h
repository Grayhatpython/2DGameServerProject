#pragma once

enum class GameSceneType
{
	None,
	Login,
	Lobby,
	Play,
	Menu,
};

enum class GameState
{
	Play,
	Paused,
	Quit,
};

class GameScene : std::enable_shared_from_this<GameScene>
{
public:
	GameScene(GameSceneType type)
		: _type(type)
	{

	}
	virtual ~GameScene()
	{

	}

	virtual void Initialize() abstract;
	virtual void ProcessInput(const struct InputState& state) abstract;
	virtual void Update(float deltaTime) abstract;
	virtual void Render() abstract;
	virtual void Clear() abstract;

public:
	GameSceneType	GetGameSceneType() { return _type; }

public:
	void			SetState(GameState state) { _state = state; }
	GameState		GetState() { return _state; }

protected:
	GameState		_state = GameState::Play;
	GameSceneType	_type = GameSceneType::None;
};