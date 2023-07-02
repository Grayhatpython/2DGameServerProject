#include "pch.h"
#include "Inventory.h"
#include "Item.h"

//	Remove...
void Inventory::AddItem(std::shared_ptr<Item> item)
{
	_items.insert(std::make_pair(item->GetItemDbId(), item));
	_slotKeyItemsTable.insert(std::make_pair(item->GetSlot(), item.get()));
}

std::shared_ptr<Item> Inventory::GetItem(int32 itemDbId)
{
	auto iter = _items.find(itemDbId);
	if (iter != _items.end())
		return iter->second;

	return nullptr;
}

std::shared_ptr<Item> Inventory::FindItem(std::function<bool(const std::shared_ptr<Item>&)> condition)
{
	for (auto& item : _items)
	{
		if (condition(item.second))
			return item.second;
	}

	return nullptr;
}

int32 Inventory::GetEmptySlot()
{
	//	TEMP 45는 너무 하드코딩
	for (int32 slot = 0; slot < 45; slot++)
	{
		auto iter = _slotKeyItemsTable.find(slot);
		if (iter == _slotKeyItemsTable.end())
			return slot;
	}

	//	TODO
	return -1;
}
