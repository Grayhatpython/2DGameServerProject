#pragma once
#include "DataSheet.h"

class DataManager
{
public:
	static void Clear();

public:
	static bool	LoadDatas();
	static bool	LoadStatData(const std::string& fileName);
	static bool	LoadMonsterData(const std::string& fileName);
	static bool	LoadSkillData(const std::string& fileName);
	static bool LoadItemData(const std::string& fileName);

public:
	static const Protocol::StatInfo& FindStatData(int32 level);
	static const GameData::MonsterData& FindMonsterData(int32 id);
	static const GameData::SkillData& FindSkillData(int32 skillId);
	static GameData::ItemData* FindItemData(int32 dataSheetId);

public:
	static std::unordered_map<int32, Protocol::StatInfo>			_stats;
	static std::unordered_map<int32, GameData::MonsterData>			_monsters;
	static std::unordered_map<int32, GameData::SkillData>			_skills;
	static std::unordered_map<int32, GameData::ItemData*>			_items;

	//	TODO
	static std::vector<Protocol::ServerInfo>						_serverInfos;
};

