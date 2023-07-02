#include "pch.h"
#include "GUIInventoryEquippedItem.h"
#include "AssetManager.h"

#include "Item.h"
#include "DataManager.h"

GUIInventoryEquippedItem::GUIInventoryEquippedItem(const std::wstring& name)
	: GUIScreen(name)
{

}

GUIInventoryEquippedItem::~GUIInventoryEquippedItem()
{
	_item = nullptr;
	_itemTexture = nullptr;
}

void GUIInventoryEquippedItem::Initialize()
{

}

void GUIInventoryEquippedItem::Render(const Vector2Int& cameraOffset)
{
	if (_itemTexture && _item)
	{
		if(_item->GetEquipped())
			GUIScreen::DrawTexture(_itemTexture, _position);
		else
		{
			_itemTexture = nullptr;
			_item = nullptr;
		}
	}
}

void GUIInventoryEquippedItem::Update(float deltaTime)
{

}

void GUIInventoryEquippedItem::SetItemTexture(Item* item)
{
	ASSERT(item);

	_item = item;

	auto itemData = DataManager::FindItemData(item->GetDataSheetId());
	ASSERT(itemData);

	_itemTexture = GAssetManager->GetTexture(itemData->imagePath);
	ASSERT(_itemTexture);
}
