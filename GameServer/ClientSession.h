#pragma once

#include "Protocol.pb.h"
class Player;
class ClientSession : public PacketSession
{
public:
	virtual ~ClientSession()
	{
		std::cout << "~ClientSession" << std::endl;
	}

public:
	//	Network Processing
	virtual void	OnConnected() override;
	virtual void	OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual	void	OnSend(int32 len) override;
	virtual void	OnDisconnected() override;

public:
	//	Logic
	void Login(Protocol::C_LOGIN& loginPacket);
	void CreatePlayer(Protocol::C_CREATE_PLAYER& createPlayerPacket);
	void EnterGame(Protocol::C_ENTER_GAME& enterGamePacket);

public:
	//	HeartBeat
	void Ping();
	void Pong();

public:
	std::shared_ptr<Player>		GetMyPlayer() { return _myPlayer; }

private:
	Protocol::ClientGameState				_clientGameState = Protocol::ClientGameState::CLIENT_STATE_LOGIN;
	std::vector<Protocol::LobbyPlayerInfo>	_lobbyPlayerInfos;
	int32									_accountDbId = 0;

	std::shared_ptr<Player>					_myPlayer;

	uint64									_heartbeatTick = 0;
};