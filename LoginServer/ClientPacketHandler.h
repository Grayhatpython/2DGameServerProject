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
bool L_C_CREATE_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_C_CREATE_ACCOUNT& packet);
bool L_C_LOGIN_ACCOUNT_Packet_Processing_Function(std::shared_ptr<PacketSession>& session, Protocol::L_C_LOGIN_ACCOUNT& packet);


class ClientPacketHandler
{
public:
	static void Initialize()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketPacketProcessingFunction[i] = Packet_Processing_Function_Undefined;

		//	Register Function
		//	Auto
		GPacketPacketProcessingFunction[PACKET_L_C_CREATE_ACCOUNT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::L_C_CREATE_ACCOUNT>(L_C_CREATE_ACCOUNT_Packet_Processing_Function, session, buffer, len); };
		GPacketPacketProcessingFunction[PACKET_L_C_LOGIN_ACCOUNT] = [](std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len) { return TPacketProcessing<Protocol::L_C_LOGIN_ACCOUNT>(L_C_LOGIN_ACCOUNT_Packet_Processing_Function, session, buffer, len); };

	}

	static bool PacketProcessing(std::shared_ptr<PacketSession>& session, BYTE* buffer, int32 len)
	{
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketPacketProcessingFunction[packetHeader->id](session, buffer, len);
	}

public:
	//	Auto
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::L_S_CONNECTED& packet) { return TMakeSendBuffer(packet, PACKET_L_S_CONNECTED); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::L_S_CREATE_ACCOUNT& packet) { return TMakeSendBuffer(packet, PACKET_L_S_CREATE_ACCOUNT); }
	static std::shared_ptr<SendBuffer> MakeSendBuffer(Protocol::L_S_LOGIN_ACCOUNT& packet) { return TMakeSendBuffer(packet, PACKET_L_S_LOGIN_ACCOUNT); }


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