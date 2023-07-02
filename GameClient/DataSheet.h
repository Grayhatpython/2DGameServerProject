#pragma once
#include "Types.h"

namespace GameData
{
	//	À½.. ±âÂú³×
	struct MonsterData
	{
		int32						id = 0;
		std::wstring				name;
		//Protocol::StatInfo			statInfo;
		std::string					imagePath;
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
		std::string			imagePath;
		Protocol::ItemType	itemType = Protocol::ItemType::ITEM_TYPE_NONE;
	};

	struct WeaponData : public ItemData
	{
		WeaponData()
			: ItemData(Protocol::ItemType::ITEM_TYPE_WEAPON)
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

