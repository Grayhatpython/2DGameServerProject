#pragma once
#include "IocpCore.h"
#include "IocpEvent.h"
#include "NetworkAddress.h"

#include "RecvBuffer.h"
//#include "BoundedQueue.h"

class NetworkService;
class Session : public IocpObject
{
	enum
	{
		BUFFER_SIZE = 0x10000,
	};

	friend class NetworkService;
	friend class ClientService;
	friend class ServerService;
	friend class IocpCore;
	friend class Acceptor;

public:
	Session();
	virtual ~Session();

public:
	void							Send(std::shared_ptr<SendBuffer> sendBuffer, bool onlyPush = true);
	void							FlushSend();
	bool							Connect();
	void							Disconnect(const WCHAR* cause);

public:
	std::shared_ptr<NetworkService>	GetNetworkService() { return _networkService.lock(); }
	void							SetNetworkSeervice(std::weak_ptr<NetworkService> networkService) { _networkService = networkService; }

	std::shared_ptr<Session>		GetSession() { return std::static_pointer_cast<Session>(shared_from_this()); }

	void							SetNetworkAddress(NetworkAddress networkAddress) { _address = networkAddress; }
	NetworkAddress					GetNetworkAddress() { return _address; }
	SOCKET							GetSocket() { return _socket; }

	bool							IsConnected() { return _isConnected.load(); }

private:
	virtual HANDLE	GetHandle() override;
	virtual void	Dispatch(class IocpEvent* iocpEvent, int32 numOfBytes = 0) override;

private:
	bool			RegisterConnect();
	bool			RegisterDisconnect();
	void			RegisterRecv();
	void			RegisterSend();

	void			ProcessConnect();
	void			ProcessDisconnect();
	void			ProcessRecv(int32 numOfBytes);
	void			ProcessSend(int32 numOfBytes);

	void			ErrorHandling(int32 errorCode);

protected:
	virtual void	OnConnected() {}
	virtual int32	OnRecv(BYTE* buffer, int32 len) { return len; }
	virtual	void	OnSend(int32 len) {}
	virtual void	OnDisconnected() {}

protected:
	virtual void	Clear();

private:
	SOCKET							_socket = INVALID_SOCKET;
	NetworkAddress					_address{};
	std::atomic<bool>				_isConnected = false;
	std::weak_ptr<NetworkService>	_networkService;

private:
	USE_LOCK;

private:
	RecvBuffer									_recvBuffer;

	Queue<std::shared_ptr<SendBuffer>>			_sendBufferQueue;

	std::atomic<bool>							_isRegisterdSend = false;
	//BoundedQueue<std::shared_ptr<SendBuffer>>	_sendBufferQueue;

private:
	ConnectEvent	_connectEvent;
	DisconnectEvent	_disconnectEvent;
	RecvEvent		_recvEvent;
	SendEvent		_sendEvent;
};

struct PacketHeader
{
	//	Total Size [ PacketHeader Size + Data Size ]
	uint16 size = 0;
	uint16 id = 0;
};

class PacketSession : public Session
{
protected:
	PacketSession();
	virtual ~PacketSession();

public:
	std::shared_ptr<PacketSession>	GetPacketSession() { return std::static_pointer_cast<PacketSession>(shared_from_this()); }

public:
	virtual int32	OnRecv(BYTE* buffer, int32 len) final;
	virtual void	OnRecvPacket(BYTE* buffer, int32 len) abstract;

};
