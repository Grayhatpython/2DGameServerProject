#include "pch.h"
#include "LoginServer.h"
#include "ClientSession.h"
#include "NetworkService.h"
#include "DBConnectionPool.h"
#include "ThreadManager.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"

#include "DBHelper.h"
LoginServer::LoginServer()
{
}

LoginServer::~LoginServer()
{
	if (GClientSessionManager)
	{
		delete GClientSessionManager;
		GClientSessionManager = nullptr;
	}

	_serverService->Close();
}

bool LoginServer::Initialize()
{
    LThreadId = ::GetCurrentThreadId();

	ClientPacketHandler::Initialize();
	GClientSessionManager = new ClientSessionManager();

	_serverService = MakeShared<ServerService>(
		NetworkAddress(L"127.0.0.1", 9999),
		MakeShared<IocpCore>(),
		MakeShared<ClientSession>,	//	TODO : SessionManger
		100
		);

	return _serverService->Start();
}

void LoginServer::Start()
{
	ASSERT(GDBConnectionPool->Initialize(1, L"MasterDB", L"Driver={SQL Server Native Client 11.0};Server=DESKTOP-3O8GGTF\\SQLEXPRESS;Database=MasterDB;Trusted_Connection=Yes;"));
	ASSERT(GDBConnectionPool->Initialize(1, L"AccountDB", L"Driver={SQL Server Native Client 11.0};Server=DESKTOP-3O8GGTF\\SQLEXPRESS;Database=AccountDB;Trusted_Connection=Yes;"));

	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"IocpWorkerThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, [&]() {
				_serverService->GetIocpCore()->Dispatch(10);
			});
	}

	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"FlushSendThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, []() {
			auto sessions = GClientSessionManager->GetSessions();
				for (auto& session : sessions)
					session->FlushSend();
				sessions.clear();
				std::this_thread::yield();
			});
	}

	while (true)
	{
		SHORT keyState = GetKeyState(VK_F1);
		bool isToggled = keyState & 1;
		bool isDown = keyState & 0x8000;

		if (isDown)
		{
			LSendBufferChunk = nullptr;
			LCanReturnSendBufferChunk = false;

			GThreadManager->Clear();
			std::this_thread::sleep_for(100ms);
			break;
		}

		::Sleep(50);
	}
}
