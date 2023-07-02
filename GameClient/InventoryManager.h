#pragma once

class Item;
class GUIInventory;
class InventoryManager
{
public:
	InventoryManager();
	~InventoryManager();

public:
	void							AddItem(Item* item);
	Item*							GetItem(int32 itemDbId);
	const HashMap<int32, Item*>&	GetItems() { return _items; }

public:
	void			Clear();

private:
	const InventoryManager& operator = (const InventoryManager&) = delete;
	InventoryManager(const InventoryManager&) = delete;

private:
	HashMap<int32, Item*>	_items;

};

extern InventoryManager* GInventoryManager;