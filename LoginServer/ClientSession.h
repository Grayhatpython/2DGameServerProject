#pragma once

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
};