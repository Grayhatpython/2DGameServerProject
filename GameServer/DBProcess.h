#pragma once
#include "DataSheet.h"
class Region;
class Item;
class Player;
//	TEMP
class DBProcess : public JobBaseObject
{
public:
	DBProcess();
	~DBProcess();

public:
	void UpdatePlayerHp(std::shared_ptr<Player> player);
	void InsertRewardItemToPlayer(std::shared_ptr<Player> player, const GameData::RewardData& rewardData);
	void UpdatePlayerItemEquipped(std::shared_ptr<Player> player, std::shared_ptr<Item> item);

public:
	//	TEMP
	void UpdateGameServerInfo();

private:
	const DBProcess& operator = (const DBProcess&) = delete;
	DBProcess(const DBProcess&) = delete;

private:

};

extern DBProcess* GDBProcess;
