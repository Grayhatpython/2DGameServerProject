#pragma once
#include "GameScene.h"

class PlayGameScene : public GameScene
{
public:
	PlayGameScene();
	virtual ~PlayGameScene();

public:
	//	TEMP
	virtual void	Initialize() override;
	virtual void	ProcessInput(const struct InputState& state) override;
	virtual void	Update(float deltaTime) override;
	virtual void	Render() override;
	virtual void	Clear() override;

};

