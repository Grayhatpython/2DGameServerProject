#pragma once

#include "ServerPacketHandler.h"

struct RecvPacketInfo
{
	RecvPacketInfo()
	{

	}

	RecvPacketInfo(BYTE* buffer, int32 len)
		: buffer(buffer), len(len)
	{

	}

	BYTE* buffer = nullptr;
	int32 len = 0;
};

class ClientService;
class ServerSession;
class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

public:
	bool Initialize();
	void PrcoessPackets();
	void Start();
	void ConnectToGameServer(const std::wstring& ip, int32 port);
	void Clear();

public:
	template<typename T>
	void Send(T& packet)
	{
		std::shared_ptr<PacketSession> session;

		if (_state == Protocol::CLIENT_STATE_LOGIN)
			session = GetLoginServerSession();
		else if (_state == Protocol::CLIENT_STATE_GAME)
			session = GetGameServerSession();
		else
			ASSERT(false);

		ASSERT(session);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		if (session)
			session->Send(sendBuffer, false);
	}

	void Send(std::shared_ptr<SendBuffer> sendBuffer);
	void PushRecvPacket(BYTE* buffer, int32 len);

public:
	void							SetGameServerSession(std::weak_ptr<PacketSession> session) { _gameServerSession = session; }
	std::shared_ptr<PacketSession>	GetGameServerSession() { return _gameServerSession.lock(); }

	void							SetLoginServerSession(std::weak_ptr<PacketSession> session) { _loginServerSession = session; }
	std::shared_ptr<PacketSession>	GetLoginServerSession() { return _loginServerSession.lock(); }

	void							SetClientGameState(Protocol::ClientGameState state) { _state = state; }
	Protocol::ClientGameState		GetClientGameState() { return _state; }

private:
	const NetworkManager& operator = (const NetworkManager&) = delete;
	NetworkManager(const NetworkManager&) = delete;

private:
	USE_LOCK;

	Queue<std::shared_ptr<RecvPacketInfo>>	_recvPacketInfos;
	std::shared_ptr<ClientService>			_clientService;

	std::weak_ptr<PacketSession>			_gameServerSession;		//	GameServer
	std::weak_ptr<PacketSession>			_loginServerSession;	//	LoginServer
	std::vector<std::wstring>				_networkThreadName;

	Protocol::ClientGameState				_state = Protocol::ClientGameState::CLIENT_STATE_GAME;
};

extern NetworkManager* GNetworkManager;
