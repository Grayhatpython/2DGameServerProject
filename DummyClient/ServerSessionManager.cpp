#include "pch.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"

ServerSessionManager* GServerSessionManager = nullptr;;

std::shared_ptr<ServerSession> ServerSessionManager::AddServerSession()
{
	WRITE_LOCK;

	//	TODO
	auto serverSession = std::make_shared<ServerSession>();
	serverSession->_id = _generateSessionId;

	_serverSessions.insert(serverSession);
	_generateSessionId++;

	std::wcout << "Create ServerSession : " << serverSession->_id << std::endl;
	return serverSession;
}

//	TODO
void ServerSessionManager::RemoveServerSession(std::shared_ptr<ServerSession> serverSession)
{
	WRITE_LOCK;

	_serverSessions.erase(serverSession);
	std::wcout << "Remove ServerSession : " << serverSession->_id << std::endl;
}

void ServerSessionManager::Clear()
{
	WRITE_LOCK;
	for (auto& session : _serverSessions)
		session->Disconnect(L"테스트 종료");
}


