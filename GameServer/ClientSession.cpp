#include "pch.h"
#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include "ClientSessionManager.h"

#include "Region.h"
#include "Player.h"
#include "ActorManager.h"
#include "RegionManager.h"
#include "DataManager.h"

#include "Item.h"
#include "Inventory.h"

#include "DBHelper.h"
#include "DBConnectionPool.h"


void ClientSession::OnConnected()
{
	auto session = std::static_pointer_cast<ClientSession>(shared_from_this());
	GClientSessionManager->AddClientSession(session);

	std::wcout << L"Client OnConnected" << std::endl;

	{
		//	���� ���� ��Ŷ ����
		Protocol::S_CONNECTED connectedSendPacket;
		connectedSendPacket.set_name("GameServer");
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(connectedSendPacket);
		Send(sendBuffer);
	}
}

void ClientSession::OnRecvPacket(BYTE* buffer, int32 len)
{
	auto session = GetPacketSession();
	//	�л� ������ ���
	//	��Ŷ Id�� �뿪�� Ȯ�� �� �ٸ� �������� �Դ� ������ Ȥ�� Ŭ���̾�Ʈ���� �Դ� ������
	//	�и��ؼ� ó���� �ʿ䰡 �ִ�.
	PacketHeader* packetHeader = reinterpret_cast<PacketHeader*>(buffer);

	ClientPacketHandler::PacketProcessing(session, buffer, len);

}

void ClientSession::OnSend(int32 len)
{

}

void ClientSession::OnDisconnected()
{

	std::wcout << L"Client OnDisconnected" << std::endl;

	{
		//	TEMP
		//	ActorManager�� �÷��̾ �����ִ�... ó������..
		//	�ٵ� Leave�� ���� ������� �𸣱� ������ 
		//	Leave�������� �� Session�� �������� �ȵȴ�
		//	�� Session�� ������ ���� �ִ°� ActorManager
		//	ó���� ����� �޸𸮰� �Ƚ��δ�.
		if (_myPlayer)
		{
			auto region = _myPlayer->GetRegion();
			auto id = _myPlayer->GetId();
			region->OnlyPushJobAndNotDistribute(&Region::Leave, id);
			/*region->OnlyPushJobAndNotDistribute([id]() {
					GActorManager->RemoveActor(id);
				});*/
			_myPlayer = nullptr;
		}
	}

	GClientSessionManager->RemoveClientSession(std::static_pointer_cast<ClientSession>(shared_from_this()));
}

void ClientSession::Login(Protocol::C_LOGIN& loginPacket)
{
	if (_clientGameState != Protocol::ClientGameState::CLIENT_STATE_LOGIN)
		return;

	_lobbyPlayerInfos.clear();

	auto uniqueId = UtilityHelper::ConvertUTF8ToUnicode(loginPacket.uniqueid());
	std::wcout << uniqueId << std::endl;

	{
		//	TODO : DB���� ���� ���� �� ����
		//	���� üũ
		//	���� id
		int32	authId = 0;
		bool	isDummyClient = false;

		//	���� Ŭ���̾�Ʈ�� ��Ʈ���� �׽�Ʈ�� ���� ���� ����
		if (uniqueId.find(L"DummyClient") == wstring::npos)
		{
			auto dbConnection = GDBConnectionPool->Pop(L"MasterDB");

			dbConnection->UnBind();

			DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_SelectAccountToken ?, ?");
			auto accountId = loginPacket.accountid();
			auto token = loginPacket.token();
			dbHepler.BindParam(0, accountId);
			dbHepler.BindParam(1, token);
			dbHepler.BindCol(0, authId);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				;

			GDBConnectionPool->Push(L"MasterDB", dbConnection);
		}
		else
			isDummyClient = true;

		//	���� ����
		if (isDummyClient || authId > 0)
		{
			//	TODO : DB���� ���� ���� �� ����
			//	���� üũ
			auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");
			dbConnection->UnBind();

			DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_SelectAccountId ?");
			auto name = UtilityHelper::ConvertUTF8ToUnicode(loginPacket.uniqueid());

			dbHepler.BindParam(0, name.c_str());
			int32 accountDbId = 0;
			dbHepler.BindCol(0, accountDbId);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				_accountDbId = accountDbId;

			//	������ �ִ� ���
			if (_accountDbId > 0)
			{
				DBHelper<1, 8> dbHepler(dbConnection, L"EXEC usp_SelectPlayers ?");
				dbHepler.BindParam(0, name.c_str());
				WCHAR playerName[32] = {0,};

				int32 playerId = 0;
				int32 level = 0;
				int32 hp = 0;
				int32 maxHp = 0;
				int32 attack = 0;
				float speed = 0.f;
				int32 totalExp = 0;

				dbHepler.BindCol(0, playerId);
				dbHepler.BindCol(1, playerName);
				dbHepler.BindCol(2, level);
				dbHepler.BindCol(3, hp);
				dbHepler.BindCol(4, maxHp);
				dbHepler.BindCol(5, attack);
				dbHepler.BindCol(6, speed);
				dbHepler.BindCol(7, totalExp);

				ASSERT(dbHepler.Execute());

				//	��ġ����... �ִ� ��ҿ� ���� �ִٸ�..?
				Protocol::LobbyPlayerInfo lobbyPlayerInfo;
				while (dbHepler.Fetch())
				{
					lobbyPlayerInfo.set_playerdbid(playerId);
					lobbyPlayerInfo.set_name(UtilityHelper::ConvertUnicodeToUTF8(playerName));
					lobbyPlayerInfo.mutable_statinfo()->set_level(level);
					lobbyPlayerInfo.mutable_statinfo()->set_hp(hp);
					lobbyPlayerInfo.mutable_statinfo()->set_maxhp(maxHp);
					lobbyPlayerInfo.mutable_statinfo()->set_attack(attack);
					lobbyPlayerInfo.mutable_statinfo()->set_speed(speed);
					lobbyPlayerInfo.mutable_statinfo()->set_totalexp(totalExp);
					_lobbyPlayerInfos.push_back(lobbyPlayerInfo);
				}
			}
			//	������ ���� ���
			else
			{
				DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_CreateAccount ?");
				auto name = UtilityHelper::ConvertUTF8ToUnicode(loginPacket.uniqueid());
				dbHepler.BindParam(0, name.c_str());
				dbHepler.BindCol(0, accountDbId);

				ASSERT(dbHepler.Execute());
				while (dbHepler.Fetch())
					_accountDbId = accountDbId;
			}

			GDBConnectionPool->Push(L"GameServerDB", dbConnection);

			Protocol::S_LOGIN loginSendPacket;
			loginSendPacket.set_loginok(1);
			for (auto& lobbyPlayerInfo : _lobbyPlayerInfos)
			{
				auto addLobbyPlayerInfo = loginSendPacket.add_lobbyplayers();
				addLobbyPlayerInfo->MergeFrom(lobbyPlayerInfo);
			}

			auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginSendPacket);
			Send(sendBuffer);

			_clientGameState = Protocol::ClientGameState::CLIENT_STATE_LOBBY;
		}
		//	���� ����..
		else
		{
			//	..	�α����� �ʴ� ������?
		}
	}
}

void ClientSession::CreatePlayer(Protocol::C_CREATE_PLAYER& createPlayerPacket)
{
	if (_clientGameState != Protocol::ClientGameState::CLIENT_STATE_LOBBY)
		return;

	{
		auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");
		dbConnection->UnBind();

		//	������ �̸��� �ش��ϴ� �÷��̾ �ִ���...
		DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_SelectPlayerId ?");
		auto name = UtilityHelper::ConvertUTF8ToUnicode(createPlayerPacket.name());
		dbHepler.BindParam(0, name.c_str());

		int32 playerDbId = 0;
		int32 outPlayerDbId = 0;
		dbHepler.BindCol(0, outPlayerDbId);
		ASSERT(dbHepler.Execute());
		while (dbHepler.Fetch())
			playerDbId = outPlayerDbId;

		Protocol::S_CREATE_PLAYER createPlayerSendPacket;
		if (playerDbId > 0)
			createPlayerSendPacket.set_createok(0);
		else
		{
			const auto& statData = DataManager::FindStatData(1);
			DBHelper<8, 1> dbHepler(dbConnection, L"EXEC usp_InsertPlayer ?, ?, ?, ?, ?, ?, ?, ?");

			int32 level = statData.level();
			int32 hp = statData.hp();
			int32 maxHp = statData.maxhp();
			int32 attack = statData.attack();
			float speed = statData.speed();
			int32 totalExp = statData.totalexp();

			dbHepler.BindParam(0, name.c_str());
			dbHepler.BindParam(1, level);
			dbHepler.BindParam(2, hp);
			dbHepler.BindParam(3, maxHp);
			dbHepler.BindParam(4, attack);
			dbHepler.BindParam(5, speed);
			dbHepler.BindParam(6, totalExp);
			dbHepler.BindParam(7, _accountDbId);

			dbHepler.BindCol(0, outPlayerDbId);
			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				playerDbId = outPlayerDbId;

			{
				Protocol::LobbyPlayerInfo lobbyPlayerInfo;
				lobbyPlayerInfo.set_playerdbid(playerDbId);
				lobbyPlayerInfo.set_name(createPlayerPacket.name());
				lobbyPlayerInfo.mutable_statinfo()->set_level(level);
				lobbyPlayerInfo.mutable_statinfo()->set_hp(hp);
				lobbyPlayerInfo.mutable_statinfo()->set_maxhp(maxHp);
				lobbyPlayerInfo.mutable_statinfo()->set_attack(attack);
				lobbyPlayerInfo.mutable_statinfo()->set_speed(speed);
				lobbyPlayerInfo.mutable_statinfo()->set_totalexp(totalExp);
				_lobbyPlayerInfos.push_back(lobbyPlayerInfo);
	
				createPlayerSendPacket.set_createok(1);
				createPlayerSendPacket.mutable_createplayer()->MergeFrom(lobbyPlayerInfo);
			}
		}

		GDBConnectionPool->Push(L"GameServerDB", dbConnection);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(createPlayerSendPacket);
		Send(sendBuffer);
	}
}

void ClientSession::EnterGame(Protocol::C_ENTER_GAME& enterGamePacket)
{
	if (_clientGameState != Protocol::ClientGameState::CLIENT_STATE_LOBBY)
		return;

	auto lobbyPlayer = std::find_if(_lobbyPlayerInfos.begin(), _lobbyPlayerInfos.end(), [&name = enterGamePacket.name()](const Protocol::LobbyPlayerInfo& lobbyPlayerInfo)
		{
			return name.compare(lobbyPlayerInfo.name()) == 0;
		});

	if (lobbyPlayer == _lobbyPlayerInfos.end())
		return;

	{
		auto session = std::static_pointer_cast<ClientSession>(shared_from_this());

		//	�κ�â���� ĳ���� ���� 
		_myPlayer = GActorManager->AddActor<Player>();
		_myPlayer->SetPlayerDbId(lobbyPlayer->playerdbid());
		_myPlayer->SetName(lobbyPlayer->name());
		Protocol::PositionInfo positionInfo;
		positionInfo.set_positionx(144.f);
		positionInfo.set_positiony(144.f);
		positionInfo.set_movedir(Protocol::MoveDir::DOWN);
		positionInfo.set_state(Protocol::AIState::IDLE);
		positionInfo.set_usedskillid(0);

		//	TODO
		_myPlayer->SetInventory(std::make_shared<Inventory>());

		_myPlayer->SetPositionInfo(positionInfo);
		_myPlayer->SetStatInfo(lobbyPlayer->statinfo());
		_myPlayer->SetSession(session);
	}

	{
		//	DB Item Info Load
		auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");
		dbConnection->UnBind();

		//	������ �̸��� �ش��ϴ� �÷��̾ �ִ���...
		DBHelper<1, 5> dbHepler(dbConnection, L"EXEC usp_SelectItems ?");
		int32 playerDbId = lobbyPlayer->playerdbid();
		dbHepler.BindParam(0, playerDbId);

		int32 itemDbId = 0;
		int32 dataSheedId = 0;
		int32 count = 0;
		int32 slot = 0;
		bool equipped = false;

		dbHepler.BindCol(0, itemDbId);
		dbHepler.BindCol(1, dataSheedId);
		dbHepler.BindCol(2, count);
		dbHepler.BindCol(3, slot);
		dbHepler.BindCol(4, equipped);
		ASSERT(dbHepler.Execute());

		Protocol::S_ITEM_LIST itemListSendPacket;
		while (dbHepler.Fetch())
		{
			auto addItemInfo = itemListSendPacket.add_iteminfos();
			addItemInfo->set_itemdbid(itemDbId);
			addItemInfo->set_datasheetid(dataSheedId);
			addItemInfo->set_count(count);
			addItemInfo->set_slot(slot);
			addItemInfo->set_equipped(equipped);

			auto item = Item::MakeItem(addItemInfo);
			_myPlayer->GetInventory()->AddItem(item);
		}

		GDBConnectionPool->Push(L"GameServerDB", dbConnection);

		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(itemListSendPacket);
		Send(sendBuffer);
	}

	{
		//	Ŭ���̾�Ʈ���� ĳ���� ���� �� ���� ���� ��Ŷ�� �� ��
		auto region = GRegionManager->FindRegionFromId(1);

		//	�ִ� DB���� ��ġ���� �����;� �Ѵ�.
		//	�ϴ� ���� ��ġ������
		region->OnlyPushJobAndNotDistribute(&Region::Enter, std::static_pointer_cast<Actor>(_myPlayer), true);

		_clientGameState = Protocol::ClientGameState::CLIENT_STATE_GAME;
	}
}

void ClientSession::Ping()
{
	if (_heartbeatTick > 0)
	{
		auto diff = ::GetTickCount64() - _heartbeatTick;
		if (diff > 30000)
		{
			Disconnect(L"Disconnected By No Response");
			return;
		}
	}

	Protocol::S_PING pingSendPacket;
	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(pingSendPacket);
	Send(sendBuffer);

}

void ClientSession::Pong()
{
	_heartbeatTick = ::GetTickCount64();
}
