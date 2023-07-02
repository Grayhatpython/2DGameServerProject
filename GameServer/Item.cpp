#include "pch.h"
#include "Item.h"
#include "DataManager.h"

Item::Item(const Protocol::ItemType& itemType)
	: _itemType(itemType)
{

}

Item::~Item()
{

}

std::shared_ptr<Item> Item::MakeItem(Protocol::ItemInfo* itemInfo)
{
	auto itemData = DataManager::FindItemData(itemInfo->datasheetid());
	ASSERT(itemData);

	auto item = CreateItem(itemData->itemType);

	ASSERT(item);
	item->Initialize(itemInfo->itemdbid(), itemInfo->datasheetid());
	item->SetCount(itemInfo->count());
	item->SetSlot(itemInfo->slot());
	item->SetEquipped(itemInfo->equipped());

	return item;
}

std::shared_ptr<Item> Item::MakeItem(const Protocol::ItemInfo& itemInfo)
{
	auto itemData = DataManager::FindItemData(itemInfo.datasheetid());
	ASSERT(itemData);

	auto item = CreateItem(itemData->itemType);

	ASSERT(item);
	item->Initialize(itemInfo.itemdbid(), itemInfo.datasheetid());
	item->SetCount(itemInfo.count());
	item->SetSlot(itemInfo.slot());
	item->SetEquipped(itemInfo.equipped());

	return item;
}

std::shared_ptr<Item> Item::CreateItem(const Protocol::ItemType& type)
{
	std::shared_ptr<Item> item;

	switch (type)
	{
	case Protocol::ItemType::ITEM_TYPE_WEAPON:
		item = ObjectPool<Weapon>::MakeShared();
		break;
	case Protocol::ItemType::ITEM_TYPE_ARMOR:
		item = ObjectPool<Armor>::MakeShared();
		break;
	case Protocol::ItemType::ITEM_TYPE_CONSUMABLE:
		item = ObjectPool<Consumable>::MakeShared();
		break;
	}

	return item;
}

Weapon::Weapon()
	: Item(Protocol::ItemType::ITEM_TYPE_WEAPON)
{

}

Weapon::~Weapon()
{

}

void Weapon::Initialize(int32 itemDbId, int32 dataSheetId)
{
	auto itemData = DataManager::FindItemData(dataSheetId);
	ASSERT(itemData);
	ASSERT(itemData->itemType == Protocol::ItemType::ITEM_TYPE_WEAPON);

	GameData::WeaponData* weaponData = static_cast<GameData::WeaponData*>(itemData);
	SetItemDbId(itemDbId);
	SetDataSheetId(dataSheetId);
	SetWeaponType(weaponData->weaponType);
	SetDamage(weaponData->damage);
	SetCount(1);
	SetStackable(false);
}

Armor::Armor()
	: Item(Protocol::ItemType::ITEM_TYPE_ARMOR)
{

}

Armor::~Armor()
{

}

void Armor::Initialize(int32 itemDbId, int32 dataSheetId)
{
	auto itemData = DataManager::FindItemData(dataSheetId);
	ASSERT(itemData);
	ASSERT(itemData->itemType == Protocol::ItemType::ITEM_TYPE_ARMOR);

	GameData::ArmorData* armorData = static_cast<GameData::ArmorData*>(itemData);
	SetItemDbId(itemDbId);
	SetDataSheetId(dataSheetId);
	SetArmorType(armorData->armorType);
	SetDefence(armorData->defence);
	SetCount(1);
	SetStackable(false);
}

Consumable::Consumable()
	: Item(Protocol::ItemType::ITEM_TYPE_CONSUMABLE)
{

}

Consumable::~Consumable()
{

}

void Consumable::Initialize(int32 itemDbId, int32 dataSheetId)
{
	auto itemData = DataManager::FindItemData(dataSheetId);
	ASSERT(itemData);
	ASSERT(itemData->itemType == Protocol::ItemType::ITEM_TYPE_CONSUMABLE);

	GameData::ConsumableData* consumableData = static_cast<GameData::ConsumableData*>(itemData);
	SetItemDbId(itemDbId);
	SetDataSheetId(dataSheetId);
	SetConsumableType(consumableData->consumableType);
	SetMaxCount(consumableData->maxCount);
	SetCount(1);
	SetStackable(consumableData->maxCount > 1 ? true : false);
}
