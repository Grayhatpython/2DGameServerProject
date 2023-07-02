#pragma once
#include "Types.h"

namespace GameData
{
	struct RewardData
	{
		int32 probability = 0;	// 확률
		int32 itemId = 0;	//	아이템 데이터 시트 Id
		int32 count = 0;
	};

	struct MonsterData
	{
		int32						id = 0;
		std::wstring				name;
		Protocol::StatInfo			statInfo;
		std::vector<RewardData>		rewardDatas;
	};

	struct ProjectilInfo
	{
		std::wstring			name;
		float					speed = 0.f;
		int32					range = 0;
	};

	struct SkillData
	{
		int32					id = 0;
		std::wstring			name;
		float					coolDown = 0.f;
		int32					damage = 0;
		Protocol::SkillType		type = Protocol::SkillType::SKILL_NONE;
		ProjectilInfo			projectileInfo;
	};

	struct ItemData
	{
		ItemData(Protocol::ItemType itemType)
			: itemType(itemType)
		{

		}

		//	dataSheetId
		int32				id = 0;
		std::wstring		name;
		Protocol::ItemType	itemType = Protocol::ItemType::ITEM_TYPE_NONE;
	};

	struct WeaponData : public ItemData
	{
		WeaponData()
			:	ItemData(Protocol::ItemType::ITEM_TYPE_WEAPON)
		{

		}

		Protocol::WeaponType weaponType = Protocol::WeaponType::WEAPON_TYPE_NONE;
		int32	damage = 0;
	};

	struct ArmorData : public ItemData
	{
		ArmorData()
			: ItemData(Protocol::ItemType::ITEM_TYPE_ARMOR)
		{

		}

		Protocol::ArmorType armorType = Protocol::ArmorType::ARMOR_TYPE_NONE;
		int32	defence = 0;
	};

	struct ConsumableData : public ItemData
	{
		ConsumableData()
			: ItemData(Protocol::ItemType::ITEM_TYPE_CONSUMABLE)
		{

		}

		Protocol::ConsumableType consumableType = Protocol::ConsumableType::CONSUMABLE_TYPE_NONE;
		int32	maxCount = 0;
	};
}

