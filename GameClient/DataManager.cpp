#include "pch.h"
#include "DataManager.h"
#include "AssetManager.h"
#include "AnimationManager.h"

#include "Texture.h"

std::unordered_map<int32, GameData::SkillData>	DataManager::_skills;
std::unordered_map<int32, GameData::ItemData*>	DataManager::_items;

void DataManager::Clear()
{
	_skills.clear();

	for (auto& item : _items)
		delete item.second;

	_items.clear();
}

bool DataManager::LoadDatas()
{
	//	TODO
	std::string path = "../Common/GameData/DataSheet";
	std::string skillDataPath = path + "/SkillData.json";
	std::string animationDataPath = path + "/AnimationData.json";
	std::string itemDataPath = path + "/ItemData.json";

	ASSERT(LoadSkillData(skillDataPath));
	ASSERT(LoadAnimationData(animationDataPath));
	ASSERT(LoadItemData(itemDataPath));

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

bool DataManager::LoadAnimationData(const std::string& fileName)
{
	rapidjson::Document doc;
	JsonParser::ParseFromFile(fileName.c_str(), doc);

	auto playerIdleMoveTexture = GAssetManager->GetTexture("../Assets/Player/Player_Move.png");
	auto playerSkillTexture = GAssetManager->GetTexture("../Assets/Player/Player_Skill.png");
	auto MonsterTexture = GAssetManager->GetTexture("../Assets/Monster/Monster.png");
	auto fireBallMoveTexture = GAssetManager->GetTexture("../Assets/Projectile/Ball.png");
	auto ballDestoryEffectTexture = GAssetManager->GetTexture("../Assets/Effects/BallDestory.png");
	auto explosionEffectTexture = GAssetManager->GetTexture("../Assets/Effects/Explosion.png");
	auto hitEffectTexture = GAssetManager->GetTexture("../Assets/Effects/Hit.png");
	auto shadowEffectTexture = GAssetManager->GetTexture("../Assets/Effects/Shadow.png");
	auto sweepEffectTexture = GAssetManager->GetTexture("../Assets/Effects/Sweep.png");

	::SDL_SetTextureBlendMode(shadowEffectTexture->GetSDLTexture(), SDL_BLENDMODE_BLEND);
	::SDL_SetTextureAlphaMod(shadowEffectTexture->GetSDLTexture(), 170);
	::SDL_SetTextureBlendMode(hitEffectTexture->GetSDLTexture(), SDL_BLENDMODE_BLEND);
	::SDL_SetTextureAlphaMod(hitEffectTexture->GetSDLTexture(), 200);

	LoadAnimationData(doc["player_idle"], playerIdleMoveTexture);
	LoadAnimationData(doc["player_move"], playerIdleMoveTexture);
	LoadAnimationData(doc["player_skill"], playerSkillTexture);
	LoadAnimationData(doc["monster_idle"], MonsterTexture);
	LoadAnimationData(doc["monster-move"], MonsterTexture);
	LoadAnimationData(doc["monster-skill"], MonsterTexture);
	LoadAnimationData(doc["fireball_move"], fireBallMoveTexture);
	LoadAnimationData(doc["fireball_destory_effect"], ballDestoryEffectTexture);
	LoadAnimationData(doc["actor_explosion_effect"], explosionEffectTexture);
	LoadAnimationData(doc["target_hit_effect"], hitEffectTexture);
	LoadAnimationData(doc["actor_shadow_effect"], shadowEffectTexture);
	LoadAnimationData(doc["player_auto_attack_sweep_effect"], sweepEffectTexture);

	return true;
}

bool DataManager::LoadAnimationData(const rapidjson::Value& animDates, Texture* texture)
{
	std::wstring animName;
	AnimSpriteInfo animInfo;

	if (animDates.IsArray())
	{
		for (rapidjson::SizeType i = 0; i < animDates.Size(); i++)
		{
			const rapidjson::Value& animData = animDates[i];
			if (animData.IsObject())
			{
				animInfo.texture = texture;
				animName = JsonParser::GetWString(animData, "name");
				animInfo.width = JsonParser::GetInt(animData, "width");
				animInfo.height = JsonParser::GetInt(animData, "height");
				animInfo.standardXIndex = JsonParser::GetInt(animData, "standardXIndex");
				animInfo.standardYIndex = JsonParser::GetInt(animData, "standardYIndex");
				animInfo.maxFrameCount = JsonParser::GetInt(animData, "maxFrameCount");
				animInfo.repeat = JsonParser::GetBool(animData, "repeat");
				animInfo.animDir = static_cast<AnimSpriteDirection>(JsonParser::GetInt(animData, "animDir"));
				auto flipData = JsonParser::GetInt(animData, "flip");
				if (flipData == 1)
					animInfo.flip = SDL_FLIP_HORIZONTAL;
				else
					animInfo.flip = SDL_FLIP_NONE;

				GAnimationManager->AddAnimInfo(animName, animInfo);
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
				weaponData->imagePath = JsonParser::GetString(weaponValue, "imagePath");
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
				armorData->imagePath = JsonParser::GetString(armorValue, "imagePath");
				auto type = JsonParser::GetWString(armorValue, "armorType");
				if (type.compare(L"Helmet") == 0)
					armorData->armorType = Protocol::ArmorType::ARMOR_TYPE_HELMET;
				else if (type.compare(L"Armor") == 0)
					armorData->armorType = Protocol::ArmorType::ARMOR_TYPE_ARMOR;
				else if (type.compare(L"Boots") == 0)
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
				consumableData->imagePath = JsonParser::GetString(consumableValue, "imagePath");
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

