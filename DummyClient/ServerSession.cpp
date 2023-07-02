#include "pch.h"
#include "ServerSession.h"
#include "ServerPacketHandler.h"

void ServerSession::OnConnected()
{
	std::wcout << L"OnConnected" << std::endl;
}

void ServerSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSession();
	//	분산 서버의 경우
	//	패킷 Id의 대역을 확인 후 다른 서버에서 왔는 것인지 혹은 클라이언트에서 왔는 것인지
	//	분리해서 처리할 필요가 있다.
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	ServerPacketHandler::PacketProcessing(session, buffer, len);
}

void ServerSession::OnSend(int32 len)
{

}

void ServerSession::OnDisconnected()
{
	std::wcout << L"OnDisconnected" << std::endl;
}