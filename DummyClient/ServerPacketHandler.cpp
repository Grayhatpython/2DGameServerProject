#include "pch.h"
#include "ServerPacketHandler.h"
#include "ServerSession.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	auto packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	return false;
}

bool S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CONNECTED& packet)
{
	std::wcout << L"S_CONNECTED_Packet_Processing_Function" << std::endl;

	{
		auto serverSession = std::static_pointer_cast<ServerSession>(session);
		Protocol::C_LOGIN loginSendPacket;

		//	테스트만을 위해 토큰 과정 생략 
		std::string id = "DummyClient_" + std::to_string(serverSession->GetId());
		loginSendPacket.set_uniqueid(id);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(loginSendPacket);
		serverSession->Send(sendBuffer);
	}

	return true;
}

bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet)
{
	std::wcout << L"S_LOGIN_Packet_Processing_Function" << std::endl;
	auto serverSession = std::static_pointer_cast<ServerSession>(session);

	//	로비 UI and 캐릭터 선택

	if (packet.lobbyplayers().size() == 0)
	{
		Protocol::C_CREATE_PLAYER createPlayerSendPacket;
		std::string name = "DummyPlayer_" + std::to_string(serverSession->GetId());
		createPlayerSendPacket.set_name(name);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createPlayerSendPacket);
		serverSession->Send(sendBuffer);
	}
	//	기존의 캐릭터가 있을 시
	else
	{
		//	일단 첫번째 캐릭터로 로그인
		Protocol::LobbyPlayerInfo lobbyPlayerInfo;
		lobbyPlayerInfo = packet.lobbyplayers()[0];
		Protocol::C_ENTER_GAME enterGameSendPacket;
		enterGameSendPacket.set_name(lobbyPlayerInfo.name());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGameSendPacket);
		serverSession->Send(sendBuffer);
	}

	return true;
}

bool S_CREATE_PLAYER_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CREATE_PLAYER& packet)
{
	//std::wcout << L"S_CREATE_PLAYER_Packet_Processing_Function" << std::endl;
	auto serverSession = std::static_pointer_cast<ServerSession>(session);

	//	캐릭터 만들기 성공 시
	if (packet.createok())
	{
		//	해당 캐릭터로 게임 접속 패킷 전달
		Protocol::C_ENTER_GAME enterGameSendPacket;
		enterGameSendPacket.set_name(packet.createplayer().name());
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGameSendPacket);
		serverSession->Send(sendBuffer);
	}
	//	실패 시 다시 만들어달라고 요청
	else
	{
		Protocol::C_CREATE_PLAYER createPlayerSendPacket;
		std::string name = "DummyPlayer_" + std::to_string(serverSession->GetId());
		createPlayerSendPacket.set_name(name);
		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(createPlayerSendPacket);
		serverSession->Send(sendBuffer);
	}

	return true;
}

bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet)
{
	//std::wcout << L"S_ENTER_GAME_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& packet)
{
	//std::wcout << L"S_LEAVE_GAME_Packet_Processing_Function" << std::endl;
	//	TODO?
	return true;
}

bool S_ITEM_LIST_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ITEM_LIST& packet)
{
	//std::wcout << L"S_ITEM_LIST_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_ADD_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ADD_ITEM& packet)
{
	//std::wcout << L"S_ADD_ITEM_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_EQUIP_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_EQUIP_ITEM& packet)
{
	//std::wcout << L"S_EQUIP_ITEM_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_SPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SPAWN& packet)
{
	//std::wcout << L"S_SPAWN_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_DESPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& packet)
{
	//std::cout << "S_DESPAWN_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_MOVE& packet)
{
	//std::cout << "S_MOVE_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_SKILL_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SKILL& packet)
{
	//std::cout << "S_SKILL_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_CHANGE_HP_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_HP& packet)
{
	//std::cout << "S_CHANGE_HP_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_CHANGE_STAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STAT& packet)
{
	//std::cout << "S_CHANGE_STAT_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_DEATH_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DEATH& packet)
{
	//std::cout << "S_DEATH_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_CHANGE_STATE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STATE& packet)
{
	//std::cout << "S_CHANGE_STATE_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_CHANGE_MOVE_DIR_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_MOVE_DIR& packet)
{
	//std::cout << "S_CHANGE_MOVE_DIR_Packet_Processing_Function" << std::endl;

	return true;
}

bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet)
{
	return true;
}

bool S_PING_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_PING& packet)
{
	std::cout << "S_PING_Packet_Processing_Function" << std::endl;

	return true;
}

bool L_S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CONNECTED& packet)
{
	return true;
}

bool L_S_CREATE_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CREATE_ACCOUNT& packet)
{
	return true;
}

bool L_S_LOGIN_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_LOGIN_ACCOUNT& packet)
{
	return true;
}
