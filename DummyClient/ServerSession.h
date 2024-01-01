#pragma once

class MyPlayer;
class ServerSession : public PacketSession
{
	friend class ServerSessionManager;

public:
	virtual ~ServerSession()
	{
		std::cout << "~ServerSession" << std::endl;
	}

public:
	virtual void	OnConnected() override;
	virtual void	OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual	void	OnSend(int32 len) override;
	virtual void	OnDisconnected() override;

public:
	int32						GetId() { return _id; }

	void						SetMyPlayer(std::shared_ptr<MyPlayer> player) { _myPlayer = player; }
	std::shared_ptr<MyPlayer>&	GetMyPlayer() { return _myPlayer; }

private:
	int32						_id = 0;
	std::shared_ptr<MyPlayer>	_myPlayer;
};