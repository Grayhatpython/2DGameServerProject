#include "pch.h"
#include "DBProcess.h"
#include "Region.h"
#include "Player.h"

#include "Item.h"
#include "Inventory.h"

#include "DBHelper.h"
#include "DBConnectionPool.h"

#include "ClientSession.h"
#include "ClientPacketHandler.h"
#include "ClientSessionManager.h"

#include "DataManager.h"

DBProcess* GDBProcess = nullptr;

DBProcess::DBProcess()
{

}

DBProcess::~DBProcess()
{

}

void DBProcess::UpdatePlayerHp(std::shared_ptr<Player> player)
{

	ASSERT(player);
	auto region = player->GetRegion();
	ASSERT(region);

	{
		//	Ref 증가
		OnlyPushJobAndNotDistribute([player]() {

			auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");
				
			//	TODO
			dbConnection->UnBind();

			DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_UpdatePlayerHp ?, ?");
			int32 hp = player->GetHp();

			if (hp == 0)
				hp = player->GetMaxHp();

			int32 playerDbId = player->GetPlayerDbId();
			dbHepler.BindParam(0, playerDbId);
			dbHepler.BindParam(1, hp);

			int32 rowCount = 0;
			dbHepler.BindCol(0, rowCount);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				rowCount;

			GDBConnectionPool->Push(L"GameServerDB", dbConnection);

			if (rowCount)
			{
				auto region = player->GetRegion();
				if (region)
					region->OnlyPushJob([hp]() {
							std::wcout << "Hp Saved : " << hp << std::endl;
						});
			}
			//	실패 시 ...?
			else
			{
				;
			}
		});
	}
}

void DBProcess::InsertRewardItemToPlayer(std::shared_ptr<Player> player, const GameData::RewardData& rewardData)
{
	ASSERT(player);
	auto region = player->GetRegion();
	ASSERT(region);

	{
		//	Ref 증가할까 -> 음.. 고민중
		OnlyPushJobAndNotDistribute([player, rewardData]() {

			auto emptySlot = player->GetInventory()->GetEmptySlot();
			if (emptySlot < 0)
				return;

			auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");

			//	TODO
			dbConnection->UnBind();

			DBHelper<4, 1> dbHepler(dbConnection, L"EXEC usp_InsertItem ?, ?, ?, ?");
			int32 dataSheetId = rewardData.itemId;
			int32 count = rewardData.count;
			int32 slot = emptySlot;
			int32 playerDbId = player->GetPlayerDbId();

			dbHepler.BindParam(0, dataSheetId);
			dbHepler.BindParam(1, count);
			dbHepler.BindParam(2, slot);
			dbHepler.BindParam(3, playerDbId);

			int32 itemDbId = 0;
			dbHepler.BindCol(0, itemDbId);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				itemDbId;

			GDBConnectionPool->Push(L"GameServerDB", dbConnection);

			if (itemDbId)
			{
				auto region = player->GetRegion();
				ASSERT(region);
				region->OnlyPushJob([dataSheetId, count, slot, itemDbId, player]() {
						Protocol::ItemInfo itemInfo;
						itemInfo.set_datasheetid(dataSheetId);
						itemInfo.set_count(count);
						itemInfo.set_slot(slot);
						itemInfo.set_itemdbid(itemDbId);
						itemInfo.set_equipped(false);
						auto newItem = Item::MakeItem(itemInfo);
						auto inventory = player->GetInventory();
						ASSERT(inventory);
						inventory->AddItem(newItem);

						Protocol::S_ADD_ITEM addItemSendPacket;
						auto newItemInfos = addItemSendPacket.add_iteminfos();
						newItemInfos->MergeFrom(itemInfo);
						auto sendBuffer = ClientPacketHandler::MakeSendBuffer(addItemSendPacket);
						auto session = player->GetSession();
						ASSERT(session);

						session->Send(sendBuffer);
					});

			}
			//	실패 시 ...?
			else
			{
				;
			}
		});
	}
}

void DBProcess::UpdatePlayerItemEquipped(std::shared_ptr<Player> player, std::shared_ptr<Item> item)
{
	ASSERT(player);
	auto region = player->GetRegion();
	ASSERT(region);
	ASSERT(item);

	{
		//	Ref 증가할까 -> 음.. 고민중
		OnlyPushJobAndNotDistribute([player, item]() {

			auto dbConnection = GDBConnectionPool->Pop(L"GameServerDB");
			//	TODO
			dbConnection->UnBind();

			DBHelper<2, 1> dbHepler(dbConnection, L"EXEC usp_UpdateItemEquipped ?, ?");
			int32 itemDbId = item->GetItemDbId();
			int32 equipped = item->GetEquipped();

			dbHepler.BindParam(0, itemDbId);
			dbHepler.BindParam(1, equipped);

			int32 rowCount = 0;
			dbHepler.BindCol(0, rowCount);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				rowCount;

			GDBConnectionPool->Push(L"GameServerDB", dbConnection);

			//	성공 시 ...?
			if (rowCount)
			{
				//	원래같으면 여기서 장착 여부 패킷 보내야 한다.
			}
			//	실패 시 ...?
			else
			{
				;
			}
		});
	}
}

void DBProcess::UpdateGameServerInfo()
{
	static uint64 serverInfoUpdateTick = 0;

	if (serverInfoUpdateTick > ::GetTickCount64())
		return;

	serverInfoUpdateTick = ::GetTickCount64() + 30000;

	{
		auto dbConnection = GDBConnectionPool->Pop(L"MasterDB");
		//	TODO
		dbConnection->UnBind();

		DBHelper<1, 1> dbHepler(dbConnection, L"EXEC usp_SelectGameServerId ?");
		auto serverName = UtilityHelper::ConvertUTF8ToUnicode(DataManager::_serverInfos[0].name());
		dbHepler.BindParam(0, serverName.c_str());

		int32 gameServerId = 0;
		dbHepler.BindCol(0, gameServerId);

		ASSERT(dbHepler.Execute());
		while (dbHepler.Fetch())
			gameServerId;

		if (gameServerId > 0)
		{
			dbConnection->UnBind();

			DBHelper<4, 1> dbHepler(dbConnection, L"EXEC usp_UpdateGameServer ?, ?, ?, ?");
			auto serverName = UtilityHelper::ConvertUTF8ToUnicode(DataManager::_serverInfos[0].name());
			auto serverIp = UtilityHelper::ConvertUTF8ToUnicode(DataManager::_serverInfos[0].ip());
			auto port = DataManager::_serverInfos[0].port();
			auto complexity = GClientSessionManager->GetServerComlexity();
			dbHepler.BindParam(0, serverName.c_str());
			dbHepler.BindParam(1, serverIp.c_str());
			dbHepler.BindParam(2, port);
			dbHepler.BindParam(3, complexity);

			int32 success = 0;
			dbHepler.BindCol(0, success);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				success;

			if (success > 0)
			{

			}
			else
			{

			}
		}
		else
		{
			dbConnection->UnBind();

			DBHelper<4, 1> dbHepler(dbConnection, L"EXEC usp_CreateGameServer ?, ?, ?, ?");
			auto serverName = UtilityHelper::ConvertUTF8ToUnicode(DataManager::_serverInfos[0].name());
			auto serverIp = UtilityHelper::ConvertUTF8ToUnicode(DataManager::_serverInfos[0].ip());
			auto port = DataManager::_serverInfos[0].port();
			auto complexity = GClientSessionManager->GetServerComlexity();
			dbHepler.BindParam(0, serverName.c_str());
			dbHepler.BindParam(1, serverIp.c_str());
			dbHepler.BindParam(2, port);
			dbHepler.BindParam(3, complexity);

			//	TEMP
			int32 success = 0;
			dbHepler.BindCol(0, success);

			ASSERT(dbHepler.Execute());
			while (dbHepler.Fetch())
				success;

			if (success > 0)
			{

			}
			else
			{

			}
		}

		GDBConnectionPool->Push(L"MasterDB", dbConnection);
	}
}
