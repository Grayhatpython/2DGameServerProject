#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"
#include "NetworkManager.h"

void ServerSession::OnConnected()
{
	std::cout << "OnConnected" << std::endl;

	auto session = GetPacketSession();

	GNetworkManager->SetServerSession(session);

	/*Protocol::C_LOGIN loginPacket;
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginPacket);
	Send(sendBuffer);*/
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSession();
	//	�л� ������ ���
	//	��Ŷ Id�� �뿪�� Ȯ�� �� �ٸ� �������� �Դ� ������ Ȥ�� Ŭ���̾�Ʈ���� �Դ� ������
	//	�и��ؼ� ó���� �ʿ䰡 �ִ�.
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	GNetworkManager->PushRecvPacket(buffer, len);
}

void ServerSession::OnSend(int32 len)
{

}

void ServerSession::OnDisconnected()
{
	std::cout << "OnDisconnected" << std::endl;
}