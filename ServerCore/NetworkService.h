#pragma once
#include "NetworkAddress.h"
#include "IocpCore.h"
#include "Acceptor.h"

enum class ServiceType : uint8
{
	Server,
	Client,
};

class Session;
using SessionFactory = std::function<std::shared_ptr<Session>(void)>;

class NetworkService : public std::enable_shared_from_this<NetworkService>
{
public:
	NetworkService(ServiceType type, NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount = 1);
	virtual ~NetworkService();

public:
	virtual bool				Start() abstract;
	bool						CanStart() { return _sessionFactory != nullptr; }

	virtual void				Close();

	// TEMP : erase
public:
	void						Broadcast(std::shared_ptr<class SendBuffer> sendBuffer);

public:
	void						SetSessionFactory(SessionFactory sessionFactory) { _sessionFactory = sessionFactory; }
	
	void						AddSession(std::shared_ptr<Session> session);
	void						RemoveSession(std::shared_ptr<Session> session);
	std::shared_ptr<Session>	CreateSession();

	int32						GetCurrentSessionCount() { return _currentSessionCount; }
	int32						GetMaxSessionCount() { return _maxSessionCount; }

public:
	ServiceType					GetServiceType() { return _type; }
	NetworkAddress				GetNetworkAddress() { return _networkAddress; }
	std::shared_ptr<IocpCore>&	GetIocpCore() { return _iocpCore; }

protected:
	USE_LOCK;
	ServiceType						_type;
	NetworkAddress					_networkAddress{};
	std::shared_ptr<IocpCore>		_iocpCore;
	
	int32							_currentSessionCount = 0;
	int32							_maxSessionCount = 0;
	Set<std::shared_ptr<Session>>	_sessions;

	SessionFactory					_sessionFactory;
};


class ClientService : public NetworkService
{
public:
	ClientService(NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount = 1);
	virtual ~ClientService();

public:
	virtual bool				Start() override;
	virtual void				Close() override;

private:

};

class ServerService : public NetworkService
{
public:
	ServerService(NetworkAddress address, std::shared_ptr<IocpCore> iocpCore, SessionFactory sessionFactory, int32 maxSessionCount = 1);
	virtual ~ServerService();

public:
	virtual bool				Start() override;
	virtual void				Close() override;

private:
	std::shared_ptr<Acceptor>	_acceptor;
};