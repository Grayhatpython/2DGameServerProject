#pragma once
#include "IocpCore.h"
#include "NetworkAddress.h"

class AcceptEvent;
class ServerService;
class Acceptor : public IocpObject
{
	friend class NetworkService;
	friend class IocpCore;

public:
	Acceptor();
	virtual ~Acceptor();

public:
	bool Start(std::shared_ptr<ServerService> serverService);
	void Clear();
	void Close();

private:
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	void RegisterAccept(AcceptEvent* acceptEvent);
	void ProcessAccept(AcceptEvent* acceptEvent);

private:
	SOCKET							_socket = INVALID_SOCKET;
	Vector<AcceptEvent*>			_acceptEvents;
	//	Ref
	std::shared_ptr<ServerService>	_serverService;
};

