#include "pch.h"
#include "ClientPacketHandler.h"
#include "ClientSession.h"
#include "Player.h"
#include "Region.h"

PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
{
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
	return false;
}

bool C_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_LOGIN& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	clientSession->Login(packet);

	return true;
}

bool C_CREATE_PLAYER_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CREATE_PLAYER& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	clientSession->CreatePlayer(packet);
	return true;
}

bool C_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_ENTER_GAME& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	clientSession->EnterGame(packet);
	return true;
}

bool C_EQUIP_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_EQUIP_ITEM& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::EquipItem, myPlayer, std::move(packet));

	return true;
}

bool C_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_MOVE& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	//std::wcout << packet.positioninfo().positionx() << L" , " << packet.positioninfo().positiony() << std::endl;

	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::Move, myPlayer, std::move(packet));

	return true;
}

bool C_SKILL_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_SKILL& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::Skill, myPlayer, std::move(packet));

	return true;
}

bool C_CHANGE_STATE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHANGE_STATE& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::ChangeState, myPlayer, std::move(packet));

	return true;
}

bool C_CHANGE_MOVE_DIR_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHANGE_MOVE_DIR& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::ChangeMoveDir, myPlayer, std::move(packet));

	return true;
}

bool C_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_CHAT& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);

	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::ChatMessage, myPlayer, std::move(packet));

	return true;
}

bool C_PONG_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_PONG& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	clientSession->Pong();
	return true;
}

bool C_POSITION_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::C_POSITION& packet)
{
	auto clientSession = std::static_pointer_cast<ClientSession>(session);
	auto myPlayer = clientSession->GetMyPlayer();
	auto region = myPlayer->GetRegion();

	if (myPlayer == nullptr || region == nullptr)
		return false;

	region->OnlyPushJobAndNotDistribute(&Region::Position, myPlayer, std::move(packet));

	return true;
}