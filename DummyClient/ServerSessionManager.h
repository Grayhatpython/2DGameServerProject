#pragma once

class ServerSession;
class ServerSessionManager
{
public:
	std::shared_ptr<ServerSession>			AddServerSession();
	void									RemoveServerSession(std::shared_ptr<ServerSession> serverSession);
	void									Clear();

public:
	Set<std::shared_ptr<ServerSession>>		GetSessions()
	{
		WRITE_LOCK;
		auto sessions = _serverSessions;

		return sessions;
	}

private:
	USE_LOCK;
	Set<std::shared_ptr<ServerSession>>		_serverSessions;
	int32									_generateSessionId = 1;
};

extern ServerSessionManager* GServerSessionManager;