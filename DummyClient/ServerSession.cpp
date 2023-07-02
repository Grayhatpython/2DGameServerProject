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
	//	�л� ������ ���
	//	��Ŷ Id�� �뿪�� Ȯ�� �� �ٸ� �������� �Դ� ������ Ȥ�� Ŭ���̾�Ʈ���� �Դ� ������
	//	�и��ؼ� ó���� �ʿ䰡 �ִ�.
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