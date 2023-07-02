#pragma once
//#include "BoundedQueue.h"

class IocpObject;
class Session;

enum class EventType : uint8
{
	Connect,
	Disconnect,
	Accept,
	Recv,
	Send,
};

class IocpEvent : public WSAOVERLAPPED
{
public:
	IocpEvent(EventType type);

public:
	void		Initialize();
	EventType	GetEventType() { return _type; }

	void						SetOwner(std::shared_ptr<IocpObject> owner) { _owner = owner; }
	std::shared_ptr<IocpObject>	GetOwner() { return _owner; }

protected:
	EventType					_type;
	//	Event Owner -> Session or Acceptor
	std::shared_ptr<IocpObject> _owner;
};

class ConnectEvent : public IocpEvent
{
public:
	ConnectEvent() : IocpEvent(EventType::Connect)
	{

	}
};

class DisconnectEvent : public IocpEvent
{
public:
	DisconnectEvent() : IocpEvent(EventType::Disconnect)
	{

	}
};


class AcceptEvent : public IocpEvent
{
public:
	AcceptEvent() : IocpEvent(EventType::Accept)
	{

	}

public:
	void						SetSession(std::shared_ptr<Session> session) { _session = session; }
	std::shared_ptr<Session> 	GetSession() { return _session; }

private:
	std::shared_ptr<Session> _session;
};

class RecvEvent : public IocpEvent
{
public:
	RecvEvent() : IocpEvent(EventType::Recv)
	{

	}

private:

};

class SendEvent : public IocpEvent
{
public:
	SendEvent() 
		: IocpEvent(EventType::Send)/*, _sendBuffers2(65536)*/
	{

	}
	
public:
	Vector<std::shared_ptr<SendBuffer>>&		GetSendBuffers() { return _sendBuffers; }

private:
	Vector<std::shared_ptr<SendBuffer>>			_sendBuffers;
};
