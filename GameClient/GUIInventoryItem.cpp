#include "pch.h"
#include "GUIInventoryItem.h"

#include "Item.h"
#include "Texture.h"
#include "AssetManager.h"
#include "DataManager.h"

#include "NetworkManager.h"
#include "ServerPacketHandler.h"

GUIInventoryItem::GUIInventoryItem(const std::wstring& name)
	:	GUIScreen(name)
{
	_background = GAssetManager->GetTexture("../Assets/GUI/Cell.png");
	_equippedTexture = GAssetManager->GetTexture("../Assets/GUI/Equip.png");
}

GUIInventoryItem::~GUIInventoryItem()
{
	_item = nullptr;
	_itemTexture = nullptr;
	_equippedTexture = nullptr;
}

void GUIInventoryItem::Initialize()
{

}

void GUIInventoryItem::Render(const Vector2Int& cameraOffset)
{
	if (_background)
		GUIScreen::DrawTexture(_background, _position);

	if (_item && _item->GetEquipped())
		GUIScreen::DrawTexture(_equippedTexture, _position);

	//	TEMP
	if(_itemTexture)
		GUIScreen::DrawTexture(_itemTexture, _position);
}

void GUIInventoryItem::Update(float deltaTime)
{

}

void GUIInventoryItem::OnMousePressed(const Vector2Int& position)
{

}

void GUIInventoryItem::OnMouseReleased(const Vector2Int& position)
{
	/*std::wcout << _name << std::endl;*/

	if (_item)
	{
		//	물약은 아이템 사용 패킷으로 보내자
		if (_item->GetItemType() == Protocol::ItemType::ITEM_TYPE_CONSUMABLE)
			return;

		Protocol::C_EQUIP_ITEM equipSendPacket;
		auto equipped = !_item->GetEquipped();
		equipSendPacket.set_equipped(equipped);
		equipSendPacket.set_itemdbid(_item->GetItemDbId());

		auto sendBuffer = ServerPacketHandler::MakeSendBuffer(equipSendPacket);
		GNetworkManager->Send(sendBuffer);
	}
}


//	물약은 여러개 보유가능하니깐 설계하자
void GUIInventoryItem::SetItem(Item* item)
{
	ASSERT(item);
	_item = item;

	auto itemData = DataManager::FindItemData(item->GetDataSheetId());
	ASSERT(itemData);

	_itemTexture = GAssetManager->GetTexture(itemData->imagePath);
	ASSERT(_itemTexture);
}
