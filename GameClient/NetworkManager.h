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
		auto serverSession = GetServerSession();
		ASSERT(serverSession);

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(packet);
		if (serverSession)
			serverSession->Send(sendBuffer, false);
	}

	void Send(std::shared_ptr<SendBuffer> sendBuffer);
	void PushRecvPacket(BYTE* buffer, int32 len);

public:
	void							SetServerSession(std::weak_ptr<PacketSession> session) { _serverSession = session; }
	std::shared_ptr<PacketSession>	GetServerSession() { return _serverSession.lock(); }

	void							SetClientGameState(Protocol::ClientGameState state) { _state = state; }
	Protocol::ClientGameState		GetClientGameState() { return _state; }

public:
	int32							GetAccountId() const { return _accountId; }
	void							SetAccountId(int32 id) { _accountId = id; }

	void							SetToken(int32 token) { _token = token; }
	int32							GetToken() const { return _token; }

	void							SetAccountName(const std::wstring& name) { _accountName = name; }
	std::wstring					GetAccountName() { return _accountName; }

private:
	const NetworkManager& operator = (const NetworkManager&) = delete;
	NetworkManager(const NetworkManager&) = delete;

private:
	USE_LOCK;

	Queue<std::shared_ptr<RecvPacketInfo>>	_recvPacketInfos;
	std::shared_ptr<ClientService>			_clientService;

	std::weak_ptr<PacketSession>			_serverSession;			//	LoginServer Or GameServer

	std::vector<std::wstring>				_networkThreadName;

private:

	//	MasterDB ( 로그인 서버로 부터 얻은 토큰 및 계정 넘버 )
	//	Game Server 접속 시 인증을 위해..
	int32									_accountId = 0;
	int32									_token = 0;
	std::wstring							_accountName;

	Protocol::ClientGameState				_state = Protocol::ClientGameState::CLIENT_STATE_LOGIN;
};

extern NetworkManager* GNetworkManager;
