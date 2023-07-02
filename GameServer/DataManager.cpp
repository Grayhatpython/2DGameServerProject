#include "pch.h"
#include "DataManager.h"
#include "JsonParser.h"
#include "ConfigManager.h"

std::unordered_map<int32, Protocol::StatInfo>			DataManager::_stats;
std::unordered_map<int32, GameData::MonsterData>		DataManager::_monsters;
std::unordered_map<int32, GameData::SkillData>			DataManager::_skills;
std::unordered_map<int32, GameData::ItemData*>			DataManager::_items;
std::vector<Protocol::ServerInfo>						DataManager::_serverInfos;

void DataManager::Clear()
{
	for (auto& item : _items)
		delete item.second;
}

bool DataManager::LoadDatas()
{
	std::string path = ConfigManager::serverConfig.dataPath;
	std::string statDataPath = path + "/StatData.json";
	std::string monsterStatDataPath = path + "/MonsterData.json";
	std::string skillDataPath = path + "/SkillData.json";
	std::string itemDataPath = path + "/ItemData.json";

	ASSERT(LoadStatData(statDataPath));
	ASSERT(LoadMonsterData(monsterStatDataPath));
	ASSERT(LoadSkillData(skillDataPath));
	ASSERT(LoadItemData(itemDataPath));

	return true;
}

bool DataManager::LoadStatData(const std::string& fileName)
{
	rapidjson::Document doc;
	JsonParser::ParseFromFile(fileName.c_str(), doc);

	const rapidjson::Value& stats = doc["stats"];
	if (stats.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < stats.Size(); i++)
		{
			const rapidjson::Value& statValue = stats[i];
			if (statValue.IsObject())
			{
				Protocol::StatInfo stat;
				stat.set_level(JsonParser::GetInt(statValue, "level"));
				stat.set_hp(JsonParser::GetInt(statValue, "maxHp"));
				stat.set_maxhp(JsonParser::GetInt(statValue, "maxHp"));
				stat.set_attack(JsonParser::GetInt(statValue, "attack"));
				stat.set_totalexp(JsonParser::GetInt(statValue, "totalExp"));
				stat.set_speed(JsonParser::GetFloat(statValue, "speed"));
				_stats.insert(std::make_pair(stat.level(), stat));
			}
		}
	}

	return true;
}

bool DataManager::LoadMonsterData(const std::string& fileName)
{
	rapidjson::Document doc;
	JsonParser::ParseFromFile(fileName.c_str(), doc);

	const rapidjson::Value& monstersData = doc["monsters"];
	if (monstersData.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < monstersData.Size(); i++)
		{
			const rapidjson::Value& monsterValue = monstersData[i];
			if (monsterValue.IsObject())
			{
				GameData::MonsterData monster;
				monster.id = JsonParser::GetInt(monsterValue, "dataSheetId");
				monster.name = JsonParser::GetWString(monsterValue, "name");

				const rapidjson::Value& statValue = monsterValue["stat"];

				if (statValue.IsObject())
				{
					Protocol::StatInfo stat;
					stat.set_level(JsonParser::GetInt(statValue, "level"));
					stat.set_hp(JsonParser::GetInt(statValue, "maxHp"));
					stat.set_maxhp(JsonParser::GetInt(statValue, "maxHp"));
					stat.set_attack(JsonParser::GetInt(statValue, "attack"));
					stat.set_totalexp(JsonParser::GetInt(statValue, "totalExp"));
					stat.set_speed(JsonParser::GetFloat(statValue, "speed"));
					monster.statInfo.MergeFrom(stat);
				}

				const rapidjson::Value& rewardsValue = monsterValue["rewardDatas"];
				if (rewardsValue.IsArray())
				{
					for (rapidjson::SizeType i = 0; i < rewardsValue.Size(); i++)
					{
						const rapidjson::Value& rewardValue = rewardsValue[i];

						if (rewardValue.IsObject())
						{
							GameData::RewardData reward;
							reward.probability = JsonParser::GetInt(rewardValue, "probability");
							reward.itemId = JsonParser::GetInt(rewardValue, "itemId");
							reward.count = JsonParser::GetInt(rewardValue, "count");
							monster.rewardDatas.push_back(reward);
						}
					}
				}

				_monsters.insert(std::make_pair(monster.id, monster));
			}
		}
	}

	return true;
}

bool DataManager::LoadSkillData(const std::string& fileName)
{
	rapidjson::Document doc;
	JsonParser::ParseFromFile(fileName.c_str(), doc);

	const rapidjson::Value& skills = doc["skills"];
	if (skills.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < skills.Size(); i++)
		{
			const rapidjson::Value& skillValue = skills[i];
			if (skillValue.IsObject())
			{
				GameData::SkillData skill;
				skill.id = JsonParser::GetInt(skillValue, "id");
				skill.name = JsonParser::GetWString(skillValue, "name");
				skill.coolDown = JsonParser::GetFloat(skillValue, "coolDown");
				skill.damage = JsonParser::GetInt(skillValue, "damage");
				auto skillType = static_cast<Protocol::SkillType>(JsonParser::GetInt(skillValue, "id"));
				skill.type = skillType;

				//	투사제 공격
				if (skillType == Protocol::SkillType::SKILL_PROJECTILE_ATTACK)
				{
					const rapidjson::Value& projectileValue = skills[i]["projectileInfo"];

					GameData::ProjectilInfo projectileInfo;
					projectileInfo.name = JsonParser::GetWString(projectileValue, "name");
					projectileInfo.speed = JsonParser::GetFloat(projectileValue, "speed");
					projectileInfo.range = JsonParser::GetInt(projectileValue, "range");
					skill.projectileInfo = projectileInfo;
				}

				_skills.insert(std::make_pair(skill.id, skill));
			}
		}
	}

	return true;
}

bool DataManager::LoadItemData(const std::string& fileName)
{
	rapidjson::Document doc;
	JsonParser::ParseFromFile(fileName.c_str(), doc);

	const rapidjson::Value& weapons = doc["weapons"];
	if (weapons.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < weapons.Size(); i++)
		{
			const rapidjson::Value& weaponValue = weapons[i];
			if (weaponValue.IsObject())
			{
				GameData::WeaponData* weaponData = new GameData::WeaponData();
				weaponData->id = JsonParser::GetInt(weaponValue, "id");
				weaponData->name = JsonParser::GetWString(weaponValue, "name");
				auto type = JsonParser::GetWString(weaponValue, "weaponType");
				if (type.compare(L"Sword") == 0)
					weaponData->weaponType = Protocol::WeaponType::WEAPON_TYPE_SWORD;
				else if (type.compare(L"Book") == 0)
					weaponData->weaponType = Protocol::WeaponType::WEAPON_TYPE_BOOK;
				else
					ASSERT(false);

				weaponData->damage = JsonParser::GetInt(weaponValue, "damage");

				_items.insert(std::make_pair(weaponData->id, weaponData));
			}
		}
	}

	const rapidjson::Value& armors = doc["armors"];
	if (armors.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < armors.Size(); i++)
		{
			const rapidjson::Value& armorValue = armors[i];
			if (armorValue.IsObject())
			{
				GameData::ArmorData* armorData = new GameData::ArmorData();
				armorData->id = JsonParser::GetInt(armorValue, "id");
				armorData->name = JsonParser::GetWString(armorValue, "name");
				auto type = JsonParser::GetWString(armorValue, "armorType");
				if (type.compare(L"Helmet") == 0)
					armorData->armorType = Protocol::ArmorType::ARMOR_TYPE_HELMET;
				else if (type.compare(L"Armor") == 0)
					armorData->armorType = Protocol::ArmorType::ARMOR_TYPE_ARMOR;
				else if(type.compare(L"Boots") == 0)
					armorData->armorType = Protocol::ArmorType::ARMOR_TYPE_BOOTS;
				else
					ASSERT(false);

				armorData->defence = JsonParser::GetInt(armorValue, "defence");

				_items.insert(std::make_pair(armorData->id, armorData));
			}
		}
	}

	const rapidjson::Value& consumables = doc["consumables"];
	if (consumables.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < consumables.Size(); i++)
		{
			const rapidjson::Value& consumableValue = consumables[i];
			if (consumableValue.IsObject())
			{
				GameData::ConsumableData* consumableData = new GameData::ConsumableData();
				consumableData->id = JsonParser::GetInt(consumableValue, "id");
				consumableData->name = JsonParser::GetWString(consumableValue, "name");
				auto type = JsonParser::GetWString(consumableValue, "consumableType");
				if (type.compare(L"Portion") == 0)
					consumableData->consumableType = Protocol::ConsumableType::CONSUMABLE_TYPE_PORTION;
				else
					ASSERT(false);

				consumableData->maxCount = JsonParser::GetInt(consumableValue, "maxCount");

				_items.insert(std::make_pair(consumableData->id, consumableData));
			}
		}
	}
	return true;
}

const Protocol::StatInfo& DataManager::FindStatData(int32 level)
{
	auto iter = _stats.find(level);
	if (iter != _stats.end())
		return iter->second;

	//	TODO
	return Protocol::StatInfo{};
}

const GameData::MonsterData& DataManager::FindMonsterData(int32 id)
{
	auto iter = _monsters.find(id);
	if (iter != _monsters.end())
		return iter->second;

	//	TODO
	return GameData::MonsterData{};
}

const GameData::SkillData& DataManager::FindSkillData(int32 skillId)
{
	auto iter = _skills.find(skillId);
	if (iter != _skills.end())
		return iter->second;

	//	TODO
	return GameData::SkillData{};
}

GameData::ItemData* DataManager::FindItemData(int32 dataSheetId)
{
	auto iter = _items.find(dataSheetId);
	if (iter != _items.end())
		return iter->second;

	//	TODO
	return nullptr;
}
