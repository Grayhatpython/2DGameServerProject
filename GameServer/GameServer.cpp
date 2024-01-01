#include "pch.h"

#include <Mmsystem.h>

#include "GameServer.h"
#include "Acceptor.h"
#include "NetworkService.h"
#include "ClientSession.h"
#include "ClientSessionManager.h"
#include "ClientPacketHandler.h"
#include "DistributeJobBaseObject.h"
#include "ThreadManager.h"
#include "JobTimer.h"
#include "ActorManager.h"
#include "RegionManager.h"
#include "ConfigManager.h"
#include "DataManager.h"
#include "DBProcess.h"
#include "RegionProcess.h"
#include "DBConnectionPool.h"
#include "PerformanceCounter.h"

#include "Region.h"

//	TEMP
void NetworkTask(std::shared_ptr<ServerService>& serverService)
{
	//	여전히 문제는 있다..
	//	현재 JobBaseObject Flush를 처리를 담당하는 스레드는 1:1관계이다..
	//	JobBaseObject에 일감이 여러 스레드에서 Push하면 일감을 처리하는 스레드 혼자서 이걸 다 처리해야 하므로
	//	즉, 일감이 계속 밀리면서 일종의 처리시간 지연으로 인해 뚝뚝 끊기는 렉이 발생 

	//	바꿀 예정
	if (serverService)
	{
		serverService->GetIocpCore()->Dispatch(TIME_OUT);

		//	TODO : Function
		/*const uint64 currentTick = ::GetTickCount64();

		GJobTimer->Execute(currentTick);*/

		//DistributeJobBaseObject::Distribute();
	}
}

GameServer::GameServer()
{

}

GameServer::~GameServer()
{
	LSendBufferChunk = nullptr;
	LCanReturnSendBufferChunk = false;

	GThreadManager->Clear();
	std::this_thread::sleep_for(200ms);

	//	TEMP
	GJobTimer->Clear();

	//	TEMP
	if (GDBProcess)
	{
		delete GDBProcess;
		GDBProcess = nullptr;
	}
	if (GActorManager)
	{
		delete GActorManager;
		GActorManager = nullptr;
	}
	if (GRegionManager)
	{
		delete GRegionManager;
		GRegionManager = nullptr;
	}
	if (GClientSessionManager)
	{
		delete GClientSessionManager;
		GClientSessionManager = nullptr;
	}

	DataManager::Clear();
	_serverService->Close();

}

bool GameServer::Initialize()
{
	LThreadId = ::GetCurrentThreadId();

	{
		ClientPacketHandler::Initialize();
		ASSERT(ConfigManager::LoadConfig());

		{
			ASSERT(DataManager::LoadDatas());
			Protocol::ServerInfo serverInfo;
			serverInfo.set_name(u8"하이아칸");
			serverInfo.set_ip(u8"127.0.0.1");
			serverInfo.set_port(8888);
			serverInfo.set_complexity(0);

			DataManager::_serverInfos.push_back(serverInfo);
		}

		ASSERT(GDBConnectionPool->Initialize(5, L"GameServerDB", ConfigManager::serverConfig.dbConnectionString.c_str()));
		ASSERT(GDBConnectionPool->Initialize(1, L"MasterDB", L"Driver={SQL Server Native Client 11.0};Server=DESKTOP-3O8GGTF\\SQLEXPRESS;Database=MasterDB;Trusted_Connection=Yes;"));
	}

	{
		GDBProcess = new DBProcess();
		GRegionManager = new RegionManager();
		GActorManager = new ActorManager();
		GClientSessionManager = new ClientSessionManager();
		GRegionManager->Add();
	}

	_serverService = MakeShared<ServerService>(
		NetworkAddress(L"127.0.0.1", 8888),
		MakeShared<IocpCore>(),
		MakeShared<ClientSession>,	//	TODO : SessionManger
		100
		);

	bool successed = _serverService->Start();
	std::this_thread::sleep_for(100ms);
	return successed;
}

void GameServer::Start()
{

	/*
	//	음.. 매번 실행될 일감들..
	//	클라이언트와의 연결이 살아 있는지 주기적으로 패킷 전달...
	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"RunningSchedule " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, []() {
				GClientSessionManager->BroadcastHeartbeat();
				std::this_thread::sleep_for(500ms);
			});
	}
	
	//	F2 key -> Server Exit
	while (true)
	{
		SHORT keyState = GetKeyState(VK_F2);
		bool isToggled = keyState & 1;
		bool isDown = keyState & 0x8000;

		if (isDown)
			break;

		::Sleep(50);
	}
	*/

	//	Process Recv ( Packet Push ) And Process Send
	for (int32 i = 0; i < 4; i++)
	{
		std::wstring name = L"IocpWorkerThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, [&]() {
				NetworkTask(_serverService);
			});
	}

	//	DB Process 
	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"DBProcessThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, []() {
				GDBProcess->UpdateGameServerInfo();
				GDBProcess->KeepExecute();
				std::this_thread::yield();
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
				std::this_thread::yield();
			});
	}

	Update();
}

void GameServer::Update()
{
	LARGE_INTEGER frequency, startTickCount, currentTickCount;
	int64 loopTickCount = 0, millisecondTickCount = 0;
	int32 framePerSecond = 0;

	::timeBeginPeriod(1);
	::QueryPerformanceFrequency(&frequency);
	millisecondTickCount = frequency.QuadPart / 1000;
	loopTickCount = frequency.QuadPart / UPDATE_FRAME_TIME_TICK;

	::QueryPerformanceCounter(&startTickCount);
	int64 sleepTime = 0;

	uint64 frameStartTick = ::GetTickCount64();
	uint64 frameEndTick = ::GetTickCount64();
	uint64 frameCurrentTick = ::GetTickCount64();

	//	Main Thread -> 30Frame or 60frame
	while (true)
	{
		if (frameEndTick - frameStartTick >= 1000)
		{
			
			::OutputDebugString(L"FPS : ");
			::OutputDebugString(std::to_wstring(framePerSecond).c_str());
			::OutputDebugString(L"\n");
			

			frameStartTick = frameEndTick;
			framePerSecond = 0;
		}

		auto region = GRegionManager->FindRegionFromId(1);
			
		//	Recv Packet Process 
		region->Execute();

		float deltaTime = (frameEndTick - frameCurrentTick) / 1000.0f;
		/*::OutputDebugString(std::to_wstring(deltaTime).c_str());
		::OutputDebugString(L"\n");*/

		frameCurrentTick = ::GetTickCount64();

		//	Game Logic Update
		region->Update(deltaTime);

		auto regionProcess = GRegionManager->GetRegionProcess();
		regionProcess->Execute();

		/*
		//	Packet Send
		for (int32 i = 0; i < UPDATE_THREAD_COUNT; i++)
		{
			auto sessions = GClientSessionManager->GetSessions();
			auto sessionCount = sessions.size();
			Vector<std::shared_ptr<ClientSession>> clientSessions;

			for (auto& session : sessions)
				clientSessions.push_back(session);

			std::vector<std::future<int32>> futures;
			for (int32 i = 0; i < UPDATE_THREAD_COUNT; i++)
			{
				futures.emplace_back(_sendThreadPool.Enqueue([&clientSessions, sessionCount, i]()
					{
						auto standardClientUpdateCount = sessionCount / UPDATE_THREAD_COUNT;
						Vector<std::shared_ptr<ClientSession>>::iterator start = clientSessions.begin() + (i * standardClientUpdateCount);
						Vector<std::shared_ptr<ClientSession>>::iterator end;
						if (i == UPDATE_THREAD_COUNT - 1)
							end = clientSessions.end();
						else
							end = clientSessions.begin() + ((i + 1) * standardClientUpdateCount);

						for (auto iter = start; iter != end; ++iter)
							(*iter)->FlushSend();

						return 1;
					}));
			}

			int32 processUpdateThreadCount = 0;
			for (auto& f : futures) {
				processUpdateThreadCount += f.get();
			}

			ASSERT(processUpdateThreadCount == UPDATE_THREAD_COUNT);
		}
		*/

		/*auto sessions = GClientSessionManager->GetSessions();
		for (auto& session : sessions)
			session->FlushSend();*/

		::QueryPerformanceCounter(&currentTickCount);
		sleepTime = (startTickCount.QuadPart + loopTickCount - currentTickCount.QuadPart) / millisecondTickCount;
		/*::OutputDebugString(std::to_wstring(sleepTime).c_str());
		::OutputDebugString(L"\n");*/

		startTickCount.QuadPart += loopTickCount;
		if (currentTickCount.QuadPart - startTickCount.QuadPart > loopTickCount)
			startTickCount = currentTickCount;
		else
			std::this_thread::sleep_for(std::chrono::milliseconds(sleepTime));

		framePerSecond++;
		frameEndTick = ::GetTickCount64();

	}

	::timeEndPeriod(1);
}
