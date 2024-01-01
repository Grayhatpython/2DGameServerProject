#pragma once

class ClientSession;
class ClientSessionManager
{
public:
	void AddClientSession(std::shared_ptr<ClientSession> clientSession);
	void RemoveClientSession(std::shared_ptr<ClientSession> clientSession);
	void Broadcast(std::shared_ptr<SendBuffer> sendBuffer);
	void BroadcastHeartbeat();

public:
	Set<std::shared_ptr<ClientSession>>	GetSessions()
	{
		WRITE_LOCK;
		auto sessions = _clientSessions;

		return sessions;
	}

	int32									GetServerComlexity()
	{
		WRITE_LOCK;
		int32 sessionCount = static_cast<int32>(_clientSessions.size());

		return sessionCount / 100;
	}

private:
	USE_LOCK;
	Set<std::shared_ptr<ClientSession>>		_clientSessions;
};

extern ClientSessionManager* GClientSessionManager;