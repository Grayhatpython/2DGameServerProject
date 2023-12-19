#include "pch.h"
#include "NetworkManager.h"

#include "Thread.h"
#include "SocketUtils.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "NetworkService.h"
#include "ServerPacketHandler.h"

NetworkManager* GNetworkManager = nullptr;

NetworkManager::NetworkManager()
{

}

NetworkManager::~NetworkManager()
{
	SocketUtils::Clear();
}

bool NetworkManager::Initialize()
{
	ServerPacketHandler::Initialize();

	//	Main ThreadId
	LThreadId = ::GetCurrentThreadId();

	_clientService = MakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 9999),			//	Login Server
		//NetworkAddress(L"127.0.0.1", 8888),		//	Game Server
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,	//	TODO : SessionManger
		1
		);

	if (_clientService == nullptr)
		return false;

	return true;
}

void NetworkManager::Start()
{
	ASSERT(_clientService->Start());

	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"NetworkThread" + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, [&]() {
				if(_clientService)
					_clientService->GetIocpCore()->Dispatch(10);
		});
	}
}

//	�α��� ���� -> ���� ���� ����
void NetworkManager::ConnectToGameServer(const std::wstring& ip, int32 port)
{

	{
		//	�α��� ���� ���� ����
		GNetworkManager->SetClientGameState(Protocol::ClientGameState::CLIENT_STATE_GAME);
		auto serverSession = GetServerSession();
		ASSERT(serverSession);
		serverSession->Disconnect(L"�α��� ���� -> �α��� ���� ���� ����");

		//	Main Thread Wait
		std::this_thread::sleep_for(200ms);
	}

	//	��Ŀ ������ ���� ó��
	GThreadManager->Clear();
	_networkThreadName.clear();

	_clientService->Close();
	_clientService = nullptr;

	_clientService = MakeShared<ClientService>(
		NetworkAddress(ip.c_str(), port),
		MakeShared<IocpCore>(),
		MakeShared<ServerSession>,	//	TODO : SessionManger
		1
		);

	ASSERT(_clientService->Start());

	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"NetworkThread" + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, [&]() {
			if (_clientService)
			_clientService->GetIocpCore()->Dispatch(10);
			});
	}
}

void NetworkManager::Clear()
{
	//	���� ��Ŷ ó��
	PrcoessPackets();

	//	������ ���� ����
	auto serverSession = GetServerSession();
	if (serverSession)
		serverSession->Disconnect(L"������������");

	//	���� ���� ó�� Wait
	std::this_thread::sleep_for(200ms);

	//	��Ŀ ������ ���� ó��
	GThreadManager->Clear();
	_networkThreadName.clear();

	//	��Ŀ ������ ���� ��ٸ�

	//	���� ������ SendBuffer Clear
	//	TEMP
	{
		//	Main
		LCanReturnSendBufferChunk = false;
		LSendBufferChunk = nullptr;
	}

	//	���� ����
	_clientService->Close();
}

void NetworkManager::Send(std::shared_ptr<SendBuffer> sendBuffer)
{
	auto serverSession = GetServerSession();
	ASSERT(serverSession);

	if (serverSession)
		serverSession->Send(sendBuffer, false);
}

void NetworkManager::PushRecvPacket(BYTE* buffer, int32 len)
{
	WRITE_LOCK;
	auto recvPacketInfo = ObjectPool<RecvPacketInfo>::MakeShared(buffer, len);
	_recvPacketInfos.push(recvPacketInfo);
}

void NetworkManager::PrcoessPackets()
{
	Vector<std::shared_ptr<RecvPacketInfo>> recvPacketInfos;
	
	{
		WRITE_LOCK;

		while (_recvPacketInfos.empty() == false)
		{
			auto recvPcaketInfo = _recvPacketInfos.front();
			recvPacketInfos.push_back(recvPcaketInfo);
			_recvPacketInfos.pop();

		}
	}

	if (recvPacketInfos.size() == 0)
		return;

	for (auto& recvPacket : recvPacketInfos)
	{
		PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(recvPacket->buffer);
		
		auto serverSession = GetServerSession();
		ASSERT(serverSession);

		ServerPacketHandler::PacketProcessing(serverSession, recvPacket->buffer, recvPacket->len);
	}
}
