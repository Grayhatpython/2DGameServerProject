#pragma once

enum
{
	TIME_OUT = 10,
	WORK_CHECK_TICK = 100,
};

class ServerService;
class GameServer
{
public:
	GameServer();
	~GameServer();

public:
	bool Initialize();
	void Start();

private:
	std::shared_ptr<ServerService>			_serverService;
	std::vector<std::wstring>				_networkThreadName;
};

