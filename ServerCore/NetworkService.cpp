#include "pch.h"
#include "NetworkService.h"
#include "Acceptor.h"
#include "Session.h"

NetworkService::NetworkService(ServiceType type, NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount)
	: _type(type), _networkAddress(address), _iocpCore(iocpCore), _sessionFactory(sessionFactory), _maxSessionCount(maxSessionCount)
{

}

NetworkService::~NetworkService()
{

}

void NetworkService::Close()
{

}

void NetworkService::Broadcast(std::shared_ptr<SendBuffer> sendBuffer)
{
	WRITE_LOCK;
	for (auto& session : _sessions)
		session->Send(sendBuffer);
}

void NetworkService::AddSession(std::shared_ptr<Session> session)
{
	WRITE_LOCK;

	_sessions.insert(session);
	_currentSessionCount++;
}

void NetworkService::RemoveSession(std::shared_ptr<Session> session)
{
	WRITE_LOCK;

	//	erase return value -> the number of elements erased.
	session->Clear();
	ASSERT(_sessions.erase(session) != 0);
	_currentSessionCount--;
}

std::shared_ptr<Session> NetworkService::CreateSession()
{
	auto session = _sessionFactory();

	if (_iocpCore->Register(session) == false)
		return nullptr;

	session->SetNetworkSeervice(shared_from_this());

	return session;
}

//	Remote Address
ClientService::ClientService(NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount)
	: NetworkService(ServiceType::Client, address, iocpCore, sessionFactory, maxSessionCount )
{

}

ClientService::~ClientService()
{

}

bool ClientService::Start()
{
	if (CanStart() == false)
		return false;

	const int32 maxSessionCount = GetMaxSessionCount();
	for (int32 i = 0; i < maxSessionCount; i++)
	{
		auto session = CreateSession();
		if (session->Connect() == false)
			return false;
	}

	return true;
}

void ClientService::Close()
{
	//	TODO
	NetworkService::Close();

	for (auto& session : _sessions)
		session->Clear();
}

//	My Server Address
ServerService::ServerService(NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount)
	: NetworkService(ServiceType::Server, address, iocpCore, sessionFactory, maxSessionCount)
{

}

ServerService::~ServerService()
{

}

bool ServerService::Start()
{
	if (CanStart() == false)
		return false;

	_acceptor = MakeShared<Acceptor>();
	if (_acceptor == nullptr)
		return false;

	std::shared_ptr<ServerService> serverService = std::static_pointer_cast<ServerService>(shared_from_this());
	if (_acceptor->Start(serverService) == false)
		return false;

	return true;
}

void ServerService::Close()
{
	//	TODO
	NetworkService::Close();

	if(_acceptor)
		_acceptor->Close();

	for (auto& session : _sessions)
		session->Clear();
}
