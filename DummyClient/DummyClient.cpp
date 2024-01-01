#include "pch.h"
#include "DummyClient.h"
#include "Thread.h"
#include "SocketUtils.h"
#include "ThreadManager.h"
#include "ServerSession.h"
#include "NetworkService.h"
#include "ServerPacketHandler.h"
#include "ServerSessionManager.h"

#include "MyPlayer.h"

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
		400
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

	const uint64 moveTick = 7000;
	uint64 startTick = ::GetTickCount64();
	uint64 movingTick = 0;
	auto frameTick = ::GetTickCount64();
	bool isMoving = false;

	while (true)
	{
		while (true)
		{
			if (::GetTickCount64() - frameTick >= 33)
				break;
		}

		auto sessions = GServerSessionManager->GetSessions();

		if (::GetTickCount64() - startTick >= moveTick)
		{
			startTick = ::GetTickCount64();
			movingTick = startTick;
			isMoving = true;

			for (auto& session : sessions)
			{
				auto serverSession = std::static_pointer_cast<ServerSession>(session);
				auto myPlayer = serverSession->GetMyPlayer();
				if (myPlayer)
				{
					auto randomValue = Random::GetIntRange(0, 3);
					auto moveDir = Protocol::MoveDir(randomValue);
					auto position = myPlayer->GetPositionInfo();
					auto forwardPosition = myPlayer->GetForwardPosition(moveDir);

					
					//Protocol::C_CHANGE_MOVE_DIR changeMoveDirSendPacket;
					//changeMoveDirSendPacket.set_movedir(movekeyInputDir);
					//auto moveDirPacketSendBuffer = ServerPacketHandler::MakeSendBuffer(changeMoveDirSendPacket);
					//GNetworkManager->Send(moveDirPacketSendBuffer);

					Protocol::PositionInfo positionInfo;
					positionInfo.set_movedir(moveDir);
					positionInfo.set_positionx(forwardPosition.x);
					positionInfo.set_positiony(forwardPosition.y);
					positionInfo.set_state(Protocol::AIState::MOVE);
					positionInfo.set_usedskillid(0);	//	TODO

					Protocol::C_MOVE movePacket;
					auto posInfo = movePacket.mutable_positioninfo();
					posInfo->MergeFrom(positionInfo);
					auto sendBuffer = ServerPacketHandler::MakeSendBuffer(movePacket);
					serverSession->Send(sendBuffer);

					myPlayer->SetPositionInfo(positionInfo);
				}
			}
		}

		if (isMoving && ::GetTickCount64() - movingTick >= 300)
		{
			isMoving = false;
			for (auto& session : sessions)
			{
				auto serverSession = std::static_pointer_cast<ServerSession>(session);
				Protocol::C_CHANGE_STATE changeStateSendPacket;
				changeStateSendPacket.set_state(Protocol::AIState::IDLE);
				auto sendBuffer = ServerPacketHandler::MakeSendBuffer(changeStateSendPacket);
				serverSession->Send(sendBuffer);
			}
		}
		
		for (auto& session : sessions)
			session->FlushSend();
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