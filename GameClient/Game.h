#pragma once
#include <SDL.h>

#define POSITION_INFO_SEND_TICK 500

#include "PlayGameScene.h"
class Game
{
public:
	Game();
	~Game();

public:
	bool Initialize();
	void Run();

private:
	void ProcessInput();
	void Update();
	void Render();

private:
	//	TEMP
	uint64							_tickCount = 0;
	uint64							_startTick = 0;
	bool							_isRunning = false;
};
