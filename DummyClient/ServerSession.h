#pragma once

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
	int32			GetId() { return _id; }

private:
	int32			_id = 0;
};