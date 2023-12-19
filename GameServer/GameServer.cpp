#include "pch.h"
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
#include "DBConnectionPool.h"

#include "Region.h"

//	TEMP
void NetworkTask(std::shared_ptr<ServerService>& serverService)
{
	//	������ ������ �ִ�..
	//	���� JobBaseObject Flush�� ó���� ����ϴ� ������� 1:1�����̴�..
	//	JobBaseObject�� �ϰ��� ���� �����忡�� Push�ϸ� �ϰ��� ó���ϴ� ������ ȥ�ڼ� �̰� �� ó���ؾ� �ϹǷ�
	//	��, �ϰ��� ��� �и��鼭 ������ ó���ð� �������� ���� �Ҷ� ����� ���� �߻� 

	//	�ٲ� ����
	if (serverService)
	{
		//LEndTickCount = ::GetTickCount64() + WORK_CHECK_TICK;

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
			serverInfo.set_name(u8"���̾�ĭ");
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
	//	Process Recv And Process Send
	for (int32 i = 0; i < 4; i++)
	{
		std::wstring name = L"IocpWorkerThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, [&]() {
				NetworkTask(_serverService);
			});
	}

	//	Game Logic Update And BroadCast Send Queue Push
	for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"Region" + std::to_wstring(i + 1) + L"UpdateThread " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		auto region = GRegionManager->FindRegionFromId(1);
		GThreadManager->Launch(name, [region]() {
				if (region)
				{
					region->KeepExecute();
					region->Update();

					std::this_thread::yield();
				}
			});
	}

	//	All Session Register Send
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

	//	��Ƽ ������� DB ó�� �� ���� �����̶� ���� ��� ó���ұ..
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

	//	��.. �Ź� ����� �ϰ���..
	//	Ŭ���̾�Ʈ���� ������ ��� �ִ��� �ֱ������� ��Ŷ ����...
	/*for (int32 i = 0; i < 1; i++)
	{
		std::wstring name = L"RunningSchedule " + std::to_wstring(i);
		_networkThreadName.push_back(name);
		GThreadManager->Launch(name, []() {
				GClientSessionManager->BroadcastHeartbeat();
				std::this_thread::sleep_for(500ms);
			});
	}*/

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
}
