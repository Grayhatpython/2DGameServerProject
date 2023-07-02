#pragma once

class Item;
class Inventory
{
public:
	void					AddItem(std::shared_ptr<Item> item);
	std::shared_ptr<Item>	GetItem(int32 itemDbId);
	std::shared_ptr<Item>	FindItem(std::function<bool(const std::shared_ptr<Item>&)> condition);

public:
	int32										GetEmptySlot();
	const HashMap<int32, std::shared_ptr<Item>> GetItems() { return _items; }

private:
	HashMap<int32, std::shared_ptr<Item>>	_items;
	HashMap<int32, Item*>					_slotKeyItemsTable;
};

