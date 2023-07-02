#pragma once
#include "GameScene.h"

class LobbyGameScene : public GameScene
{
public:
	LobbyGameScene();
	virtual ~LobbyGameScene();

public:
	//	TEMP
	virtual void	Initialize() override;
	virtual void	ProcessInput(const struct InputState& state) override;
	virtual void	Update(float deltaTime) override;
	virtual void	Render() override;
	virtual void	Clear() override;

};

