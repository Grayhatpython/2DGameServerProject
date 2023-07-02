#pragma once
#include "DataSheet.h"
#include "JsonParser.h"

class Texture;
class DataManager
{
public:
	static void Clear();

public:
	static bool	LoadDatas();
	static bool	LoadSkillData(const std::string& fileName);
	static bool LoadAnimationData(const std::string& fileName);
	static bool LoadItemData(const std::string& fileName);

public:
	static bool LoadAnimationData(const rapidjson::Value& animDates, Texture* texture);
	static GameData::ItemData* FindItemData(int32 dataSheetId);

public:
	static const GameData::SkillData& FindSkillData(int32 skillId);

public:
	static std::unordered_map<int32, GameData::SkillData>		_skills;
	static std::unordered_map<int32, GameData::ItemData*>		_items;
};


