#include "pch.h"
#include "ClientSessionManager.h"
#include "ClientSession.h"

ClientSessionManager* GClientSessionManager = nullptr;;

void ClientSessionManager::AddClientSession(std::shared_ptr<ClientSession> clientSession)
{
	WRITE_LOCK;
	_clientSessions.insert(clientSession);
}

void ClientSessionManager::RemoveClientSession(std::shared_ptr<ClientSession> clientSession)
{
	WRITE_LOCK;
	_clientSessions.erase(clientSession);
}

void ClientSessionManager::Broadcast(std::shared_ptr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (auto& clientSession : _clientSessions)
		clientSession->Send(sendBuffer);
}
