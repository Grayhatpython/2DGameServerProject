#include "pch.h"
#include "DummyClient.h"
#include "Thread.h"
#include "SocketUtils.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "NetworkService.h"
#include "ServerPacketHandler.h"
#include "ServerSessionManager.h"

DummyClient* GDummyClient = nullptr;

DummyClient::DummyClient()
{

}

DummyClient::~DummyClient()
{
	Clear();
	SocketUtils::Clear();
}

bool DummyClient::Initialize()
{
	ServerPacketHandler::Initialize();

	GServerSessionManager = new ServerSessionManager();

	//	Main ThreadId
	LThreadId = ::GetCurrentThreadId();

	_clientService = MakeShared<ClientService>(
		NetworkAddress(L"127.0.0.1", 8888),
		MakeShared<IocpCore>(),
			[]() {
				return GServerSessionManager->AddServerSession();
			},	//	TODO : SessionManger
		300
		);

	if (_clientService == nullptr)
		return false;

	return true;
}

void DummyClient::Start()
{
	ASSERT(_clientService->Start());

	for (int32 i = 0; i < 2; i++)
	{
		std::wstring name = L"NetworkThread" + std::to_wstring(i);
		_workerThreadNames.push_back(name);
		GThreadManager->Launch(name, [&]() {
			if (_clientService != nullptr)
			_clientService->GetIocpCore()->Dispatch(10);
			});
	}

	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"FlushSend" + std::to_wstring(i);
		_workerThreadNames.push_back(name);
		GThreadManager->Launch(name, []() {
			auto sessions = GServerSessionManager->GetSessions();
				for (auto& session : sessions)
					session->FlushSend();
				std::this_thread::yield();
			});
	}

	while (true)
	{
		SHORT keyState = GetKeyState(VK_F3);
		bool isToggled = keyState & 1;
		bool isDown = keyState & 0x8000;

		if (isDown)
			break;

		::Sleep(50);
	}
}

void DummyClient::Clear()
{
	GServerSessionManager->Clear();

	//	워커 스레드 종료 처리
	GThreadManager->Clear();
	std::this_thread::sleep_for(200ms);

	//	메인 스레드 SendBuffer Clear
	//	TEMP
	{
		//	Main
		LCanReturnSendBufferChunk = false;
		LSendBufferChunk = nullptr;
	}

	if (GServerSessionManager)
	{
		delete GServerSessionManager;
		GServerSessionManager = nullptr;
	}

	//	서비스 종료
	_clientService->Close();
}