#include "pch.h"
#include "InventoryManager.h"
#include "Item.h"

InventoryManager* GInventoryManager = nullptr;

InventoryManager::InventoryManager()
{
}

InventoryManager::~InventoryManager()
{
	Clear();
}

void InventoryManager::AddItem(Item* item)
{
	_items.insert(std::make_pair(item->GetItemDbId(), item));
}

Item* InventoryManager::GetItem(int32 itemDbId)
{
	auto iter = _items.find(itemDbId);
	if (iter != _items.end())
		return iter->second;

	return nullptr;
}

void InventoryManager::Clear()
{
	for (auto& item : _items)
		cdelete(item.second);

	_items.clear();
}
