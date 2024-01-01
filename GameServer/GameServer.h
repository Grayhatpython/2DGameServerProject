#pragma once

enum
{
	TIME_OUT = 10,
	RECV_PACKET_PROCESS_TIME_TICK = 50,
	UPDATE_FRAME_TIME_TICK = 60,
};

#include <Mmsystem.h>
#include "ThreadPool.h"

class ServerService;
class GameServer
{
public:
	GameServer();
	~GameServer();

public:
	bool Initialize();
	void Start();
	void Update();

private:
	ThreadPool								_sendThreadPool;
	std::shared_ptr<ServerService>			_serverService;
	std::vector<std::wstring>				_networkThreadName;
};

