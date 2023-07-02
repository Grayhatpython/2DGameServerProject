#pragma once

class Item
{
public:
	Item(const Protocol::ItemType& itemType);
	virtual ~Item();

public:
	static Item*	MakeItem(const Protocol::ItemInfo& itemInfo);

public:
	virtual void				Initialize(int32 itemDbId, int32 dataSheetId) abstract;

public:
	void						SetItemInfo(const Protocol::ItemInfo& itemInfo) { _itemInfo.CopyFrom(itemInfo); }
	const Protocol::ItemInfo&	GetItemInfo() const { return _itemInfo; }

	void						SetItemDbId(int32 id) { _itemInfo.set_itemdbid(id); }
	int32						GetItemDbId() { return _itemInfo.itemdbid(); }

	void						SetDataSheetId(int32 id) { _itemInfo.set_datasheetid(id); }
	int32						GetDataSheetId() { return _itemInfo.datasheetid(); }

	void						SetCount(int32 count) { _itemInfo.set_count(count); }
	int32						GetCount() { return _itemInfo.count(); }

	void						SetSlot(int32 slot) { _itemInfo.set_slot(slot); }
	int32						GetSlot() { return _itemInfo.slot(); }

	void						SetEquipped(bool equipped) { _itemInfo.set_equipped(equipped); }
	bool						GetEquipped() { return _itemInfo.equipped(); }

	void						SetItemType(const Protocol::ItemType& itemType) { _itemType = itemType; }
	Protocol::ItemType			GetItemType() { return _itemType; }

	void						SetStackable(bool stackable) { _stackable = stackable; }
	bool						GetStackable() { return _stackable; }

private:
	Protocol::ItemInfo	_itemInfo;
	Protocol::ItemType	_itemType = Protocol::ItemType::ITEM_TYPE_NONE;

	//	중복 적재
	bool				_stackable = false;
};

class Weapon : public Item
{
public:
	Weapon();
	virtual ~Weapon();

public:
	virtual void Initialize(int32 itemDbId, int32 dataSheetId) override;

public:
	void					SetWeaponType(const Protocol::WeaponType& weaponType) { _weaponType = weaponType; }
	Protocol::WeaponType	GetWeaponType() { return _weaponType; }

	void					SetDamage(int32 damage) { _damage = damage; }
	int32					GetDamage() { return _damage; }

private:
	Protocol::WeaponType _weaponType = Protocol::WeaponType::WEAPON_TYPE_NONE;
	int32	_damage = 0;
};

class Armor : public Item
{
public:
	Armor();
	virtual ~Armor();

public:
	virtual void Initialize(int32 itemDbId, int32 dataSheetId) override;

public:
	void					SetArmorType(const Protocol::ArmorType& armorType) { _armorType = armorType; }
	Protocol::ArmorType		GetArmorType() { return _armorType; }

	void					SetDefence(int32 defence) { _defence = defence; }
	int32					GetDefence() { return _defence; }

private:
	Protocol::ArmorType _armorType = Protocol::ArmorType::ARMOR_TYPE_NONE;
	int32	_defence = 0;
};


class Consumable : public Item
{
public:
	Consumable();
	virtual ~Consumable();

public:
	virtual void Initialize(int32 itemDbId, int32 dataSheetId) override;

public:
	void						SetConsumableType(const Protocol::ConsumableType& consumableType) { _consumableType = consumableType; }
	Protocol::ConsumableType	GetConsumableType() { return _consumableType; }

	void						SetMaxCount(int32 maxCount) { _maxCount = maxCount; }
	int32						GetMaxCount() { return _maxCount; }

private:
	Protocol::ConsumableType _consumableType = Protocol::ConsumableType::CONSUMABLE_TYPE_NONE;
	int32	_maxCount = 0;
};
