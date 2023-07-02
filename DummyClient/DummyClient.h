#pragma once
#include "ServerPacketHandler.h"

class ClientService;
class ServerSession;
class DummyClient
{
public:
	DummyClient();
	~DummyClient();

public:
	bool Initialize();
	void Start();
	void Clear();

private:
	const DummyClient& operator = (const DummyClient&) = delete;
	DummyClient(const DummyClient&) = delete;

private:
	USE_LOCK;
	std::shared_ptr<ClientService>			_clientService;
	std::vector<std::wstring>				_workerThreadNames;
};

extern DummyClient* GDummyClient;
