#include "pch.h"
#include "StatComponent.h"

#include "Item.h"
#include "InventoryManager.h"

COMPONENT_TYPE_DEFINITION(Component, StatComponent);

StatComponent::StatComponent()
{
}

StatComponent::~StatComponent()
{
}

//	���ʹ� �ָ��ϴ�.. ������Ʈ�� �����ؾߴ볪
void StatComponent::ReCalculationAdditionalStat()
{
	_weaponDamage = 0;
	_armorDefence = 0;

	const auto& items = GInventoryManager->GetItems();
	for (const auto& item : items)
	{
		if (item.second->GetEquipped() == false)
			continue;

		switch (item.second->GetItemType())
		{
		case Protocol::ItemType::ITEM_TYPE_WEAPON:
		{
			auto weapon =static_cast<Weapon*>(item.second);
			_weaponDamage += weapon->GetDamage();
		}
		break;
		case Protocol::ItemType::ITEM_TYPE_ARMOR:
		{
			auto armor = static_cast<Armor*>(item.second);
			_armorDefence += armor->GetDefence();
		}
		break;
		}
	}
}

