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

//	로그인 서버 -> 게임 서버 연결
void NetworkManager::ConnectToGameServer(const std::wstring& ip, int32 port)
{

	{
		//	로그인 서버 연결 종료
		GNetworkManager->SetClientGameState(Protocol::ClientGameState::CLIENT_STATE_GAME);
		auto serverSession = GetServerSession();
		ASSERT(serverSession);
		serverSession->Disconnect(L"로그인 성공 -> 로그인 서버 연결 종료");

		//	Main Thread Wait
		std::this_thread::sleep_for(200ms);
	}

	//	워커 스레드 종료 처리
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
	//	남은 패킷 처리
	PrcoessPackets();

	//	서버와 연결 종료
	auto serverSession = GetServerSession();
	if (serverSession)
		serverSession->Disconnect(L"서버연결종료");

	//	연결 종료 처리 Wait
	std::this_thread::sleep_for(200ms);

	//	워커 스레드 종료 처리
	GThreadManager->Clear();
	_networkThreadName.clear();

	//	워커 스레드 종료 기다림

	//	메인 스레드 SendBuffer Clear
	//	TEMP
	{
		//	Main
		LCanReturnSendBufferChunk = false;
		LSendBufferChunk = nullptr;
	}

	//	서비스 종료
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
