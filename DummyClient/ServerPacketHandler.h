#pragma once
#include "Protocol.pb.h"

using PacketProcessingFunction = std::function<bool(std::shared_ptr<PacketSession>&, BYTE*, int32)>;
extern PacketProcessingFunction GPacketPacketProcessingFunction[UINT16_MAX];

enum : uint16
{
//	Auto
	PACKET_L_S_CONNECTED = 100,
	PACKET_L_C_CREATE_ACCOUNT = 101,
	PACKET_L_S_CREATE_ACCOUNT = 102,
	PACKET_L_C_LOGIN_ACCOUNT = 103,
	PACKET_L_S_LOGIN_ACCOUNT = 104,
	PACKET_S_CONNECTED = 200,
	PACKET_C_LOGIN = 201,
	PACKET_S_LOGIN = 202,
	PACKET_C_CREATE_PLAYER = 203,
	PACKET_S_CREATE_PLAYER = 204,
	PACKET_C_ENTER_GAME = 205,
	PACKET_S_ENTER_GAME = 206,
	PACKET_S_LEAVE_GAME = 207,
	PACKET_S_ITEM_LIST = 208,
	PACKET_S_ADD_ITEM = 209,
	PACKET_C_EQUIP_ITEM = 210,
	PACKET_S_EQUIP_ITEM = 211,
	PACKET_S_SPAWN = 212,
	PACKET_S_DESPAWN = 213,
	PACKET_C_MOVE = 214,
	PACKET_S_MOVE = 215,
	PACKET_C_SKILL = 216,
	PACKET_S_SKILL = 217,
	PACKET_S_CHANGE_HP = 218,
	PACKET_S_CHANGE_STAT = 219,
	PACKET_S_DEATH = 220,
	PACKET_C_CHANGE_STATE = 221,
	PACKET_S_CHANGE_STATE = 222,
	PACKET_C_CHANGE_MOVE_DIR = 223,
	PACKET_S_CHANGE_MOVE_DIR = 224,
	PACKET_C_CHAT = 225,
	PACKET_S_CHAT = 226,
	PACKET_S_PING = 227,
	PACKET_C_PONG = 228,

};

//	Custom Packet Processing Function
bool Packet_Processing_Function_Undefined(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len);
//	Auto
bool S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CONNECTED& packet);
bool S_LOGIN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LOGIN& packet);
bool S_CREATE_PLAYER_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CREATE_PLAYER& packet);
bool S_ENTER_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ENTER_GAME& packet);
bool S_LEAVE_GAME_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_LEAVE_GAME& packet);
bool S_ITEM_LIST_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ITEM_LIST& packet);
bool S_ADD_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_ADD_ITEM& packet);
bool S_EQUIP_ITEM_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_EQUIP_ITEM& packet);
bool S_SPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SPAWN& packet);
bool S_DESPAWN_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DESPAWN& packet);
bool S_MOVE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_MOVE& packet);
bool S_SKILL_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_SKILL& packet);
bool S_CHANGE_HP_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_HP& packet);
bool S_CHANGE_STAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STAT& packet);
bool S_DEATH_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_DEATH& packet);
bool S_CHANGE_STATE_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_STATE& packet);
bool S_CHANGE_MOVE_DIR_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHANGE_MOVE_DIR& packet);
bool S_CHAT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_CHAT& packet);
bool S_PING_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::S_PING& packet);
bool L_S_CONNECTED_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CONNECTED& packet);
bool L_S_CREATE_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_CREATE_ACCOUNT& packet);
bool L_S_LOGIN_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_S_LOGIN_ACCOUNT& packet);


class ServerPacketHandler
{
public:
	static void Initialize()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketPacketProcessingFunction[i] = Packet_Processing_Function_Undefined;

		//	Register Function
		//	Auto
		GPacketPacketProcessingFunction[PACKET_S_CONNECTED] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CONNECTED>(S_CONNECTED_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_LOGIN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_LOGIN>(S_LOGIN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CREATE_PLAYER] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CREATE_PLAYER>(S_CREATE_PLAYER_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_ENTER_GAME] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_ENTER_GAME>(S_ENTER_GAME_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_LEAVE_GAME] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_LEAVE_GAME>(S_LEAVE_GAME_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_ITEM_LIST] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_ITEM_LIST>(S_ITEM_LIST_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_ADD_ITEM] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_ADD_ITEM>(S_ADD_ITEM_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_EQUIP_ITEM] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_EQUIP_ITEM>(S_EQUIP_ITEM_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_SPAWN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_SPAWN>(S_SPAWN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_DESPAWN] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_DESPAWN>(S_DESPAWN_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_MOVE] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_MOVE>(S_MOVE_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_SKILL] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_SKILL>(S_SKILL_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHANGE_HP] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHANGE_HP>(S_CHANGE_HP_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHANGE_STAT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHANGE_STAT>(S_CHANGE_STAT_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_DEATH] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_DEATH>(S_DEATH_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHANGE_STATE] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHANGE_STATE>(S_CHANGE_STATE_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHANGE_MOVE_DIR] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHANGE_MOVE_DIR>(S_CHANGE_MOVE_DIR_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_CHAT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_CHAT>(S_CHAT_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_S_PING] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::S_PING>(S_PING_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_L_S_CONNECTED] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::L_S_CONNECTED>(L_S_CONNECTED_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_L_S_CREATE_ACCOUNT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::L_S_CREATE_ACCOUNT>(L_S_CREATE_ACCOUNT_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_L_S_LOGIN_ACCOUNT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::L_S_LOGIN_ACCOUNT>(L_S_LOGIN_ACCOUNT_Packet_Processing_Function, session, buffer, len); };

	}

	static bool PacketProcessing(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketPacketProcessingFunction[packetHeader->id](session, buffer, len);
	}

public:
	//	Auto
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_LOGIN& packet) { return TMakeSendBuffer(packet, PACKET_C_LOGIN); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CREATE_PLAYER& packet) { return TMakeSendBuffer(packet, PACKET_C_CREATE_PLAYER); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_ENTER_GAME& packet) { return TMakeSendBuffer(packet, PACKET_C_ENTER_GAME); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_EQUIP_ITEM& packet) { return TMakeSendBuffer(packet, PACKET_C_EQUIP_ITEM); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_MOVE& packet) { return TMakeSendBuffer(packet, PACKET_C_MOVE); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_SKILL& packet) { return TMakeSendBuffer(packet, PACKET_C_SKILL); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHANGE_STATE& packet) { return TMakeSendBuffer(packet, PACKET_C_CHANGE_STATE); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHANGE_MOVE_DIR& packet) { return TMakeSendBuffer(packet, PACKET_C_CHANGE_MOVE_DIR); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_CHAT& packet) { return TMakeSendBuffer(packet, PACKET_C_CHAT); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::C_PONG& packet) { return TMakeSendBuffer(packet, PACKET_C_PONG); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::L_C_CREATE_ACCOUNT& packet) { return TMakeSendBuffer(packet, PACKET_L_C_CREATE_ACCOUNT); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::L_C_LOGIN_ACCOUNT& packet) { return TMakeSendBuffer(packet, PACKET_L_C_LOGIN_ACCOUNT); }


private:
	template<typename PacketType, typename ProcessFunction>
	static bool TPacketProcessing(ProcessFunction function, std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketType packet;

		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
		const uint16 dataSize = packetHeader->size - sizeof(PacketHeader);

		if (packet.ParseFromArray(&packetHeader[1], dataSize) == false)
			return false;

		return function(session, packet);
	}

	template<typename T>
	static std::shared_ptr<SendBuffer> TMakeSendBuffer(T& packet, uint16 packetId)
	{
		const uint16 dataSize = static_cast<uint16>(packet.ByteSizeLong());
		const uint16 packetTotalSize = dataSize + sizeof(PacketHeader);

		auto sendBuffer = GSendBufferManager->Open(packetTotalSize);
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		packetHeader->id = packetId;
		packetHeader->size = packetTotalSize;

		ASSERT(packet.SerializeToArray(&packetHeader[1], dataSize));

		sendBuffer->Close(packetTotalSize);
		return sendBuffer;
	}
};