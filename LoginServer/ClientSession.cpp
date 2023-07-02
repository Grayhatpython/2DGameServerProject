#include "pch.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include "ClientSessionManager.h"

void ClientSession::OnConnected()
{
	auto session = std::static_pointer_cast<ClientSession>(shared_from_this());

	std::wcout << L"Client OnConnected" << std::endl;

	{
		Protocol::L_S_CONNECTED connectedSendPacket;
		connectedSendPacket.set_name("LoginServer");
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(connectedSendPacket);
		Send(sendBuffer);
	}

	GClientSessionManager->AddClientSession(session);
}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSession();

	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::PacketProcessing(session, buffer, len);
}

void ClientSession::OnSend(int32 len)
{

}

void ClientSession::OnDisconnected()
{
	std::wcout << L"Client OnDisconnected" << std::endl;

	GClientSessionManager->RemoveClientSession(std::static_pointer_cast<ClientSession>(shared_from_this()));
}
