#pragma once

class ServerService;
class LoginServer
{
public:
	LoginServer();
	~LoginServer();

public:
	bool Initialize();
	void Start();

private:
	std::shared_ptr<ServerService>			_serverService;
	std::vector<std::wstring>				_networkThreadName;
};
