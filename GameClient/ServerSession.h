#pragma once

class ServerSession : public PacketSession
{
public:
	virtual ~ServerSession()
	{
		std::cout << "~ServerSession" << std::endl;
	}

public:
	std::shared_ptr<ServerSession> GetServerSession() { return std::static_pointer_cast<ServerSession>(shared_from_this()); }

public:
	virtual void	OnConnected() override;
	virtual void	OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual	void	OnSend(int32 len) override;
	virtual void	OnDisconnected() override;
};